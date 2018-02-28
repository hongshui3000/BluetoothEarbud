/* Copyright (c) 2016 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */
#include <Python.h>

#include <structmember.h>
#include <assert.h>

#include "read_dwarf.h"
#include "libdwarf.h"
#include "dwarf.h"
#include "libelf.h"
#include "gelf.h"

#define READ_DWARF_CACHE_PUBNAME_DIES 1

/**
\file

read_dwarf C/Python interface layer
*/

#define TRUE 1U
#define FALSE 0U


#define READ_DWARF_VERSION 2

#define SHF_KAL_MINIM 0x80000000

#ifdef _WIN32
#include <io.h>
#define open _open
#define close _close
/* There are two different names out there for the function that gives you the
 * number of program header sections (if it exists at all, that is).  As far as
 * I can tell elf_getphdrnum is the proper name, but elf_getphnum is seen in
 * the Windows port version. */
#define elf_getphdrnum elf_getphnum
#else
#include <unistd.h>
#define O_BINARY 0
#endif
#ifndef PN_XNUM
/* Older libelfs (e.g. RH5) don't have a mechanism to support program
   header tables beyond 0xffff sections, so rather than calling 
   elf_getphnum or elf_getphdrnum you just look up e_phnum and 
   e_phentsize in the main header */
#define NO_EXTENDED_PROGRAM_HEADER_TABLE
#endif


/**
 * Set up the specified exception with a string which combines the supplied
 * one and the built-in libdwarf error string.
 * @param exception The exception to be raised
 * @param rdc read_dwarf context variable
 * @param localerr The custom error string to combine with the libdwarf one
 */
static void set_exception(PyObject *exception, read_dwarf_ctx *rdc,
        const char *localerr)
{
    char *libdwarferr = read_dwarf_errmsg(rdc);
    if (strcmp(libdwarferr,"Dwarf_Error is NULL") != 0)
    {
        char msg[256];
        assert(strlen(libdwarferr) + strlen(localerr) < 256);
        sprintf(msg, "%s: %s", localerr, libdwarferr);
        PyErr_SetString(exception, msg);
    }
    else
    {
        PyErr_SetString(exception, localerr);
    }
}

/**
 * \defgroup parsing [Python interface] Type parsing
 */
/*@{*/
static int parse_type(read_dwarf_ctx *rdc,
                      Dwarf_Die type_die,
                      PyObject *dict,
                      unsigned *type_byte_size,
                      int is_pointed_to,
                      char *typename_start,
                      size_t *typename_len);

static int parse_variable(read_dwarf_ctx *rdc,
                          Dwarf_Die var_die,
                          PyObject *var_dict,
                          unsigned *byte_size);

static PyObject *parse_enumerated_type(read_dwarf_ctx *rdc,
                                       Dwarf_Die enum_die);


/*@}*/


/**
 * \defgroup wrapping [Python interface] Python wrapping
 */
/*@{*/

/**
 * PyObject that simply wraps a DWARF DIE.
 */
typedef struct
{
    PyObject_HEAD
    Dwarf_Off offset;
} Dwarf_Symbol_Ref;

/**
 * PyObject underlying the Dwarf_Reader class.
 */
typedef struct
{
    PyObject_HEAD
    read_dwarf_ctx *rdc;
} Dwarf_Reader;


/**
 * PyObject underlying the Dwarf_Reader class.
 */
typedef struct
{
    PyObject_HEAD
    int fd; /* File descriptor for the open ELF file.  Needs to be closed on
                destruction */
    Elf *elf; /* ELF context pointer for use by libelf */
    Elf32_Half e_machine; /* Which architecture this is.  Different architectures
    tend to have different idiosyncracies. */
    Elf_Scn *dbg_str_sec; /* Pointer to the debug_strings section, if present */
} Elf_Reader;


/******************************************************************************
 * Dwarf_Symbol_Ref class
 *
 *****************************************************************************/

static int Dwarf_Symbol_Ref_init_c(Dwarf_Symbol_Ref *self, Dwarf_Off offset)
{
    self->offset = offset;
    return 0;
}

static void
Dwarf_Symbol_Ref_dealloc(Dwarf_Symbol_Ref *self)
{
    self->ob_type->tp_free((PyObject *)self);
}


/**
 * Python type definition object for the Dwarf_Symbol_Ref class.  Very dull indeed.
 */
static PyTypeObject Dwarf_Symbol_RefType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "_read_dwarf.Dwarf_Symbol_Ref",      /*tp_name*/
    sizeof(Dwarf_Symbol_Ref),            /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)Dwarf_Symbol_Ref_dealloc,/*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT,        /*tp_flags*/
    "DWARF symbol reference",  /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    0,                         /* tp_methods */
    0,                         /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    0,                         /* tp_init */
    0,                         /* tp_alloc */
    0,                         /* tp_new */
};


/**
 * Helper function to create a Dwarf_Symbol_Ref from a die offset.  Returns
 * NULL if creation failed.
 */
static Dwarf_Symbol_Ref *create_symbol_ref(Dwarf_Off die_offset)
{
    Dwarf_Symbol_Ref *symbol_ref = (Dwarf_Symbol_Ref *)
                                        Dwarf_Symbol_RefType.tp_alloc(
                                                 &Dwarf_Symbol_RefType, 0);
    if (symbol_ref == NULL)
    {
        return NULL;
    }

    if (Dwarf_Symbol_Ref_init_c(symbol_ref, die_offset) < 0)
    {
        Py_DECREF(symbol_ref);
        return NULL;
    }
    return symbol_ref;
}



/**
 * Free an array of DIEs
 * \param rdc read_dwarf context
 * \param die_list Array of DIEs
 * \param len Length of list
 */
static void free_die_list(read_dwarf_ctx *rdc, Dwarf_Die *die_list, unsigned len)
{
    unsigned i;
    for (i = 0; i < len; ++i)
    {
        read_dwarf_free_die(rdc, die_list[i]);
    }
    free(die_list);
}


/**
 * Get a dictionary describing the indicated symbol's structure.  The symbol
 * should be a variable.
 *
 * Raises a RuntimeError if anything goes wrong.
 *
 * @param self Dwarf_Reader instance
 * @param args A Dwarf_Symbol_Ref
 * @return Python dictionary containing type names, member names, word offsets,
 * etc
 */
static PyObject * Dwarf_Reader_get_var_struct_dict(Dwarf_Reader *self,
                                                   PyObject *args)
{
    Dwarf_Symbol_Ref *symbol_ref;
    PyObject *struct_dict;
    unsigned type_size;
    Dwarf_Die die;

    if (!PyArg_ParseTuple(args, "O", (PyObject *)&symbol_ref))
    {
        return NULL;
    }

    if (read_dwarf_offdie(self->rdc, symbol_ref->offset, &die) != DW_DLV_OK)
    {
        PyErr_SetString(PyExc_RuntimeError,
                                "Error getting DIE at symbol ref's offset");
        return NULL;
    }

    struct_dict = PyDict_New();
    /* Now walk through the structure type filling in the details */

    if (parse_variable(self->rdc, die, struct_dict, &type_size) != DW_DLV_OK)
    {
        /* parse_struct only returns failure when something
         unrecoverably bad happens, in which case attempting
         to free all the PyObjects is pointless.*/
        PyErr_SetString(PyExc_RuntimeError, "Error parsing structure DIE");
        Py_DECREF(struct_dict);
        read_dwarf_free_die(self->rdc, die);
        return NULL;
    }

    read_dwarf_free_die(self->rdc, die);
    return struct_dict;
}

/**
 * Get a dictionary describing the indicated symbol's structure.  The symbol
 * should be a type.
 *
 * Raises a RuntimeError if anything goes wrong.
 *
 * @param self Dwarf_Reader instance
 * @param args A Dwarf_Symbol_Ref
 * @return Python dictionary containing type names, member names, word offsets,
 * etc
 */
static PyObject * Dwarf_Reader_get_type_struct_dict(Dwarf_Reader *self,
                                                    PyObject *args)
{
    Dwarf_Symbol_Ref *symbol_ref;
    PyObject *struct_dict;
    char typename[256];
    size_t typename_len = 0;
    unsigned byte_size;
    Dwarf_Die die;


    if (!PyArg_ParseTuple(args, "O", (PyObject *)&symbol_ref))
    {
        return NULL;
    }

    if (read_dwarf_offdie(self->rdc, symbol_ref->offset, &die) != DW_DLV_OK)
    {
        PyErr_SetString(PyExc_RuntimeError,
                                "Error getting DIE at symbol ref's offset");
        return NULL;
    }

    struct_dict = PyDict_New();
    /* Now walk through the structure type filling in the details */

    if (parse_type(self->rdc, die, struct_dict, &byte_size, FALSE,
                   /* Point at the RH end of the typename array!*/
                   typename+sizeof(typename)-1, &typename_len) != DW_DLV_OK)
    {
        /* parse_struct only returns failure when something
         unrecoverably bad happens, in which case attempting
         to free all the PyObjects is pointless.*/
        PyErr_SetString(PyExc_RuntimeError, "Error parsing structure DIE");
        Py_DECREF(struct_dict);
        read_dwarf_free_die(self->rdc, die);
        return NULL;
    }
    read_dwarf_free_die(self->rdc, die);
    return struct_dict;
}

/**
 * Return a boolean indicating if the supplied symbol has the DW_AT_external
 * attribute or not
 * @param self Dwarf_Reader context
 * @param args A Dwarf_Symbol_Ref
 * @return True if the attribute is present else False
 */
static PyObject * Dwarf_Reader_symbol_is_external(Dwarf_Reader *self,
                                                  PyObject *args)
{
    PyObject *symbol_ref;
    Dwarf_Bool has_attr;
    Dwarf_Die die;

    if (!PyArg_ParseTuple(args, "O", &symbol_ref))
    {
        return NULL;
    }

    if (read_dwarf_offdie(self->rdc, ((Dwarf_Symbol_Ref *)symbol_ref)->offset,
                                                    &die) != DW_DLV_OK)
    {
        PyErr_SetString(PyExc_RuntimeError,
                                "Error getting DIE at symbol ref's offset");
        return NULL;
    }

    if (read_dwarf_has_attr(self->rdc, die,
                            DW_AT_external, &has_attr, NULL, NULL) != DW_DLV_OK)
    {
        set_exception(PyExc_RuntimeError, self->rdc,
                        "Error checking for external attribute!");
        read_dwarf_free_die(self->rdc, die);
        return NULL;
    }
    read_dwarf_free_die(self->rdc, die);
    return PyInt_FromLong(has_attr);
}

/**
 * Dwarf_Reader initialisation
 *
 * @param self New Dwarf_Reader instance
 * @param args Name of ELF file to open
 * @return 0 on sucess, -1 on failure
 */
static int Dwarf_Reader_init(Dwarf_Reader *self, PyObject *args)
{
    PyObject *elf_filename_or_elf_reader;
    char *elf_filename = NULL;
    Elf *elf = NULL;

    if (! PyArg_ParseTuple(args, "O", &elf_filename_or_elf_reader))
    {
        return -1;
    }

    if (PyString_Check(elf_filename_or_elf_reader))
    {
        elf_filename = PyString_AsString(elf_filename_or_elf_reader);
    }
    else
    {
        elf = ((Elf_Reader *)elf_filename_or_elf_reader)->elf;
    }

    if ( (self->rdc = read_dwarf_init(elf, elf_filename,
                                      READ_DWARF_VERBOSITY_ERRORS)) == NULL)
    {
        PyErr_SetString(PyExc_RuntimeError, "ELF initialisation failed");
        return -1;
    }

    return 0;
}

static PyObject *Dwarf_Reader_set_verbosity(Dwarf_Reader *self, PyObject *args)
{
    int new_level;
    READ_DWARF_VERBOSITY old_level;
    if (!PyArg_ParseTuple(args, "i", &new_level))
    {
        return NULL;
    }

    switch((READ_DWARF_VERBOSITY)new_level)
    {
    case READ_DWARF_VERBOSITY_SILENT:
    case READ_DWARF_VERBOSITY_ERRORS:
    case READ_DWARF_VERBOSITY_WARNINGS:
    case READ_DWARF_VERBOSITY_INFO:
        old_level = read_dwarf_set_verbosity(self->rdc, new_level);
        break;
    default:
        PyErr_SetString(PyExc_ValueError, "Unrecognised verbosity level");
        return NULL;
    }
    return PyInt_FromLong(old_level);
}

static PyObject *Dwarf_Reader_print_mem_stats(Dwarf_Reader *self, PyObject *args)
{
    read_dwarf_mem_stats(self->rdc);
    Py_INCREF(Py_None);
    return Py_None;
}

/**
 * Helper function to add a given DIE to a given dictionary using the given
 * name as the key
 * @param rdc read_dwarf context variable
 * @param glob_dict Dictionary to add the DIE to
 * @param name name to add it under
 * @param die DIE to add
 * @return DW_DLV_OK if it worked, else DW_DLV_ERROR, in which case an exception
 * is ready to be raised.
 */
static int set_symbol_dict_entry(read_dwarf_ctx *rdc, PyObject *glob_dict,
                                 const char *name, Dwarf_Off die_offset)
{
    Dwarf_Symbol_Ref *symbol_ref = create_symbol_ref(die_offset);

    if (symbol_ref == NULL)
    {
        PyErr_SetString(PyExc_RuntimeError, "Error initialising symbol ref");
        return DW_DLV_ERROR;
    }

    if (PyDict_SetItemString(glob_dict, name, (PyObject *)symbol_ref) < 0)
    {
        Py_DECREF(symbol_ref);
        PyErr_SetString(PyExc_RuntimeError,
                        "Error setting dictionary entry!");
        return DW_DLV_ERROR;
    }
    /* SetItem increments the object's reference count, so we must
     * throw away our reference to it */
    Py_DECREF(symbol_ref);
    return DW_DLV_OK;
}

/**
 * Helper function to construct a Python list containing the child entities of
 * of a function of any given type
 * @param rdc read_dwarf context variable
 * @param func_die DIE of type DW_TAG_subroutine_type
 * @param tag The tag to gather (e.g. DW_TAG_formal_parameter, DW_TAG_inlined_subroutine)
 * @param return_names If TRUE, return a list of (name, symbol_ref) pairs, otherwise
 * just return a list of the symbol refs
 * @return Python list containing the requested entries
 */
static PyObject *get_func_child_list(read_dwarf_ctx *rdc, Dwarf_Die func_die,
                                     Dwarf_Half tag, int return_names)
{
    PyObject *ret;
    Dwarf_Die *param_dies;
    unsigned n_dies, i;
    char *name;


    /* Get the list of DIEs from the DWARF */
    if (read_dwarf_get_func_child_entries(rdc,
                                     func_die,
                                     tag,
                                     &param_dies,
                                     &n_dies) != DW_DLV_OK)
    {
        set_exception(PyExc_RuntimeError, rdc,
                      "Failed to get formal parameters for function");
        return NULL;
    }

    /* Now put them all in a nice Python list */
    ret = PyList_New(n_dies);
    if (n_dies)
    {
        for (i = 0; i < n_dies; ++i)
        {
            Dwarf_Symbol_Ref *symbol_ref;
            if ((symbol_ref = create_symbol_ref(
                                  read_dwarf_die_offset(rdc, param_dies[i])))
                                                                       == NULL)
            {
                PyErr_SetString(PyExc_RuntimeError,
                                "Error initialising Dwarf_Symbol_Ref");
                Py_DECREF(ret);
                free_die_list(rdc, param_dies,  n_dies);
                return NULL;
            }

            if (return_names)
            {
                /* Get the names of the parameters for convenience (as they aren't
                 * available apart from the DWARF */
                switch (read_dwarf_diename(rdc, param_dies[i], &name))
                {
                case DW_DLV_ERROR:
                    set_exception(PyExc_RuntimeError, rdc,
                            "Error getting formal paramter name!");
                    return NULL;
                case DW_DLV_NO_ENTRY:
                    name = "<Name not found>";
                    break;
                default:
                    break;
                }
                /* Create a name, symbol tuple so that we preserve the ordering of
                 * the parameters for better display-ability */

                /* PyList_SET_ITEM steals the reference to the tuple so we don't need
                 * a DECREF on it after setting (as we would with a dict).  However
                 * we do now need a DECREF on symbol_ref, since it's now owned by
                 * the tuple */
                PyList_SET_ITEM(ret, i,
                                Py_BuildValue("(sO)", name, (PyObject *)symbol_ref));
                Py_DECREF(symbol_ref);
            }
            else
            {
                /* PyList_SET_ITEM steals the reference to symbol_ref so we don't need
                 * a DECREF on symbol_ref after setting (as we would with a dict) */
                PyList_SET_ITEM(ret, i, (PyObject *)symbol_ref);
            }
        }
        free_die_list(rdc, param_dies, n_dies);
    }
    return ret;
}

static PyObject *get_formal_param_list(read_dwarf_ctx *rdc, Dwarf_Die func_die)
{
    return get_func_child_list(rdc, func_die, DW_TAG_formal_parameter, TRUE);
}

static PyObject *get_local_var_list(read_dwarf_ctx *rdc, Dwarf_Die func_die)
{
    return get_func_child_list(rdc, func_die, DW_TAG_variable, TRUE);
}

static PyObject *get_inline_call_list(read_dwarf_ctx *rdc, Dwarf_Die func_die)
{
    return get_func_child_list(rdc, func_die, DW_TAG_inlined_subroutine, FALSE);
}


/**
 * Simple structure to pass a list of DWARF tags to the DIE tree traversal
 * function.  Includes a flag to indicate whether typedefs just be treated as
 * just another tag, or whether they should be followed, i.e. treated as
 * having the tag of whatever they refer to.
 */
typedef struct
{
    Dwarf_Half *tags;
    Dwarf_Unsigned n_tags;
    int follow_typedefs;
} TARGET_TAGS;

/**
 * Helper function that indicates if the given tag is in the given list of
 * target tags
 * @param tags The target tags to search through
 * @param tag The tag to search for
 * @return If the follow_typedefs flag is set in @p tag, there are three cases:
 *                      1) tag wasn't a typedef and was in the list
 *                      2) tag wasn't a typedef and wasn't in the lsit
 *                      3) tag was a typedef
 *         If the follow_typedefs flag isn't set, only the first two occur
 */
enum TAG_IN_TARGET_TAGS
{
    TAG_IN_TARGET_TAGS_NOT_FOUND = 0,
    TAG_IN_TARGET_TAGS_FOUND = 1,
    TAG_IN_TARGET_TAGS_TYPEDEF = 2
};


static enum TAG_IN_TARGET_TAGS tag_in_target_tags(const TARGET_TAGS *tags,
                                                  Dwarf_Half tag)
{
    unsigned i;
    if (tags->follow_typedefs && tag == DW_TAG_typedef)
    {
        return TAG_IN_TARGET_TAGS_TYPEDEF;
    }

    for (i = 0; i < tags->n_tags; ++i)
    {
        if (tags->tags[i] == tag)
        {
            return TAG_IN_TARGET_TAGS_FOUND;
        }
    }
    return TAG_IN_TARGET_TAGS_NOT_FOUND;
}

/**
 * Traversal action helper adding symbols with a given tag to a Python dict
 * by name if they don't have the "Declaration" attribute (i.e. they are
 * definitions, not just declarations)
 * @param rdc read_dwarf context
 * @param die DIE being acted upon
 * @param level Recursion level (unused)
 * @param externals_only Only add symbols which have the External attribute
 * @return DW_DLV_OK if symbol added, else DW_DLV_NO_ENTRY
 */
static int add_tagged_symbols_to_dict_action_impl(read_dwarf_ctx *rdc,
                                                  Dwarf_Die die, int level,
                                                  int externals_only)
{
    PyObject *dict = (PyObject *)(rdc->scratch_out);
    TARGET_TAGS *search_tags = (TARGET_TAGS *)(rdc->scratch_in);
    Dwarf_Half tag;
    enum TAG_IN_TARGET_TAGS tag_found;
    int via_typedef = FALSE;

    (void)level;

    /* Does this symbol have the specified tag? */
    if (search_tags->n_tags == 0 ||
        (read_dwarf_tag(rdc, die, &tag) == DW_DLV_OK))
    {
        /* First we may need to follow a typedef to find the tag of the thing that
         * has been aliased */
        Dwarf_Die child = 0;
        Dwarf_Die parent;
        tag_found = tag_in_target_tags(search_tags, tag);
        parent = die;
        while (tag_found == TAG_IN_TARGET_TAGS_TYPEDEF)
        {
            via_typedef = TRUE;
            if (read_dwarf_get_immediate_type(rdc, parent, &child) != DW_DLV_OK ||
                    read_dwarf_tag(rdc, child, &tag) != DW_DLV_OK)
            {
                return DW_DLV_NO_ENTRY;
            }
            tag_found = tag_in_target_tags(search_tags, tag);
            if (parent != die)
            {
                read_dwarf_free_die(rdc, parent);
            }
            parent = child;
        }
        read_dwarf_free_die(rdc, child);

        /* Now tag_found should be either FOUND or NOT_FOUND, so we add the
         * entry if the former */
        if (tag_found == TAG_IN_TARGET_TAGS_FOUND)
        {
            Dwarf_Bool has_attr;
            if ((!externals_only ||
                    (read_dwarf_has_attr(rdc, die, DW_AT_external, &has_attr, NULL, NULL) ==
                                                            DW_DLV_OK && has_attr))
                 &&
                read_dwarf_has_attr(rdc, die, DW_AT_declaration, &has_attr, NULL, NULL) ==
                                                            DW_DLV_OK && !has_attr)
            {
                char *name;
                char new_name[256];
                char anon_enum_name[64];
                Dwarf_Off die_offset;
                if (!via_typedef && tag == DW_TAG_pointer_type)
                {
#ifdef GATHER_POINTER_TYPES
                    /* Pointer entries don't have their own name - we need to
                     * construct it from the pointed-to type's name */
                    char *typename = read_dwarf_typename(rdc, die);
                    if (typename != NULL)
                    {
                        assert(strlen(typename) + 1 < sizeof(new_name));
                        strcpy(new_name, typename);
                        free(typename);
                        name = new_name;
                    }
#else
                    /* It's not clear that we want to gather pointer entries
                     * themselves. We can generate the type dicts for pointer
                     * types automagically in the Python if we actually need
                     * them */
                    return DW_DLV_NO_ENTRY;
#endif
                }
                else if (read_dwarf_is_abstract_entry(rdc, die))
                {
                    /* We don't want to gather DIEs representing abstract
                     * entities such as inlined functions, since these aren't
                     * directly of interest to the debugger.
                     */
                    return DW_DLV_NO_ENTRY;
                }
                else
                {
                    switch(read_dwarf_diename(rdc, die, &name))
                    {
                    case DW_DLV_ERROR:
                        return DW_DLV_NO_ENTRY;
                    case DW_DLV_NO_ENTRY:
                        switch(tag)
                        {
                        case DW_TAG_subprogram:
                        /* It's possible to encounter function entries without
                         * names, because they are out-of-line instances of
                         * functions marked as inline: the name lives in the
                         * corresponding abstract instance. */
                        {
                            Dwarf_Die abstract_die;
                            switch(read_dwarf_abstract_origin(rdc, die, &abstract_die))
                            {
                            case DW_DLV_NO_ENTRY:
                            case DW_DLV_ERROR:
                                return DW_DLV_NO_ENTRY;
                            default:
                                break;
                            }
                            if (read_dwarf_diename(rdc, abstract_die, &name) != DW_DLV_OK)
                            {
                                read_dwarf_free_die(rdc, abstract_die);
                                return DW_DLV_NO_ENTRY;
                            }
                            read_dwarf_free_die(rdc, abstract_die);
                        }
                            break;
                        case DW_TAG_enumeration_type:
                        {
                            /* It is possible to encounter enumeration types
                             * without names, because the name may only be
                             * provided via a typedef.  But the compiler might
                             * not insert the typedef itself into the DWARF info
                             * if it isn't explicitly used, which renders the
                             * enum invisible to read_dwarf unless we insert it
                             * with a default name */
                            /* Construct the name from the DIE offset */
                            Dwarf_Off die_offset;
                            if ((die_offset = read_dwarf_die_offset(rdc, die))
                                               == READ_DWARF_INVALID_DIE_OFFSET)
                            {
                                return DW_DLV_NO_ENTRY;
                            }
                            sprintf(anon_enum_name, "#%lx",
                                    (unsigned long)die_offset);
                            name = anon_enum_name;
                        }
                            break;
                        default:
                            return DW_DLV_NO_ENTRY;
                        break;
                        }
                        break;
                    default:
                        break;
                    }
                }
                if (PyDict_GetItemString(dict, name) != NULL)
                {
                    return DW_DLV_NO_ENTRY;
                }

                /* Add the type prefix in if appropriate */
                if (!via_typedef && (tag == DW_TAG_structure_type ||
                                     tag == DW_TAG_enumeration_type ||
                                     tag == DW_TAG_union_type))
                {
                    char *write_at = new_name;
                    const char *prefix = read_dwarf_typename_prefix(tag);
                    /* Check the name of the variable plus "struct " won't
                     * overflow the buffer */
                    assert(strlen(name) + strlen(prefix) + 1 < sizeof(new_name));
                    strcpy(new_name, prefix);
                    write_at += strlen(prefix);
                    strcpy(write_at, name);
                    name = new_name;
                }

                die_offset = read_dwarf_die_offset(rdc, die);
                if (set_symbol_dict_entry(rdc, dict, name, die_offset) != DW_DLV_OK)
                {
                    /* If set_symbol_dict_entry return error, it will have set up
                     * a RuntimeException.  The caller should interpret a traversal
                     * cancellation as meaning the dictionary is broken.  */
                    rdc->cancel_traversal = TRUE;
                }
            }
        }
    }
    return DW_DLV_NO_ENTRY;
}

/**
 * Wrapper around \c add_tagged_symbols_to_dict_action_impl that sets the
 * "externals_only" flag to FALSE
 * @param rdc read_dwarf context
 * @param die DIE being acted upon
 * @param level Recursion level (unused)
 * @return DW_DLV_OK if symbol added, else DW_DLV_NO_ENTRY
 */
static int add_tagged_symbols_to_dict_action(read_dwarf_ctx *rdc,
                                             Dwarf_Die die, int level)
{
    return add_tagged_symbols_to_dict_action_impl(rdc, die, level, FALSE);
}

/**
 * Wrapper around \c add_tagged_symbols_to_dict_action_impl that sets the
 * "externals_only" flag to TRUE
 * @param rdc read_dwarf context
 * @param die DIE being acted upon
 * @param level Recursion level (unused)
 * @return DW_DLV_OK if symbol added, else DW_DLV_NO_ENTRY
 */
static int add_external_tagged_symbols_to_dict_action(read_dwarf_ctx *rdc,
                                                      Dwarf_Die die, int level)
{
    return add_tagged_symbols_to_dict_action_impl(rdc, die, level, TRUE);
}

/**
 * Helper function to implement get_tag_dict and get_type_dict: looks for all
 * symbols whose tag is in the given list
 * @param rdc read_dwarf context
 * @param target_tag_list List of tags to match
 * @param n_tags Number of tags to match
 * @param globals_only Should we just collect DIEs with the external attribute
 * @return Dictionary-of-dictionaries containing all matching symbols, organised
 * by CU
 */
static PyObject *search_globals_and_cus(read_dwarf_ctx *rdc,
                                        Dwarf_Half target_tag_list[],
                                        Dwarf_Unsigned n_tags,
                                        unsigned char globals_only,
                                        unsigned char follow_typedefs)
{
#if READ_DWARF_CACHE_PUBNAME_DIES
    Dwarf_Global *globals;
    Dwarf_Die global_die;
    PyObject *glob_dict;
    Dwarf_Signed ngbls;
    Dwarf_Off die_offset;
    Dwarf_Half tag;
    char *name;
    unsigned i;
#endif
    Dwarf_Die next_cu;
    PyObject *master_dict, *cu_dict;
    int retval;
    TARGET_TAGS target_tags;

    target_tags.tags = target_tag_list;
    target_tags.n_tags = n_tags;
    target_tags.follow_typedefs = follow_typedefs;

    /* This is a dictionary of dictionaries, one per CU and one for the DWARF
     * globals */
    master_dict = PyDict_New();
    if (!master_dict)
    {
        return NULL;
    }

    /* I think having the globals listed explicitly is a waste of space because
     * they all appear inside CUs in any case.
     *
     * EDIT: not true of GNU ARM ELFs.*/
#if READ_DWARF_CACHE_PUBNAME_DIES
    /* Get the explicit Dwarf Globals first */
    switch (read_dwarf_get_globals(rdc, &globals, &ngbls))
    {
    case DW_DLV_ERROR:
        set_exception(PyExc_RuntimeError, rdc, "Error getting globals");
        return NULL;
    case DW_DLV_NO_ENTRY:
        ngbls = 0;
        break;
    default:
        break;
    }
    if (ngbls > 0)
    {
        glob_dict = PyDict_New();
        if (!glob_dict)
        {
            read_dwarf_free_globals(rdc, globals, ngbls);
            Py_DECREF(master_dict);
            return NULL;
        }
        if (PyDict_SetItemString(master_dict,"<globals>", glob_dict) < 0)
        {
            read_dwarf_free_globals(rdc, globals, ngbls);
            Py_DECREF(glob_dict);
            Py_DECREF(master_dict);
            PyErr_SetString(PyExc_RuntimeError,
                            "Couldn't add globals dict to master dict!");
            return NULL;
        }
        /* PyDict_SetItemString INCREFs the item being set */
        Py_DECREF(glob_dict);

        /* Search for those matching the target tag and shove them into the dict */
        for (i = 0; i < ngbls; ++i)
        {
            if (read_dwarf_get_global_die(rdc, globals[i], &global_die) != DW_DLV_OK ||
                    read_dwarf_tag(rdc, global_die, &tag) != DW_DLV_OK)
            {
                set_exception(PyExc_RuntimeError, rdc, "Error getting global's tag");
                read_dwarf_free_globals(rdc, globals, ngbls);
                Py_DECREF(master_dict);
                return NULL;
            }

            {
                Dwarf_Die child = 0;
                Dwarf_Die parent;
                enum TAG_IN_TARGET_TAGS tag_found = tag_in_target_tags(&target_tags, tag);
                parent = global_die;
                while (tag_found == TAG_IN_TARGET_TAGS_TYPEDEF)
                {
                    if (read_dwarf_get_immediate_type(rdc, parent, &child) != DW_DLV_OK ||
                            read_dwarf_tag(rdc, child, &tag) != DW_DLV_OK)
                    {
                        read_dwarf_free_globals(rdc, globals, ngbls);
                        read_dwarf_free_die(rdc, parent);
                        Py_DECREF(glob_dict);
                        Py_DECREF(master_dict);
                        PyErr_SetString(PyExc_RuntimeError,
                                        "Couldn't add globals dict to master dict!");
                        return NULL;
                    }
                    tag_found = tag_in_target_tags(&target_tags, tag);
                    if (parent != global_die)
                    {
                        read_dwarf_free_die(rdc, parent);
                    }
                    parent = child;
                }
                read_dwarf_free_die(rdc, child);
            }

            die_offset = read_dwarf_die_offset(rdc, global_die);

            if (tag_in_target_tags(&target_tags, tag) == TAG_IN_TARGET_TAGS_FOUND)
            {
                Dwarf_Bool has_attr;
                /* Don't both with functions that don't have a low PC attribute*/
                if (tag != DW_TAG_subprogram ||
                        (read_dwarf_has_attr(rdc, global_die, DW_AT_low_pc, &has_attr,
                                NULL, NULL) == DW_DLV_OK && has_attr))
                {

                    if (read_dwarf_get_globname(rdc, globals[i], &name) != DW_DLV_OK)
                    {
                        set_exception(PyExc_RuntimeError, rdc, "Error getting global's name");
                        read_dwarf_free_globals(rdc, globals, ngbls);
                        read_dwarf_free_die(rdc, global_die);
                        Py_DECREF(master_dict);
                        return NULL;
                    }

                    if (set_symbol_dict_entry(rdc, glob_dict, name, die_offset) != DW_DLV_OK)
                    {
                        /* If set_symbol_dict_entry return error, it will have set up
                         * a RuntimeException, so we can return NULL */
                        Py_DECREF(master_dict);
                        read_dwarf_free_die(rdc, global_die);
                        read_dwarf_free_globals(rdc, globals, ngbls);
                        read_dwarf_free_globname(rdc, name);
                        return NULL;
                    }
                    read_dwarf_free_globname(rdc, name);
                }
                read_dwarf_free_die(rdc, global_die);
            }
        }
        read_dwarf_free_globals(rdc, globals, ngbls);
    }
#endif

    /* Now we need to run through the CUs  */
    while((retval = read_dwarf_get_next_cu(rdc, &next_cu)) != DW_DLV_NO_ENTRY)
    {
        char *cu_name;

        if (retval == DW_DLV_ERROR)
        {
            /* If we get an error accessing a CU, back out and get ready to die
             */
            Py_DECREF(master_dict);
            set_exception(PyExc_RuntimeError, rdc, "Error getting CU!");
            return NULL;
        }

        /* Get the CU's name */
        if ((cu_name = read_dwarf_cu_name(rdc, next_cu, TRUE)) == NULL)
        {
            Py_DECREF(master_dict);
            set_exception(PyExc_RuntimeError, rdc, "Error getting CU name!");
            return NULL;
        }
        if ((cu_dict = PyDict_GetItemString(master_dict, cu_name)) == NULL)
        {
            /* Create a new dictionary for this CU */
            cu_dict = PyDict_New();
            if (!cu_dict)
            {
                Py_DECREF(master_dict);
                free(cu_name);
                return NULL;
            }
            /* Insert the CU dictionary */
            if (PyDict_SetItemString(master_dict, cu_name, cu_dict) < 0)
            {
                Py_DECREF(master_dict);
                Py_DECREF(cu_dict);
                free(cu_name);
                set_exception(PyExc_RuntimeError, rdc, "Error inserting CU "
                        "dict into master dict!");
                return NULL;
             }
            /* master_dict now owns the memory and we don't want any other references
             * hanging around */
            Py_DECREF(cu_dict);
        }
        free(cu_name);

        /* Traverse the CU tree.  */
        rdc->scratch_in = (void *)(&target_tags);
        rdc->scratch_out = (void *)cu_dict;
        if (read_dwarf_traverse_die_tree(rdc,
                                         next_cu,
                                         globals_only ? add_external_tagged_symbols_to_dict_action :
                                         add_tagged_symbols_to_dict_action,
                                         0, 0, FALSE, TRUE) == DW_DLV_NO_ENTRY)
        {
            read_dwarf_free_die(rdc, next_cu);
        }
        if (rdc->cancel_traversal)
        {
            /* add_tagged_symbols_to_dict_action will only cancel traversal
             * on error */
            Py_DECREF(master_dict);
            set_exception(PyExc_RuntimeError, rdc, "Error during CU tree "
                    "traversal");
            return NULL;
        }
    }

    return master_dict;
}

/**
 * Create a dictionary of Dwarf_Symbol_Refs for all the symbols with a
 * particular tag type.
 *
 * Raises RuntimeError if anything goes wrong.
 *
 * @param self Dwarf_Reader instance
 * @return Python dictionary of Dwarf_Symbol_Refs for all the global variable
 * symbols
 */
static PyObject * Dwarf_Reader_get_tag_dict(Dwarf_Reader *self,
                                                 PyObject *args)
{
    Dwarf_Half target_tag;
    unsigned char globals_only = 0;
    unsigned char follow_typedefs = 0;
    if (PyArg_ParseTuple(args, "H|bb", &target_tag, &globals_only,
                                                        &follow_typedefs) < 0)
    {
        return NULL;
    }

    return search_globals_and_cus(self->rdc, &target_tag, 1, globals_only,
                                                            follow_typedefs);
}

/**
 * Return a dictionary containing all CU references indexed by name.  If multiple
 * CU entries with the same name are encountered, they are inserted into a list;
 * otherwise the dictionary entries are just symbol references.
 */
static PyObject *Dwarf_Reader_get_cu_ref_dict(Dwarf_Reader *self)
{
    int retval;
    Dwarf_Die next_cu;
    PyObject *cu_dict, *cu_entry;

    cu_dict = PyDict_New();

    while((retval = read_dwarf_get_next_cu(self->rdc, &next_cu)) != DW_DLV_NO_ENTRY)
    {
        char *cu_name;
        Dwarf_Symbol_Ref *symbol_ref;

        if (retval == DW_DLV_ERROR)
        {
            /* If we get an error accessing a CU, back out and get ready to die
             */
            Py_DECREF(cu_dict);
            read_dwarf_free_die(self->rdc, next_cu);
            set_exception(PyExc_RuntimeError, self->rdc, "Error getting CU!");
            return NULL;
        }

        /* Get the CU's name */
        if ((cu_name = read_dwarf_cu_name(self->rdc, next_cu, TRUE)) == NULL)
        {
            Py_DECREF(cu_dict);
            read_dwarf_free_die(self->rdc, next_cu);
            set_exception(PyExc_RuntimeError, self->rdc, "Error getting CU name!");
            return NULL;
        }

        if ((symbol_ref = create_symbol_ref(
                              read_dwarf_die_offset(self->rdc, next_cu))) == NULL)
        {
            Py_DECREF(cu_dict);
            free(cu_name);
            read_dwarf_free_die(self->rdc, next_cu);
            set_exception(PyExc_RuntimeError, self->rdc, "Error creating symbol ref!");
            return NULL;
        }
        read_dwarf_free_die(self->rdc, next_cu);
        /* If there's already an entry for this name, we need to make the value
         * into a list if it isn't already one, and then append the new entry
         * to the list.  We don't just automatically create lists for every
         * entry because in nearly all cases that's a waste of time */
        if ((cu_entry = PyDict_GetItemString(cu_dict, cu_name)) != NULL)
        {
            /* PyDict_GetItemString returns a borrowed reference, but in both
             * the case that cu_entry is a list and the case that it's not, we
             * need it to behave as if it's a reference we own, albeit for
             * completely different reasons */
            Py_INCREF(cu_entry);
            /* If the entry isn't already a list, insert it into a list */
            if (!PyList_Check(cu_entry))
            {
                PyObject *cu_entry_list;
                if ((cu_entry_list = PyList_New(1))== NULL)
                {
                    Py_DECREF(symbol_ref);
                    Py_DECREF(cu_entry);
                    Py_DECREF(cu_dict);
                    free(cu_name);
                    return NULL;
                }
                PyList_SET_ITEM(cu_entry_list, 0, cu_entry);
                if (PyDict_SetItemString(cu_dict, cu_name, cu_entry_list) < 0)
                {
                    Py_DECREF(symbol_ref);
                    Py_DECREF(cu_entry);
                    Py_DECREF(cu_entry_list);
                    Py_DECREF(cu_dict);
                    free(cu_name);
                    return NULL;
                }
                /* Rename the list so that we don't need to care subsequently
                 * whether we went through the list creation route or not */
                cu_entry = cu_entry_list;
            }
            /* Now add the new entry to the list */
            if (PyList_Append(cu_entry, (PyObject *)symbol_ref) < 0)
            {
                Py_DECREF(symbol_ref);
                Py_DECREF(cu_entry);
                Py_DECREF(cu_dict);
                free(cu_name);
                return NULL;
            }
            Py_DECREF(cu_entry);
        }
        else if (PyDict_SetItemString(cu_dict, cu_name, (PyObject *)symbol_ref) < 0)
        {
            Py_DECREF(symbol_ref);
            Py_DECREF(cu_dict);
            free(cu_name);
            set_exception(PyExc_RuntimeError, self->rdc, "Error inserting symbol ref!");
            return NULL;
        }
        free(cu_name);
        Py_DECREF(symbol_ref);
    }
    return cu_dict;
}

/**
 * Get the master dictionary of type DIEs, i.e. arrays, enumerations, pointers,
 * structures, unions, base types, and typedefs
 * @param self Dwarf_Reader
 * @return Master dictionary described above
 */
static PyObject *Dwarf_Reader_get_type_dict(Dwarf_Reader *self)
{
    Dwarf_Half tags[] = {DW_TAG_array_type,
                         DW_TAG_enumeration_type,
                         DW_TAG_pointer_type,
                         DW_TAG_structure_type,
                         DW_TAG_union_type,
                         DW_TAG_base_type,
                         DW_TAG_subroutine_type};

    return search_globals_and_cus(self->rdc, tags, sizeof(tags)/sizeof(tags[0]),
                                  FALSE, TRUE);
}

/**
 * Get a dictionary of function DIE offsets, excluding any that are marked as
 * inlined, as there's nothing very useful we can do with those.
 *
 * Note on reference counting: PyDict_Next borrows references to the keys and
 * values it retrieves from the dictionaries.  So unlike when we're constructing
 * new objects to put into dictionaries, we *don't* want to decref the values
 * we add to the filtered CU dictionaries.  (However, we *do* need to decref the
 * filtered CU dictinonaries themselves as they are newly created objects).
 */
static PyObject *Dwarf_Reader_get_func_dict(Dwarf_Reader *self)
{
    Dwarf_Half func_tags[] = {DW_TAG_subprogram};
    PyObject *all_funcs;
#if 0
    PyObject *non_inline_funcs;
    PyObject *dict_key, *dict_value;
    Py_ssize_t dict_pos;
#endif

    all_funcs = search_globals_and_cus(self->rdc,
            func_tags, sizeof(func_tags)/sizeof(func_tags[0]), FALSE, FALSE);

#if 0
    non_inline_funcs = PyDict_New();
    if (non_inline_funcs == NULL)
    {
        Py_DECREF(all_funcs);
        return NULL;
    }

    dict_pos = 0;
    while(PyDict_Next(all_funcs, &dict_pos, &dict_key, &dict_value))
    {
        Py_ssize_t pos = 0;
        PyObject *key;
        PyObject *value;
        PyObject * non_inline_cu_funcs = PyDict_New();
        if (non_inline_cu_funcs == NULL)
        {
            Py_DECREF(all_funcs);
            Py_DECREF(non_inline_funcs);
            return NULL;
        }
        while(PyDict_Next(dict_value, &pos, &key, &value))
        {

            Dwarf_Die die;
            Dwarf_Off offset = ((Dwarf_Symbol_Ref *)value)->offset;
            Dwarf_Bool has_attr;
            Dwarf_Half attr_form;
            Dwarf_Attribute attr;
            Dwarf_Unsigned attr_value;
            int ret;

            if (read_dwarf_offdie(self->rdc, offset, &die) != DW_DLV_OK)
            {
                Py_DECREF(all_funcs);
                Py_DECREF(non_inline_funcs);
                Py_DECREF(non_inline_cu_funcs);
                set_exception(PyExc_RuntimeError, self->rdc,
                                        "Error getting DIE at symbol ref's offset");
                return NULL;
            }

            ret = read_dwarf_has_attr(self->rdc, die, DW_AT_inline, &has_attr,
                                        &attr_form, &attr);
            if (ret == DW_DLV_ERROR)
            {
                read_dwarf_free_die(self->rdc, die);
                Py_DECREF(all_funcs);
                Py_DECREF(non_inline_funcs);
                Py_DECREF(non_inline_cu_funcs);
                set_exception(PyExc_RuntimeError, self->rdc, "Error getting inline attribute");
                return NULL;
            }

            if (!has_attr)
            {
                if (PyDict_SetItem(non_inline_cu_funcs, key, value) < 0)
                {
                    read_dwarf_free_die(self->rdc, die);
                    Py_DECREF(all_funcs);
                    Py_DECREF(non_inline_funcs);
                    Py_DECREF(non_inline_cu_funcs);
                    set_exception(PyExc_RuntimeError, self->rdc, "Error setting dict item");
                    return NULL;
                }
            }
            else
            {
                if (ret == DW_DLV_NO_ENTRY)
                {
                    if (PyDict_SetItem(non_inline_cu_funcs, key, value) < 0)
                    {
                        read_dwarf_free_die(self->rdc, die);
                        Py_DECREF(all_funcs);
                        Py_DECREF(non_inline_funcs);
                        Py_DECREF(non_inline_cu_funcs);
                        set_exception(PyExc_RuntimeError, self->rdc, "Error setting dict item");
                        return NULL;
                    }
                }
                else
                {
                    if(read_dwarf_get_unsigned_attr(self->rdc, attr, &attr_value) != DW_DLV_OK)
                    {
                        read_dwarf_free_die(self->rdc, die);
                        Py_DECREF(all_funcs);
                        Py_DECREF(non_inline_funcs);
                        Py_DECREF(non_inline_cu_funcs);
                        set_exception(PyExc_RuntimeError, self->rdc, "Error getting inline attribute");
                        return NULL;
                    }
                    switch(attr_value)
                    {
                    case DW_INL_not_inlined:
                    case DW_INL_declared_not_inlined:
                        if (PyDict_SetItem(non_inline_cu_funcs, key, value) < 0)
                        {
                            read_dwarf_free_die(self->rdc, die);
                            Py_DECREF(all_funcs);
                            Py_DECREF(non_inline_funcs);
                            Py_DECREF(non_inline_cu_funcs);
                            set_exception(PyExc_RuntimeError, self->rdc, "Error setting dict item");
                            return NULL;
                        }
                        break;
                    default:
                    {
                        Dwarf_Bool has_abstract_origin_attr;
                        if (read_dwarf_has_attr(self->rdc, die,
                                                DW_AT_abstract_origin,
                                                &has_abstract_origin_attr,
                                                NULL, NULL) == DW_DLV_OK &&
                                                has_abstract_origin_attr)
                        {
                            if (PyDict_SetItem(non_inline_cu_funcs, key, value) < 0)
                            {
                                read_dwarf_free_die(self->rdc, die);
                                Py_DECREF(all_funcs);
                                Py_DECREF(non_inline_funcs);
                                Py_DECREF(non_inline_cu_funcs);
                                set_exception(PyExc_RuntimeError, self->rdc, "Error setting dict item");
                                return NULL;
                            }
                        }
                    }
                    break;
                    }
                }
            }
            read_dwarf_free_die(self->rdc, die);
        }
        if (PyDict_SetItem(non_inline_funcs, dict_key, non_inline_cu_funcs) < 0)
        {
            Py_DECREF(all_funcs);
            Py_DECREF(non_inline_funcs);
            Py_DECREF(non_inline_cu_funcs);
            set_exception(PyExc_RuntimeError, self->rdc, "Error setting dict item");
            return NULL;
        }
        Py_DECREF(non_inline_cu_funcs);
    }
    Py_DECREF(all_funcs);
    return non_inline_funcs;
#else
    return all_funcs;
#endif
}

static PyObject *get_attr(read_dwarf_ctx *rdc, Dwarf_Off offset, Dwarf_Half attr_id)
{
    Dwarf_Bool has_attr;
    Dwarf_Half form;
    Dwarf_Attribute attr;
    Dwarf_Die die;

    if (read_dwarf_offdie(rdc, offset, &die) != DW_DLV_OK)
    {
        PyErr_SetString(PyExc_RuntimeError,
                                "Error getting DIE at symbol ref's offset");
        return NULL;
    }

    switch(read_dwarf_has_attr(rdc, die, attr_id, &has_attr, &form, &attr))
    {
    case DW_DLV_ERROR:
        read_dwarf_free_die(rdc, die);
        set_exception(PyExc_RuntimeError, rdc, "Error checking DIE attribute");
        return NULL;
    case DW_DLV_NO_ENTRY:
        read_dwarf_free_die(rdc, die);
        Py_INCREF(Py_None);
        return Py_None;
    default:
        break;
    }
    read_dwarf_free_die(rdc, die);

    if (!has_attr)
    {
        Py_INCREF(Py_None);
        return Py_None;
    }

    /* The attribute exists.  Now let's get its value */
    switch(form)
    {
    case DW_FORM_data1:
    case DW_FORM_data2:
    case DW_FORM_data4:
    case DW_FORM_data8:
    case DW_FORM_udata:
    {
        Dwarf_Unsigned value;
        if (read_dwarf_get_unsigned_attr(rdc, attr, &value) != DW_DLV_OK)
        {
            read_dwarf_free_attr(rdc, attr);
            set_exception(PyExc_RuntimeError, rdc, "Error getting DIE attribute");
            return NULL;
        }
        read_dwarf_free_attr(rdc, attr);
        return PyLong_FromLongLong(value);
    }
        break;
    case DW_FORM_sdata:
    {
        Dwarf_Signed value;
        if (read_dwarf_get_int_attr(rdc, attr, TRUE, &value) != DW_DLV_OK)
        {
            read_dwarf_free_attr(rdc, attr);
            set_exception(PyExc_RuntimeError, rdc, "Error getting DIE attribute");
            return NULL;
        }
        read_dwarf_free_attr(rdc, attr);
        return PyLong_FromLongLong(value);
    }
    case DW_FORM_addr:
    {
        Dwarf_Addr value;
        if (read_dwarf_get_addr_attr(rdc, attr,&value) != DW_DLV_OK)
        {
            read_dwarf_free_attr(rdc, attr);
            set_exception(PyExc_RuntimeError, rdc, "Error getting DIE attribute");
            return NULL;
        }
        read_dwarf_free_attr(rdc, attr);
        return PyLong_FromLongLong(value);
    }
    case DW_FORM_string:
    case DW_FORM_strp:
    {
        const char *value;
        if (read_dwarf_get_string_attr(rdc, attr, &value) != DW_DLV_OK)
        {
            read_dwarf_free_attr(rdc, attr);
            set_exception(PyExc_RuntimeError, rdc, "Error getting DIE attribute");
            return NULL;
        }
        read_dwarf_free_attr(rdc, attr);
        return PyString_FromString(value);
    }
        break;
    case DW_FORM_ref1:
    case DW_FORM_ref2:
    case DW_FORM_ref4:
    case DW_FORM_ref8:
    case DW_FORM_ref_udata:
    {
        Dwarf_Off offset;
        Dwarf_Symbol_Ref *symbol_ref;
        if (read_dwarf_get_ref_attr(rdc, attr, TRUE, &offset) != DW_DLV_OK)
        {
            read_dwarf_free_attr(rdc, attr);
            set_exception(PyExc_RuntimeError, rdc, "Error getting DIE reference attribute");
            return NULL;
        }
        /* Now construct the a SymbolRef object*/
        symbol_ref = create_symbol_ref(offset);
        if (symbol_ref == NULL)
        {
            set_exception(PyExc_RuntimeError, rdc, "Error getting DIE reference attribute");
        }
        read_dwarf_free_attr(rdc, attr);
        return (PyObject *)symbol_ref;
    }
    case DW_FORM_ref_addr:
    case DW_FORM_sec_offset:
    {
        Dwarf_Off offset;
        Dwarf_Symbol_Ref *symbol_ref;
        if (read_dwarf_get_gbl_ref_attr(rdc, attr, &offset) != DW_DLV_OK)
        {
            read_dwarf_free_attr(rdc, attr);
            set_exception(PyExc_RuntimeError, rdc, "Error getting DIE address reference attribute");
            return NULL;
        }
        /* Now construct the a SymbolRef object*/
        symbol_ref = create_symbol_ref(offset);
        if (symbol_ref == NULL)
        {
            set_exception(PyExc_RuntimeError, rdc, "Error getting DIE address reference attribute");
        }
        read_dwarf_free_attr(rdc, attr);
        return (PyObject *)symbol_ref;
    }
    default:
        read_dwarf_free_attr(rdc, attr);
        {
            char msg[64];
            sprintf(msg, "DIE attribute has unsupported form 0x%02x", form);
            set_exception(PyExc_RuntimeError, rdc, msg);
        }
        break;
    }
    read_dwarf_free_attr(rdc, attr);
    return NULL;
}



/**
 * Get the low program counter (address) of a function
 * @param self Dwarf_Reader
 * @param args A function symbol
 * @return long long containing the function's start address
 */
static PyObject *Dwarf_Reader_get_low_pc(Dwarf_Reader *self, PyObject *args)
{
    PyObject *func_sym_ref;
    Dwarf_Addr low_pc;
    Dwarf_Die die;

    if (PyArg_ParseTuple(args, "O", &func_sym_ref) < 0)
    {
        return NULL;
    }

    if (read_dwarf_offdie(self->rdc, ((Dwarf_Symbol_Ref *)func_sym_ref)->offset, &die) != DW_DLV_OK)
    {
        PyErr_SetString(PyExc_RuntimeError,
                                "Error getting DIE at symbol ref's offset");
        return NULL;
    }

    switch (read_dwarf_get_low_pc(self->rdc,
                                  die,
                                  &low_pc))
    {
    case DW_DLV_ERROR:
        read_dwarf_free_die(self->rdc, die);
        set_exception(PyExc_RuntimeError, self->rdc, "Error getting function low PC");
        return NULL;
    case DW_DLV_NO_ENTRY:
        read_dwarf_free_die(self->rdc, die);
        set_exception(PyExc_ValueError, self->rdc, "Function has no low PC attribute!");
        return NULL;
    default:
        break;
    }

    read_dwarf_free_die(self->rdc, die);
    return PyLong_FromLongLong(low_pc);
}

static PyObject *Dwarf_Reader_get_ranges(Dwarf_Reader *self, PyObject *args)
{
    PyObject *func_sym_ref, *low_pc_obj, *high_pc_obj, *ranges_obj,
    *pc_tuple, *ret_obj;
    Dwarf_Off die_off;
    int allow_absence = 0;

    if (PyArg_ParseTuple(args, "O|b", &func_sym_ref, &allow_absence) < 0)
    {
        return NULL;
    }

    die_off = ((Dwarf_Symbol_Ref *)func_sym_ref)->offset;

    /* We return a tuple of pairs.  If the function has low_pc and high_pc
     * attributes it's just a single-entry tuple containing this pair.  Otherwise
     * it's whatever the DW_AT_ranges attribute tells us. */
    low_pc_obj = get_attr(self->rdc, die_off, DW_AT_low_pc);
    high_pc_obj = get_attr(self->rdc, die_off, DW_AT_high_pc);
    if (low_pc_obj == NULL || high_pc_obj == NULL)
    {
        return NULL;
    }
    if (low_pc_obj != Py_None && high_pc_obj != Py_None)
    {
        pc_tuple = Py_BuildValue("(OO)", low_pc_obj, high_pc_obj);
        Py_DECREF(low_pc_obj);
        Py_DECREF(high_pc_obj);
        ret_obj = Py_BuildValue("[O]", pc_tuple);
        Py_DECREF(pc_tuple);
    }
    else if ((ranges_obj = get_attr(self->rdc, die_off, DW_AT_ranges)) != Py_None)
    {
        /* Decode the ranges attribute */
        Dwarf_Die die;
        Dwarf_Addr *ranges;
        unsigned num_ranges, i;

        if (ranges_obj == NULL)
        {
            return NULL;
        }
        if (read_dwarf_offdie(self->rdc, die_off, &die) != DW_DLV_OK)
        {
            char msg[128];
            sprintf(msg, "Error getting DIE at symbol ref's offset 0x%08lx", (unsigned long)die_off);
            PyErr_SetString(PyExc_RuntimeError, msg);
            return NULL;
        }
        switch (read_dwarf_get_ranges(self->rdc, die, &ranges, &num_ranges))
        {
        case DW_DLV_ERROR:
            read_dwarf_free_die(self->rdc, die);
            PyErr_SetString(PyExc_RuntimeError, "Error getting DIE ranges");
            return NULL;
        case DW_DLV_NO_ENTRY:
            read_dwarf_free_die(self->rdc, die);
            Py_INCREF(Py_None);
            return Py_None;
        default:
            read_dwarf_free_die(self->rdc, die);
            ret_obj = PyList_New(num_ranges);
            for (i = 0; i < num_ranges; ++i)
            {
                PyList_SET_ITEM(ret_obj, i, Py_BuildValue("KK", ranges[2*i], ranges[2*i+1]));
            }
            break;
        }
    }
    else
    {
        if (allow_absence)
        {
            Py_INCREF(Py_None);
            return Py_None;
        }
        PyErr_SetString(PyExc_RuntimeError, "Couldn't find either low_pc or ranges attributes!");
        return NULL;
    }
    return ret_obj;
}

/**
 * Get an attribute of a DIE as a suitable Python object: several attribute types
 * are supported
 * @param self Dwarf_Reader
 * @param args A symbol reference
 * @return Python object containing a suitable representation of the attribute
 */
static PyObject *Dwarf_Reader_get_attr(Dwarf_Reader *self, PyObject *args)
{
    PyObject *func_sym_ref;
    Dwarf_Half attr_id;

    if (PyArg_ParseTuple(args, "OH", &func_sym_ref, &attr_id) < 0)
    {
        return NULL;
    }
    return get_attr(self->rdc, ((Dwarf_Symbol_Ref *)func_sym_ref)->offset,
            attr_id);
}


static PyObject *get_reg_unwind_rule_entry(Dwarf_Signed is_offset,
                                           Dwarf_Signed offset,
                                           Dwarf_Signed regnum)
{
    if (is_offset)
    {
        PyObject *obj;
        obj = Py_BuildValue("{sLsL}", "offset", offset, "register", regnum);
        return obj;
    }

    /* Libdwarf uses two magic register numbers to indicate the
     * "same value" and "undefined" rules */
    switch(regnum)
    {
    case DW_FRAME_SAME_VAL:
        return Py_BuildValue("{si}", "same_value", 0);
    case DW_FRAME_UNDEFINED_VAL:
        return Py_BuildValue("{si}", "undefined", 0);
    default:
        return Py_BuildValue("{sL}", "register",
                                     regnum);
    }
}

/**
 * Get the requested unwind data out of the DWARF
 * @param self
 * @param args Expects a function symbol, an address offset and a list of
 * register numbers
 * @return
 */
static PyObject *Dwarf_Reader_get_unwind_data(Dwarf_Reader *self,
                                                                PyObject *args)
{
    PyObject *func_sym_ref;
    unsigned addr_offset;
    PyObject *unwind_dict;
    PyObject *reg_num_list;
    PyObject *reg_num_obj, *reg_rule_obj;
    Dwarf_Signed *reg_nums;
    Dwarf_Signed i, num_regs;
    Dwarf_Signed *reg_rules;
    Dwarf_Die die;
    int ret;

    if (PyArg_ParseTuple(args, "O|I|O", &func_sym_ref, &addr_offset,
                         &reg_num_list) < 0)
    {
        return NULL;
    }

    if (read_dwarf_offdie(self->rdc, ((Dwarf_Symbol_Ref *)func_sym_ref)->offset, &die) != DW_DLV_OK)
    {
        PyErr_SetString(PyExc_RuntimeError,
                                "Error getting DIE at symbol ref's offset");
        return NULL;
    }

    /* Construct a C array of target register nums */
    num_regs = (Dwarf_Signed)PyList_Size(reg_num_list);
    reg_nums = (Dwarf_Signed *)malloc((size_t)(num_regs * sizeof(Dwarf_Signed)));
    if (reg_nums == NULL)
    {
        set_exception(PyExc_RuntimeError, self->rdc,
                      "Couldn't allocate array for requested registers");
        return NULL;
    }
    for (i = 0; i < num_regs; ++i)
    {
        reg_num_obj = PyList_GetItem(reg_num_list, (Py_ssize_t)i);
        if (reg_num_obj == NULL)
        {
            free(reg_nums);
            return NULL;
        }
        reg_nums[i] = PyLong_AsLong(reg_num_obj);
    }

    /* Get the info out of the DWARF */
    ret = read_dwarf_unwind_data(self->rdc, die, addr_offset, reg_nums,
                                 num_regs, &reg_rules, TRUE);
    read_dwarf_free_die(self->rdc, die);
    switch(ret)
    {
    case DW_DLV_ERROR:
        set_exception(PyExc_RuntimeError, self->rdc,
                      "Error getting unwind data");
        free(reg_nums);
        return NULL;
    case DW_DLV_NO_ENTRY:
        /* Return an empty dictionary to indicate that there's no information
         * to be had */
        free(reg_nums);
        return PyDict_New();
    case DW_DLV_OK:
        break;
    }

    /* Shove it into a dictionary to return */
    unwind_dict = PyDict_New();

    if (unwind_dict == NULL)
    {
        set_exception(PyExc_RuntimeError, self->rdc,
                      "Failed to allocate empty dictionary!");
        free(reg_nums);
        free(reg_rules);
        return NULL;
    }

    for (i = 0; i < num_regs; ++i)
    {
        reg_num_obj = Py_BuildValue("i", (int)reg_nums[i]);
        reg_rule_obj = get_reg_unwind_rule_entry(reg_rules[3*i],
                                                 reg_rules[3*i+1],
                                                 reg_rules[3*i+2]);
        if (PyDict_SetItem(unwind_dict, reg_num_obj, reg_rule_obj) < 0)
        {
            Py_DECREF(reg_num_obj);
            Py_DECREF(reg_rule_obj);
            Py_DECREF(unwind_dict);
            free(reg_nums);
            free(reg_rules);
            return NULL;
        }
        Py_DECREF(reg_num_obj);
        Py_DECREF(reg_rule_obj);
    }

    /* Now add the return address rule */
    reg_rule_obj = get_reg_unwind_rule_entry(reg_rules[3*num_regs],
                                             reg_rules[3*num_regs + 1],
                                             reg_rules[3*num_regs + 2]);
    if (PyDict_SetItemString(unwind_dict, "return_address", reg_rule_obj) < 0)
    {
        Py_DECREF(reg_rule_obj);
        Py_DECREF(unwind_dict);
        free(reg_nums);
        free(reg_rules);
        return NULL;
    }
    Py_DECREF(reg_rule_obj);
    free(reg_nums);
    free(reg_rules);
    return unwind_dict;
}

/**
 * Get the source file and line number of a given address offset within a function
 * @param self Dwarf_Reader context variable
 * @param args The function symbol reference and the offset (defaults to 0)
 * @return (filename, line number) tuple if source info found, else None.
 * Raises RuntimeError if something goes wrong.
 */
static PyObject *Dwarf_Reader_get_srcfile_and_lineno(Dwarf_Reader *self,
                                                     PyObject *args)
{
    unsigned addr_offset = 0;
    PyObject *func_sym_ref;
    char *source_file;
    Dwarf_Unsigned line_no;
    Dwarf_Die die;
    PyObject *ret;

    /* The caller supplies a program address */
    if (PyArg_ParseTuple(args, "O|I", &func_sym_ref, &addr_offset) < 0)
    {
        return NULL;
    }

    if (read_dwarf_offdie(self->rdc, ((Dwarf_Symbol_Ref *)func_sym_ref)->offset,
                                                &die) != DW_DLV_OK)
    {
        PyErr_SetString(PyExc_RuntimeError,
                                "Error getting DIE at symbol ref's offset");
        return NULL;
    }

    /* We look up the corresponding source file and line number in the DWARF */
    source_file = read_dwarf_get_source_file_and_line(self->rdc, die,
                                      addr_offset,
                                      &line_no);

    read_dwarf_free_die(self->rdc, die);
    if (source_file == NULL)
    {
        if (line_no)
        {
            /* The look-up didn't find any information for that function */
            Py_INCREF(Py_None);
            return Py_None;
        }
        else
        {
            /* Something went wrong during the look-up */
            set_exception(PyExc_RuntimeError, self->rdc,
                          "Error getting source information");
            return NULL;
        }
    }

    /* ...and return a (filename, line number) tuple */
    ret = Py_BuildValue("si", source_file, line_no);

    /* ...not forgetting to free the memory libdwarf returned the source file
     * name in */
    read_dwarf_free_srcfile(self->rdc, source_file);

    return ret;
}

/**
 * Look up the formal parameters of the given function and return references to
 * them in a list of (name, symbol) tuples (in order to preserve the order)
 * @param self Dwarf_Reader symbol
 * @param args Function symbol ref
 * @return List of (name, symbol) tuplies for the formal parameters of the function
 */
static PyObject *Dwarf_Reader_get_formal_params(Dwarf_Reader *self,
                                                PyObject *args)
{
    PyObject *func_sym_ref;
    PyObject *ret_obj;
    Dwarf_Die die;

    /* The caller supplies a function symbol */
    if (PyArg_ParseTuple(args, "O", &func_sym_ref) < 0)
    {
        return NULL;
    }

    if (read_dwarf_offdie(self->rdc, ((Dwarf_Symbol_Ref *)func_sym_ref)->offset,
                                                &die) != DW_DLV_OK)
    {
        PyErr_SetString(PyExc_RuntimeError,
                                "Error getting DIE at symbol ref's offset");
        return NULL;
    }

    ret_obj = get_formal_param_list(self->rdc, die);
    read_dwarf_free_die(self->rdc, die);
    return ret_obj;
}


/**
 * Look up the local variables of the given function and return references to
 * them in a list of (name, symbol) tuples (in order to preserve the order)
 * @param self Dwarf_Reader symbol
 * @param args Function symbol ref
 * @return List of (name, symbol) tuplies for the local variables of the function
 */
static PyObject *Dwarf_Reader_get_local_variables(Dwarf_Reader *self,
                                                  PyObject *args)
{
    PyObject *func_sym_ref;
    PyObject *ret_obj;
    Dwarf_Die die;

    /* The caller supplies a function symbol */
    if (PyArg_ParseTuple(args, "O", &func_sym_ref) < 0)
    {
        return NULL;
    }

    if (read_dwarf_offdie(self->rdc, ((Dwarf_Symbol_Ref *)func_sym_ref)->offset,
                                                            &die) != DW_DLV_OK)
    {
        PyErr_SetString(PyExc_RuntimeError,
                                "Error getting DIE at symbol ref's offset");
        return NULL;
    }

    ret_obj = get_local_var_list(self->rdc, die);
    read_dwarf_free_die(self->rdc, die);
    return ret_obj;
}

/**
 * Look up the return type of the given function and return references to
 * them in a list of (name, symbol) tuples (in order to preserve the order)
 * @param self Dwarf_Reader symbol
 * @param args Function symbol ref
 * @return List of (name, symbol) tuplies for the local variables of the function
 */
static PyObject *Dwarf_Reader_get_return_type(Dwarf_Reader *self,
                                              PyObject *args)
{
    PyObject *func_sym_ref;
    Dwarf_Die return_type_die;
    Dwarf_Symbol_Ref *symbol_ref;
    Dwarf_Off return_type_offset;
    Dwarf_Die die;

    /* The caller supplies a function symbol */
    if (PyArg_ParseTuple(args, "O", &func_sym_ref) < 0)
    {
        return NULL;
    }

    if (read_dwarf_offdie(self->rdc, ((Dwarf_Symbol_Ref *)func_sym_ref)->offset,
                                                            &die) != DW_DLV_OK)
    {
        PyErr_SetString(PyExc_RuntimeError,
                                "Error getting DIE at symbol ref's offset");
        return NULL;
    }

    /* Get the return type DIE from the DWARF if there is one */
    switch (read_dwarf_get_immediate_type(self->rdc, die, &return_type_die))
    {
    case DW_DLV_OK:
        break;
    case DW_DLV_NO_ENTRY:
        read_dwarf_free_die(self->rdc, die);
        Py_INCREF(Py_None);
        return Py_None;
    case DW_DLV_ERROR:
        read_dwarf_free_die(self->rdc, die);
        set_exception(PyExc_RuntimeError, self->rdc,
                      "Failed to get return type of function");
        return NULL;
    }
    read_dwarf_free_die(self->rdc, die);

    return_type_offset = read_dwarf_die_offset(self->rdc, return_type_die);
    read_dwarf_free_die(self->rdc, return_type_die);

    symbol_ref = create_symbol_ref(return_type_offset);
    if (symbol_ref == NULL){
        PyErr_SetString(PyExc_RuntimeError,
                        "Error initialising Dwarf_Symbol_Ref");
        return NULL;
    }

    return (PyObject *)symbol_ref;
}

static PyObject *Dwarf_Reader_get_inline_calls(Dwarf_Reader *self,
                                                PyObject *args)
{
    PyObject *func_sym_ref;
    PyObject *ret_obj;
    Dwarf_Die die;

    /* The caller supplies a function symbol */
    if (PyArg_ParseTuple(args, "O", &func_sym_ref) < 0)
    {
        return NULL;
    }

    if (read_dwarf_offdie(self->rdc, ((Dwarf_Symbol_Ref *)func_sym_ref)->offset,
                                                &die) != DW_DLV_OK)
    {
        PyErr_SetString(PyExc_RuntimeError,
                                "Error getting DIE at symbol ref's offset");
        return NULL;
    }

    ret_obj = get_inline_call_list(self->rdc, die);
    read_dwarf_free_die(self->rdc, die);
    return ret_obj;
}


/**
 * Get the location rule for a local at the given program counter
 * @param self Dwarf_Reader context
 * @param args The symbol ref, the program counter and whether the instruction
 * at this PC is being executed (True) or still to be executed (False)
 * @return A list of tuples indicating the
 */
static PyObject *Dwarf_Reader_get_local_loc(Dwarf_Reader *self, PyObject *args)
{
    PyObject *var_sym_ref;
    Dwarf_Addr pc;
    Dwarf_Small *op_list;
    Dwarf_Unsigned *arg_list;
    unsigned num_ops, i;

    Dwarf_Die die;
    PyObject *loc_list;
    unsigned char executing;

    /* We use "executing" to indicate that the instruction the PC is pointing to
     * is actually *being* executed, not *about to be* executed. If this is the
     * case then rules which are only valid up to completion of the last
     * instruction before this one will not be valid, even though judging
     * purely by the supplied PC we are still (just) in the valid range. In
     * other words, this flag is true for all frames except the innermost of
     * interrupt stack */
    if (PyArg_ParseTuple(args, "O|K|B", &var_sym_ref, &pc, &executing) < 0)
    {
        return NULL;
    }

    if (read_dwarf_offdie(self->rdc, ((Dwarf_Symbol_Ref *)var_sym_ref)->offset,
                                                            &die) != DW_DLV_OK)
    {
        PyErr_SetString(PyExc_RuntimeError,
                                "Error getting DIE at symbol ref's offset");
        return NULL;
    }
    /* Pull out the full set of information about where the value is at all
     * valid PCs */
    switch (read_dwarf_get_loc_list(self->rdc, die, pc, executing,
                                    &op_list, &arg_list, &num_ops))
    {
    case DW_DLV_NO_ENTRY:
        read_dwarf_free_die(self->rdc, die);
        Py_INCREF(Py_None);
        return Py_None;
    case DW_DLV_ERROR:
        read_dwarf_free_die(self->rdc, die);
        set_exception(PyExc_RuntimeError, self->rdc, "Error getting location info");
        return NULL;
    case DW_DLV_OK:
        break;
    }
    read_dwarf_free_die(self->rdc, die);

    if (num_ops > 0)
    {
        loc_list = PyList_New(num_ops);
        for (i = 0; i < num_ops; ++i)
        {
            PyList_SET_ITEM(loc_list, i,
                            Py_BuildValue("bkkk", op_list[i],
                                          (unsigned long)arg_list[3*i],
                                          (unsigned long)arg_list[3*i+1],
                                          (unsigned long)arg_list[3*i+2]));
        }
        free(op_list);
        free(arg_list);
        return loc_list;
    }

    Py_INCREF(Py_None);
    return Py_None;
}

static void Dwarf_Reader_dealloc(Dwarf_Reader *self)
{
    read_dwarf_finish(self->rdc);
    self->ob_type->tp_free((PyObject *)self);
}


static PyMethodDef Dwarf_Reader_methods[] = {
    {"get_cu_ref_dict", (PyCFunction)Dwarf_Reader_get_cu_ref_dict, METH_VARARGS,
        "Get a dictionary containing Dwarf_Symbol_Refs for all the CU"
            "entries indexed by name"},
    {"get_tag_dict", (PyCFunction)Dwarf_Reader_get_tag_dict, METH_VARARGS,
       "Get a dictionary containing Dwarf_Symbol_Refs for all the symbols with "
            "the indicated tag"},
    {"get_type_dict", (PyCFunction)Dwarf_Reader_get_type_dict, METH_VARARGS,
      "Get a dictionary containing Dwarf_Symbol_Refs for all the symbols that "
            "have a type tag"},
    {"get_func_dict", (PyCFunction)Dwarf_Reader_get_func_dict, METH_VARARGS,
       "Get a dictionary containing Dwarf_Symbol_Refs for all the functions that "
                    "are not inlined"},
    {"get_var_struct_dict", (PyCFunction)Dwarf_Reader_get_var_struct_dict,
            METH_VARARGS,
      "Return a dictionary containing details of the variable's structure "
            "type"},
    {"get_type_struct_dict", (PyCFunction)Dwarf_Reader_get_type_struct_dict,
            METH_VARARGS,
        "Return a dictionary containing details of a structure type"},
    {"symbol_is_external", (PyCFunction)Dwarf_Reader_symbol_is_external,
            METH_VARARGS,
        "Return bool indicating whether the supplied symbol has the external "
            "attribute"},
    {"get_func_addr", (PyCFunction)Dwarf_Reader_get_low_pc, METH_VARARGS,
        "Return the start address (low PC) of the given function"},
    {"get_ranges", (PyCFunction)Dwarf_Reader_get_ranges, METH_VARARGS,
            "Return the address ranges of the given function or CU"},
    {"get_attr", (PyCFunction)Dwarf_Reader_get_attr, METH_VARARGS,
        "Return the the requested attribute of a DIE"},
    {"get_unwind_data", (PyCFunction)Dwarf_Reader_get_unwind_data, METH_VARARGS,
        "Return a dictionary containing the information required to unwind a "
        "given function"},
    {"get_srcfile_and_lineno", (PyCFunction)Dwarf_Reader_get_srcfile_and_lineno,
            METH_VARARGS,
        "Return a tuple containing the filename and the line number"},
    {"get_formal_params", (PyCFunction)Dwarf_Reader_get_formal_params,
            METH_VARARGS,
        "Return a list containing the formal parameter symbols"},
    {"get_local_variables", (PyCFunction)Dwarf_Reader_get_local_variables,
            METH_VARARGS,
        "Return a list containing the local variable symbols"},
    {"get_inline_calls", (PyCFunction)Dwarf_Reader_get_inline_calls,
            METH_VARARGS,
        "Return a list containing the inline subroutine calls from the function"
    },
    {"get_return_type", (PyCFunction)Dwarf_Reader_get_return_type,
            METH_VARARGS,
        "Return the return type"},
    {"get_local_loc", (PyCFunction)Dwarf_Reader_get_local_loc, METH_VARARGS,
        "Return the address of the local on the stack, or a register name"},
    {"print_mem_stats", (PyCFunction)Dwarf_Reader_print_mem_stats, METH_VARARGS,
        "Print memory stats from libdwarf"},
    {"set_verbosity", (PyCFunction)Dwarf_Reader_set_verbosity, METH_VARARGS,
        "Set the verbosity from 0 to 3"},
      {NULL}        /* Sentinel */
};


static int Elf_Reader_init(Elf_Reader *self, PyObject *args)
{
    char *elf_filename;
    Elf32_Ehdr *ehdr;
    Elf_Scn *sec;
    Elf32_Shdr *shdr;

    if (! PyArg_ParseTuple(args, "s", &elf_filename))
    {
        return -1;
    }

    /* Elf version check is mandatory */
    if (elf_version(EV_CURRENT) == EV_NONE)
      {
        printf("Version mismatch!\n");
        return -1;
      }

    /* Open the supplied Elf file */
    self->fd = open(elf_filename,O_RDONLY | O_BINARY);
    if (self->fd < 0)
    {
        printf("Opening '%s' failed!\n", elf_filename);
        printf("Error opening file: %s\n", strerror( errno ) );
        PyErr_SetString(PyExc_RuntimeError, "File open failed!");
        return -1;
    }

    /* Initialise the ELF session */
    self->elf = elf_begin(self->fd, ELF_C_READ, NULL);

    if (self->elf == NULL)
    {
        printf("%s\n", elf_errmsg(elf_errno()));
        PyErr_SetString(PyExc_RuntimeError, "elf_begin failed");
        return -1;
    }

    if (elf_kind(self->elf) != ELF_K_ELF)
    {
        printf("%s is not an ELF file\n", elf_filename);
        PyErr_SetString(PyExc_RuntimeError, "Not an ELF");
        return -1;
    }

    if ((ehdr = elf32_getehdr(self->elf)) == NULL)
    {
        printf("%s\n", elf_errmsg(elf_errno()));
        PyErr_SetString(PyExc_RuntimeError, "Failed to get elf header!");
        return -1;
    }

    self->e_machine = ehdr->e_machine;

    /* Now loop over the string table and find the debug strings section */
    sec = NULL;
    self->dbg_str_sec = NULL;
    while ((sec = elf_nextscn(self->elf, sec)) != NULL)
    {
        char *name;
        if ((shdr = elf32_getshdr(sec)) == 0)
        {
            PyErr_SetString(PyExc_RuntimeError, "getshdr failed!\n");
            return -1;
        }

        if ((name = elf_strptr(self->elf, ehdr->e_shstrndx, shdr->sh_name)) == NULL)
        {
            PyErr_SetString(PyExc_RuntimeError, "get section name failed\n");
            return -1;
        }

        if (strcmp(name, "debug_strings") == 0)
        {
            self->dbg_str_sec = sec;
        }
    }

    return 0;
}

static void Elf_Reader_dealloc(Elf_Reader *self)
{
    (void)elf_end(self->elf);
    close(self->fd);
}

#define DECREF_DICTS() \
    do {\
        Py_DECREF(gbl_vars);\
        Py_DECREF(funcs);\
        Py_DECREF(cus);\
        Py_DECREF(abs_syms);\
        Py_DECREF(minim_ranges);\
    } while (0)


static int insert_sym_by_name(Elf *elf, const Elf32_Shdr *shdr, Elf32_Word name_ind,
                              PyObject *dict, PyObject *value)
{
    char *name;
    if ((name = elf_strptr(elf, shdr->sh_link, name_ind)) == NULL ||
        PyDict_SetItemString(dict, name, value) < 0)
    {
        PyErr_SetString(PyExc_RuntimeError,
                "get or insert symbol name failed!");
        Py_DECREF(value);
        return -1;
    }
    Py_DECREF(value);
    return 0;

}

static int leading_name_match(Elf *elf, const Elf32_Shdr *shdr,
                              Elf32_Word st_name, const char **filter_out_list,
                              int n_filter_out)
{
    char *name = elf_strptr(elf, shdr->sh_link, st_name);
    int matched = 0, i;

    if (name != NULL)
    {
        for (i = 0; i < n_filter_out && !matched; i++)
        {
            if (memcmp(name, filter_out_list[i], strlen(filter_out_list[i])) == 0)
            {
                matched = 1;
            }
        }
    }
    return matched;
}

/**
 * Add the given symbol to the appropriate dictionary (globals or current cu
 * locals) if:
 *  a) its name doesn't match the relevant architecture's filter-out list
 *  b) its not a local outside any compilation unit
 *
 * @param self Elf_Reader context
 * @param shdr Symbol's section header
 * @param sym The symbol itself
 * @param funcs Global function dict
 * @param current_cu_funcs Current CU's local function dict
 * @return TRUE if the symbol was successfully processed (added or discarded),
 * FALSE if an error occurred.
 */
static int add_func_symbol(Elf_Reader *self, const Elf32_Shdr *shdr,
                            const GElf_Sym *sym, PyObject *funcs,
                            PyObject *current_cu_funcs)
{
    int discard_sym;
    static const char *kcc_func_filter[] = {".Lg", "Lc_"};
    static const char *arm_func_filter[] = {"__"};

    /* Filter out symbols in specific ELF files that aren't really
     * functions - e.g. debugging symbols and branch labels */
    switch (self->e_machine)
    {
    case EM_KALIMBA:
        discard_sym = leading_name_match(self->elf, shdr,
                                          sym->st_name,
                                          kcc_func_filter,
                                          sizeof(kcc_func_filter)/
                                          sizeof(kcc_func_filter[0]));
        break;
    case EM_ARM:
        discard_sym = leading_name_match(self->elf, shdr,
                                          sym->st_name,
                                          arm_func_filter,
                                          sizeof(arm_func_filter)/
                                          sizeof(arm_func_filter[0]));
        break;
    default:
        discard_sym = FALSE;
    }

    if (! discard_sym)
    {
        /* Add the function either to the globals or to the
         * compilation unit if it's a local, or it's a local outside a compilation
         * unit, ignore it */
        PyObject *dest_dict =
                ELF32_ST_BIND(sym->st_info) == STB_GLOBAL ?
                                   funcs : current_cu_funcs;
        if (dest_dict != NULL)
        {
            assert(sym->st_value <= (unsigned long)(-1));
            if (insert_sym_by_name(self->elf, shdr, sym->st_name,
                                   dest_dict,
                                   Py_BuildValue("k",
                                     (unsigned long)(sym->st_value)))
                    < 0)
            {
                return FALSE;
            }
        }
    }
    return TRUE;
}

static PyObject *add_new_cu(PyObject *cus, const char *cu_name)
{
    PyObject *current_cu = NULL;
    char *new_cu_name = NULL;

    /* Make sure we're adding this under a unique name */
    if ((current_cu = PyDict_GetItemString(cus, cu_name)) != NULL)
    {
        /* Allocate space for a suffix "#<N>" where <N> is a decimal
         * number up to two digits long.*/
        new_cu_name = malloc(strlen(cu_name) + 4);
        if (new_cu_name == NULL)
        {
            return NULL;
        }
        /* keep incrementing the suffix until it's a new name */
        {
            int i = 1;
            do {
                if (i > 99)
                {
                    free(new_cu_name);
                    return NULL;
                }
                sprintf(new_cu_name, "%s#%d", cu_name, i);
                i++;
            } while((current_cu = PyDict_GetItemString(cus,
                    new_cu_name)) != NULL);
        }
    }
    else
    {
        new_cu_name = NULL;
    }
    current_cu = PyDict_New();
    if (current_cu == NULL)
    {
        free(new_cu_name);
        return NULL;
    }
    /* File symbols don't appear to have a meaningful value of
     * their own so we simply store an empty dictionary against
     * the CU's name (i.e. the filename) into which we will
     * insert the functions and variables listed as part of this
     * CU.
     */
    if (new_cu_name == NULL)
    {
        if (PyDict_SetItemString(cus, cu_name, current_cu) < 0)
        {
            Py_DECREF(current_cu);
            return NULL;
        }
    }
    else
    {
        if (PyDict_SetItemString(cus, new_cu_name, current_cu) < 0)
        {
            Py_DECREF(current_cu);
            free(new_cu_name);
        }
        free(new_cu_name);
    }
    return current_cu;
}


static PyObject *Elf_Reader_get_symbols(Elf_Reader *self)
{
    /* Walk through the symbol table inserting interesting symbols into
     * dictionaries */

    const char *xap_abs_filter[] = {"__size_of_"};

    PyObject *minim_ranges = NULL;
    PyObject *gbl_vars = NULL;
    PyObject *funcs = NULL;
    PyObject *cus = NULL;
    PyObject *abs_syms = NULL;
    PyObject *current_cu = NULL;
    PyObject *current_cu_vars =  NULL;
    PyObject *current_cu_funcs = NULL;
    PyObject *return_tuple = NULL;
    char *cu_name;

    Elf_Scn *sec = NULL;
    Elf_Data *data = NULL;
    Elf32_Shdr *shdr = NULL;
    Elf32_Word n_syms, i;
    GElf_Sym sym;
    size_t debug_strings_index = self->dbg_str_sec != NULL ?
                                            elf_ndxscn(self->dbg_str_sec) : 0;
    int discard_sym;

    /* Set up dictionaries */
    minim_ranges = PyList_New(0);
    gbl_vars = PyDict_New();
    funcs = PyDict_New();
    cus = PyDict_New();
    abs_syms = PyDict_New();
    if (minim_ranges == NULL || gbl_vars == NULL || funcs == NULL ||
            cus == NULL || abs_syms == NULL)
    {
        Py_XDECREF(minim_ranges);
        Py_XDECREF(gbl_vars);
        Py_XDECREF(funcs);
        Py_XDECREF(cus);
        Py_XDECREF(abs_syms);
        PyErr_SetString(PyExc_RuntimeError, "Dictionary creation failed");
    }



    /* Loop through the symbol table pushing name/value mappings into the
     * appropriate dictionaries based on the symbols' type and scope
     */
    sec = NULL;
    while ((sec = elf_nextscn(self->elf, sec)) != NULL)
    {
        if ((shdr = elf32_getshdr(sec)) == 0)
        {
            PyErr_SetString(PyExc_RuntimeError, "getshdr failed!");
            return NULL;
        }

        if (self->e_machine == EM_KALIMBA && (shdr->sh_flags & SHF_EXECINSTR) &&
                            (shdr->sh_flags & SHF_KAL_MINIM))
        {
            /* We want to know the ranges of instruction addresses that are MINIM */
            unsigned long start_address;
            unsigned long end_address;
            PyObject *tuple;
            assert(shdr->sh_addr+shdr->sh_size <= (unsigned long)-1);
            start_address = (unsigned long)shdr->sh_addr;
            end_address = (unsigned long)(shdr->sh_addr + shdr->sh_size);

            /* "k" is Python's way of saying "unsigned long".  Obviously. */
            if ((tuple = Py_BuildValue("kk", start_address, end_address))
                                                                    == NULL ||
                PyList_Append(minim_ranges, tuple) < 0)
            {
                Py_XDECREF(tuple);
                DECREF_DICTS();
                PyErr_SetString(PyExc_RuntimeError,
                                    "Couldn't add entry to minim_ranges list");
                return NULL;
            }
            Py_DECREF(tuple);
        }

        if (shdr->sh_type == SHT_SYMTAB)
        {
            /* Search for:
             *  - global variables (globvars) - Store value and size
             *  - compilation units (cus) - Store a dictionary into which the
             *  following local symbols can be inserted
             *  - functions (funcs) - Store symbol's value only (doesn't attempt
             *  to distinguish globals and statics)
             *  - absolutes (abs) - Store symbol's value only
             */
            if ((data = elf_getdata(sec, data)) == NULL)
            {
                DECREF_DICTS();
                PyErr_SetString(PyExc_RuntimeError, "get symbol table data "
                                                        "descriptor failed!");
                return NULL;
            }

            /* Each symbol takes up a fixed-size block so easy to calculate
             * how many there are. */
            n_syms = shdr->sh_size / shdr->sh_entsize;

            for (i = 0; i < n_syms; ++i)
            {
                if (gelf_getsym(data, i, &sym) == NULL)
                {
                    DECREF_DICTS();
                    PyErr_SetString(PyExc_RuntimeError, "get symbol failed!");
                    return NULL;
                }

                /* A global marks the end of a CU's symbols, so record that
                 * we're no long accumulating the contents of a CU */
                if (ELF32_ST_BIND(sym.st_info) == STB_GLOBAL)
                {
                    current_cu = NULL;
                    current_cu_vars = NULL;
                    current_cu_funcs = NULL;
                }

                switch(ELF32_ST_TYPE(sym.st_info))
                {
                case STT_FUNC:
                    /* The symbol has type "function" */
                    if (!add_func_symbol(self, shdr, &sym, funcs,
                                                            current_cu_funcs))
                    {
                        DECREF_DICTS();
                        return NULL;
                    }
                    break;
                /* KCC doesn't give variable symbols a type very often so we
                 * assume that symbols with no type are variables.  But that's
                 * not such a good plan with XAP ELFs. */
                case STT_NOTYPE:
                /* First figure out if this is really a variable or not */
                {
                    Elf_Scn *sym_sec;
                    Elf32_Shdr *sym_shdr;
                    /* If it's in a special section it's not a variable */
                    if (sym.st_shndx >= 0xfff0)
                    {
                        break;
                    }
                    /* Look up the symbol's section header */
                    if ((sym_sec = elf_getscn(self->elf, sym.st_shndx)) == NULL ||
                        (sym_shdr = elf32_getshdr(sym_sec)) == NULL)
                    {
                        DECREF_DICTS();
                        PyErr_SetString(PyExc_RuntimeError,
                                    "Couldn't get symbol's section header!");
                        return NULL;
                    }
                    /* If the section takes no space at runtime we're not
                     * interested in it */
                    if (!(sym_shdr->sh_flags & SHF_ALLOC))
                    {
                        break;
                    }
                    /* If the section is executable then presumably the
                     * symbol is a function */
                    else if (sym_shdr->sh_flags & SHF_EXECINSTR)
                    {
                        if (!add_func_symbol(self, shdr, &sym, funcs,
                                                            current_cu_funcs))
                        {
                            DECREF_DICTS();
                            return NULL;
                        }
                    }
                    else
                    {
                        /* We'll assume it's a variable */
                        /* Put value and size into a tuple and insert either into
                         * the globals or into the current compilation unit */
                        /* Store address and size, although size is usually 0 */
                        PyObject *var_tuple = Py_BuildValue("II", sym.st_value,
                                                                   sym.st_size);
                        /* Select the right dictionary to store the symbol in */
                        PyObject *dest_dict =
                                ELF32_ST_BIND(sym.st_info) == STB_GLOBAL ?
                                                       gbl_vars : current_cu_vars;
                        if (dest_dict != NULL)
                        {
                            if (insert_sym_by_name(self->elf, shdr, sym.st_name,
                                                      dest_dict,var_tuple) < 0)
                            {
                                DECREF_DICTS();
                                return NULL;
                            }
                        }
                        /* Otherwise the symbol is a local but there's no current
                         * compilation unit.  Just ignore these. */
                    }
                    break;
                }
                case STT_OBJECT:
                {
                    Elf_Scn *sym_sec;
                    Elf32_Shdr *sym_shdr;
                    /* If it's in a special section it's not a variable */
                    if (sym.st_shndx >= 0xfff0)
                    {
                        break;
                    }
                    /* Look up the symbol's section header */
                    if ((sym_sec = elf_getscn(self->elf, sym.st_shndx)) == NULL ||
                        (sym_shdr = elf32_getshdr(sym_sec)) == NULL)
                    {
                        DECREF_DICTS();
                        PyErr_SetString(PyExc_RuntimeError,
                                    "Couldn't get symbol's section header!");
                        return NULL;
                    }
                    /* If the section takes no space at runtime we're not
                     * interested in it */
                    if (!(sym_shdr->sh_flags & SHF_ALLOC))
                    {
                        break;
                    }
                    /* XAP ELFs have debug strings marked as variables */
                    if (self->e_machine != EM_XAP || sym.st_shndx != debug_strings_index)
                    {
                        /* Put value and size into a tuple and insert either into
                         * the globals or into the current compilation unit */
                        /* Store address and size, although size is usually 0 */
                        PyObject *var_tuple = Py_BuildValue("II", sym.st_value,
                                                                   sym.st_size);
                        /* Select the right dictionary to store the symbol in */
                        PyObject *dest_dict =
                                ELF32_ST_BIND(sym.st_info) == STB_GLOBAL ?
                                                       gbl_vars : current_cu_vars;
                        if (dest_dict != NULL)
                        {
                            if (insert_sym_by_name(self->elf, shdr, sym.st_name,
                                                      dest_dict,var_tuple) < 0)
                            {
                                DECREF_DICTS();
                                return NULL;
                            }
                        }
                        /* Otherwise the symbol is a local but there's no current
                         * compilation unit.  Just ignore these. */
                    }
                    break;
                }
                case STT_FILE:
                    /* The symbol is marked as a file, which indicates a
                     * compilation unit. */

                    /* Insert a new CU dictionary indexed by the symbol's name,
                     * (unless there's already a CU of exactly the same name,
                     * in which case we add to it, because there's no other
                     * (sane) way to avoid losing symbols) */

                    cu_name = elf_strptr(self->elf, shdr->sh_link, sym.st_name);
                    if (cu_name == NULL)
                    {
                        DECREF_DICTS();
                        return NULL;
                    }
                    current_cu = add_new_cu(cus, cu_name);
                    if (current_cu == NULL)
                    {
                        DECREF_DICTS();
                        return NULL;
                    }
                    /* Add subdictionaries for storing the local variables and
                     * functions (statics) in this CU */
                    if ((current_cu_vars = PyDict_New()) == NULL ||
                            PyDict_SetItemString(current_cu, "vars",
                                                        current_cu_vars) < 0)
                    {
                        DECREF_DICTS();
                        Py_XDECREF(current_cu_vars);
                    }
                    Py_DECREF(current_cu_vars);
                    if ((current_cu_funcs = PyDict_New()) == NULL
                            || PyDict_SetItemString(current_cu, "funcs",
                                                        current_cu_funcs) < 0)
                    {
                        DECREF_DICTS();
                        Py_XDECREF(current_cu_funcs);
                    }
                    Py_DECREF(current_cu_funcs);
                    break;

                default:
                    break;
                }

                /* Gather global abs symbols (all abs symbols for XAP) */
                if ((self->e_machine == EM_XAP || ELF32_ST_BIND(sym.st_info) == STB_GLOBAL)
                    && sym.st_shndx == SHN_ABS && ELF32_ST_TYPE(sym.st_info) != STT_FILE)
                {
                    assert(sym.st_value <= (unsigned long)(-1));
                    switch(self->e_machine)
                    {
                    case EM_XAP:
                        discard_sym = leading_name_match(self->elf, shdr, sym.st_name,
                                xap_abs_filter,
                                sizeof(xap_abs_filter)/sizeof(xap_abs_filter[0]));
                        break;
                    default:
                        discard_sym = FALSE;
                    }
                    if (!discard_sym)
                    {
                        if (insert_sym_by_name(self->elf, shdr, sym.st_name,
                                               abs_syms,
                                Py_BuildValue("k",(unsigned long)(sym.st_value))) < 0)
                        {
                            DECREF_DICTS();
                            return NULL;
                        }
                    }
                }
            }
        }
    }

    /* Insert all the dictionaries into a tuple and return it to the caller */
    if ((return_tuple = Py_BuildValue("OOOOO", gbl_vars, funcs,
                                        cus, abs_syms, minim_ranges)) == NULL)
    {
        DECREF_DICTS();
        return NULL;
    }
    DECREF_DICTS();
    return return_tuple;
}

static PyObject *Elf_Reader_get_debug_strings(Elf_Reader *self, PyObject *args)
{
    PyObject *debug_strings;
    Elf32_Shdr *dbg_str_sec_hdr;
    Elf32_Addr dbg_str_sec_addr;
    Elf_Data *data = NULL;
    PyObject *is_le_obj;
    int char_width, is_littleendian;

    if (self->dbg_str_sec == NULL)
    {
        /* There's no debug string section: that's not necessarily an error,
         * so just return an empty dictionary */
        return PyDict_New();
    }

    if (PyArg_ParseTuple(args, "iO", &char_width, &is_le_obj) < 0)
    {
       return NULL;
    }
    is_littleendian = PyObject_IsTrue(is_le_obj);

    dbg_str_sec_hdr = elf32_getshdr(self->dbg_str_sec);
    dbg_str_sec_addr = dbg_str_sec_hdr->sh_addr;

    debug_strings = PyDict_New();

    while ((data = elf_getdata(self->dbg_str_sec, data)) != NULL)
    {
        const char *dbg_str;
        size_t cur_str_offset = 0, len_unpacked = 0;

        /* First character, taking endianness offset into account */
        dbg_str = (const char *)data->d_buf + (is_littleendian ? 0 : char_width - 1);

        while (cur_str_offset + len_unpacked < data->d_size)
        {
            if (*(dbg_str + cur_str_offset + len_unpacked) != 0)
            {
                len_unpacked += char_width;
            }
            /* Null terminator: this is the end of the string */
            else
            {
                size_t addr;
                PyObject *string_obj = NULL, *addr_obj = NULL;
                /* How much space the string will take up when packed one char
                 * per byte */
                const size_t len_packed = len_unpacked / char_width;
                /* Space for the packed string to be written into */
                char *packed_str = (char *)malloc(len_packed);
                size_t i;

                if (packed_str == NULL)
                {
                    Py_DECREF(debug_strings);
                    PyErr_SetString(PyExc_RuntimeError, "Couldn't allocate string buffer!");
                }
                /* Copy */
                for (i = 0; i < len_packed; ++i)
                {
                    packed_str[i] = *(dbg_str + cur_str_offset + char_width*i);
                }
                /* The address is the section's virtual address, plus the data block
                 * offset plus the offset within the data block, scaled by the
                 * width of a char (we assume they're packed one per addressable unit) */
                addr = (dbg_str_sec_addr + data->d_off + cur_str_offset)/char_width + (char_width - 1);
                assert(addr < (unsigned long)-1);
                if ((string_obj = PyString_FromStringAndSize(packed_str,
                                                        len_packed)) == NULL ||
                    (addr_obj = Py_BuildValue("k",(unsigned long)(addr))) == NULL ||
                        PyDict_SetItem(debug_strings, addr_obj, string_obj) < 0)
                {
                    free(packed_str);
                    Py_XDECREF(string_obj);
                    Py_XDECREF(addr_obj);
                    Py_DECREF(debug_strings);
                    return NULL;
                }
                free(packed_str);
                Py_DECREF(string_obj);
                Py_DECREF(addr_obj);

                /* Calculate offset to start of next string */
                cur_str_offset += len_unpacked + char_width;
                len_unpacked = 0;
            }
        }
    }
    return debug_strings;
}

/**
 * Loop over the sections returning the contents of all executable sections
 * along with the LMA and VMA in a dictionary of (string, int, int) tuples
 */
static PyObject *Elf_Reader_get_program_sections(Elf_Reader *self)
{
    Elf_Scn *sec = NULL;
    Elf32_Phdr *phdr = elf32_getphdr(self->elf);
    size_t phnum;
    Elf32_Ehdr *ehdr = elf32_getehdr(self->elf);
    PyObject *prog_sections = PyDict_New();

    if (prog_sections == NULL)
    {
        return NULL;
    }

#ifdef NO_EXTENDED_PROGRAM_HEADER_TABLE
    phnum = ehdr->e_phnum;
#else
    if (elf_getphnum(self->elf, &phnum) < 0)
    {
        Py_DECREF(prog_sections);
        PyErr_SetString(PyExc_RuntimeError,
                "Failed to get number of program header table entries");
        return NULL;
    }
#endif

    while ((sec = elf_nextscn(self->elf, sec)) != NULL)
    {
        Elf32_Shdr *shdr;
        char *sec_name;

        /* Read section header */
        if ((shdr = elf32_getshdr(sec)) == 0)
        {
            Py_DECREF(prog_sections);
            PyErr_SetString(PyExc_RuntimeError, "getshdr failed!");
            return NULL;
        }

        /* Read section name */
        if ((sec_name = elf_strptr(self->elf, ehdr->e_shstrndx,
                                                        shdr->sh_name)) == NULL)
        {
            Py_DECREF(prog_sections);
            PyErr_SetString(PyExc_RuntimeError, "Failed to get section name!");
            return NULL;
        }

        /* Find sections that
         *  - take up space at runtime
         */
        if (shdr->sh_flags & SHF_ALLOC &&
            shdr->sh_type == SHT_PROGBITS &&
                (self->e_machine != EM_XAP || strcmp(sec_name, "debug_strings") != 0))
        {
            Elf_Data *data = elf_getdata(sec, NULL);

            if (data != NULL)
            {
                PyObject *text_string = NULL;
                PyObject *sec_tuple = NULL;

                size_t i;
                Elf32_Addr vma = 0, lma = 0; /* initialise to avoid spurious 
                                                "possibly uninitialised" warning */

                if ((text_string = PyString_FromStringAndSize(data->d_buf,
                                                        data->d_size)) == NULL)
                {
                    Py_DECREF(prog_sections);
                    return NULL;
                }

                /* Find the program segment containing this section */
                for (i = 0; i < phnum; ++i)
                {
                    if (phdr[i].p_offset <= shdr->sh_offset &&
                        shdr->sh_offset < phdr[i].p_offset + phdr[i].p_filesz)
                    {
                        lma = shdr->sh_offset + phdr[i].p_paddr - phdr[i].p_offset;
                        vma = shdr->sh_offset + phdr[i].p_vaddr - phdr[i].p_offset;
                        break;
                    }
                }
                if (i == phnum)
                {
                    Py_DECREF(prog_sections);
                    Py_DECREF(text_string);
                    PyErr_SetString(PyExc_RuntimeError,
                            "Couldn't find program header containing section");
                    return NULL;
                }

                assert(lma < (unsigned long)-1);
                assert(vma < (unsigned long)-1);
                if ((sec_tuple = Py_BuildValue("OkkI",
                                                text_string, lma, vma, shdr->sh_flags))
                                                                    == NULL ||
                        PyDict_SetItemString(prog_sections, sec_name,
                                                                sec_tuple) < 0)
                {
                    Py_XDECREF(sec_tuple);
                    Py_DECREF(prog_sections);
                    Py_DECREF(text_string);
                    return NULL;
                }
                Py_DECREF(text_string);
                Py_DECREF(sec_tuple);
            }
        }
    }
    return prog_sections;
}

static PyMethodDef Elf_Reader_methods[] = {
    {"get_symbols", (PyCFunction)Elf_Reader_get_symbols, METH_VARARGS,
       "Get dictionaries containing name/value mappings for all the symbols in "
            "various interesting categories"},
    {"get_debug_strings", (PyCFunction)Elf_Reader_get_debug_strings,
        METH_VARARGS, "Get a dictionary mapping debug string addresses to the "
                "strings themselves"},
    {"get_program_sections", (PyCFunction)Elf_Reader_get_program_sections,
            METH_VARARGS,
        "Get a list of loadable section text with LMA and VMA"},
    {NULL}
};



/**
 * Dwarf_Reader class
 *
 * This is where the action is.  This class looks up references to symbols and
 * CUs, which client code should store, and then returns information based on
 * the references.
 */
static PyTypeObject Dwarf_ReaderType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "_read_dwarf.reader",      /*tp_name*/
    sizeof(Dwarf_Reader),            /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)Dwarf_Reader_dealloc,/*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT,        /*tp_flags*/
    "DWARF reader",            /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    Dwarf_Reader_methods,            /* tp_methods */
    0,                         /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Dwarf_Reader_init,     /* tp_init */
    0,                         /* tp_alloc */
    0,                         /* tp_new */
};


/**
 * Elf_Reader class
 *
 * This is where the action is.  This class looks up references to symbols and
 * CUs, which client code should store, and then returns information based on
 * the references.
 */
static PyTypeObject Elf_ReaderType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "_read_dwarf.symtab_reader",      /*tp_name*/
    sizeof(Elf_Reader),            /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)Elf_Reader_dealloc,/*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT,        /*tp_flags*/
    "ELF symbol table reader",            /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    Elf_Reader_methods,            /* tp_methods */
    0,                         /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Elf_Reader_init,     /* tp_init */
    0,                         /* tp_alloc */
    0,                         /* tp_new */
};



static PyObject *get_version(PyObject *self, PyObject *unused_args)
{
    PyObject *version;

    version = PyInt_FromLong(READ_DWARF_VERSION);

    return version;
}

static PyObject *arch_is_supported(PyObject *self, PyObject* args)
{
    int em_code;
    if (PyArg_ParseTuple(args, "i", &em_code) < 0)
    {
        return NULL;
    }
    return PyBool_FromLong(read_dwarf_arch_supported(em_code));
}

static PyMethodDef ReadDwarfMethods[] = {
    {"get_version",  get_version, METH_VARARGS,
     "Returns the _read_dwarf version number."},
     {"arch_is_supported", arch_is_supported, METH_VARARGS,
     "Indicates whether a given architecture code (Elf header em_arch value) is "
     "supported"
     },
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

/**
 * Initialises the _read_dwarf module with the three classes above.
 */
PyMODINIT_FUNC
init_read_dwarf(void)
{
    PyObject *module;

    Dwarf_ReaderType.tp_new = PyType_GenericNew;
    Dwarf_Symbol_RefType.tp_new = PyType_GenericNew;
    Elf_ReaderType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&Dwarf_ReaderType) < 0 ||
        PyType_Ready(&Dwarf_Symbol_RefType) < 0 ||
        PyType_Ready(&Elf_ReaderType) < 0)
    {
        return;
    }

    module = Py_InitModule3("_read_dwarf", ReadDwarfMethods,
                            "Simplified DWARF (and ELF) reader");

    Py_INCREF(&Dwarf_ReaderType);
    Py_INCREF(&Dwarf_Symbol_RefType);
    Py_INCREF(&Elf_ReaderType);
    PyModule_AddObject(module, "Dwarf_Reader", (PyObject *)&Dwarf_ReaderType);
    PyModule_AddObject(module, "Symbol_Ref", (PyObject *)&Dwarf_Symbol_RefType);
    PyModule_AddObject(module, "Elf_Reader", (PyObject *)&Elf_ReaderType);
}

/*@}*/

/**
 * Given the DIE of a variable, get its name (may be anonymous) and
 * then pass its type DIE (if it has one) to \c parse_type to add type details.
 * This may result in recursive calls back here.
 *
 * \param rdc read_dwarf context
 * \param var_die Variable's DIE
 * \param parent_types PyList of all the types encountered in this variable's
 * parentage, used to avoid cyclic recursion
 * \param var_dict Python dictionary to insert the variable's details into
 * \param start_offset The byte offset at the variable appears in any containing
 * structure
 * \param byte_size (returned) The byte size of this variable
 * \return DW_DLV_OK if OK, DW_DLV_NO_ENTRY if the variable doesn't have a type,
 * or DW_DLV_ERROR otherwise.
 *
 * \ingroup parsing
 */
static int parse_variable(read_dwarf_ctx *rdc,
                          Dwarf_Die var_die,
                          PyObject *var_dict,
                          unsigned *byte_size)
{
    Dwarf_Die type_die;
    char typename[256];
    size_t typename_len = 0;

    if (read_dwarf_get_immediate_type(rdc, var_die, &type_die) != DW_DLV_OK)
    {
        return DW_DLV_ERROR;
    }

    if (parse_type(rdc, type_die, var_dict, byte_size, FALSE,
                   /* Point at the RH end of the typename array!*/
                   typename+sizeof(typename)-1, &typename_len) != DW_DLV_OK)
    {
        printf("Error parsing type\n");
        return DW_DLV_ERROR;
    }
    assert(typename_len < 256);

    return DW_DLV_OK;
}



static int add_member(read_dwarf_ctx *rdc, PyObject *member_list,
                      Dwarf_Die member_die)
{
    PyObject *member_ref;
    PyObject *member_tuple;
    char *name;
    Dwarf_Unsigned offset;
    Dwarf_Half child_tag;

    /* Check that this is a member and not some other sort of entity */
    switch(read_dwarf_tag(rdc, member_die, &child_tag))
    {
    case DW_DLV_ERROR:
        return DW_DLV_ERROR;
    case DW_DLV_NO_ENTRY:
        break; /* assume it's a member by default */
    case DW_DLV_OK:
        if (child_tag != DW_TAG_member)
        {
            return DW_DLV_NO_ENTRY;
        }
        break;
    }

    /* Get the name */
    if (read_dwarf_diename(rdc, member_die, &name) != DW_DLV_OK)
    {
        printf("Error getting member die name!\n");
        return DW_DLV_ERROR;
    }

    /* Get the offset */
    switch (read_dwarf_get_member_offset(rdc, member_die, &offset))
    {
    case DW_DLV_NO_ENTRY:
        offset = 0;
        break;
    case DW_DLV_ERROR:
        printf("Error getting member offset!\n");
        return DW_DLV_ERROR;
    default:
        break;
    }

    /* Create the symbol ref from the member DIE */
    member_ref = (PyObject *)create_symbol_ref(read_dwarf_die_offset(rdc, member_die));
    if (member_ref == NULL)
    {
        return DW_DLV_ERROR;
    }
    if ((member_tuple = Py_BuildValue("sLO", name, offset, member_ref)) == NULL)
    {
        Py_DECREF(member_ref);
        return DW_DLV_ERROR;
    }
    Py_DECREF(member_ref);

    if (PyList_Append(member_list, member_tuple) < 0)
    {
        Py_DECREF(member_tuple);
        return DW_DLV_ERROR;
    }
    Py_DECREF(member_tuple);

    return DW_DLV_OK;
}

/**
 * Add a (value, name) dictionary item to the supplied enumerator dict
 * @param rdc read_dwarf context
 * @param etr_dict Dictionary to add
 * @param etr_die
 * @return
 */
static int add_enumerator(read_dwarf_ctx *rdc, Dwarf_Die etr_die,
                          PyObject *etr_dict)
{
    char *name;
    Dwarf_Signed value;
    PyObject *value_obj;

    /* Get the name */
    if (read_dwarf_diename(rdc, etr_die, &name) != DW_DLV_OK)
    {
        return DW_DLV_ERROR;
    }

    /* Get the value */
    if (read_dwarf_enumerator_value(rdc, etr_die, &value) != DW_DLV_OK)
    {
        return DW_DLV_ERROR;
    }

    /* Insert an entry into the enumerator dictionary */
    value_obj = Py_BuildValue("i", value);
    if (PyDict_SetItemString(etr_dict, name, value_obj) < 0)
    {
        Py_DECREF(value_obj);
        return DW_DLV_ERROR;
    }
    Py_DECREF(value_obj);
    return DW_DLV_OK;
}


/**
 * Adds structure-specific parsing to the general type parsing done by \c
 * parse_type.  Should be passed a DIE with tag value structure_type.
 *
 * @param rdc read_dwarf context
 * @param type_die DIE of the structure type being parsed
 * @param type_dict Dictionary of type information, to add to
 * @param parent_types PyList to pass through; contains full set of parent types
 * @param start_address Address of first member
 * @return DW_DLV_OK if OK, DW_DLV_NO_ENTRY if structure has no children nad
 * DW_DLV_ERROR if anything went wrong.
 *
 * \ingroup parsing
 */

static int parse_type_structure(read_dwarf_ctx *rdc, Dwarf_Die type_die,
                                PyObject *type_dict)
{
    int retval;
    PyObject *member_list;
    Dwarf_Die member_die;
    Dwarf_Die prev_member_die;

    /* Walk through the child DIEs extracting information. */

    /* We have to put the members into a list rather than a dictionary, because
     * dictionaries don't preserve order, and we need the order preserved so
     * we can postpone calculating the actual addresses until the members are
     * accessed (the Python that allows access to individual members must
     * therefore calculate all the members' addresses at once) */

    switch(read_dwarf_child(rdc, type_die, &member_die))
    {
      case DW_DLV_OK:

          /* There is at least one child. Add it and its address to the type_dict,
           * and then get its size in order to get the address of the next member */

          /* Make a 'members' key in the dictionary containing an empty list*/
          member_list = PyList_New(0);
          if (member_list == NULL)
          {
              return DW_DLV_ERROR;
          }

          if (PyDict_SetItemString(type_dict, "members", member_list) < 0)
          {
              Py_DECREF(member_list);
              return DW_DLV_ERROR;
          }
          Py_DECREF(member_list);

          if (add_member(rdc, member_list, member_die) == DW_DLV_ERROR)
          {
              printf("Couldn't add first member!\n");
              read_dwarf_free_die(rdc, member_die);
              return DW_DLV_ERROR;
          }

          /* Now get each sibling, doing the same thing */
          prev_member_die = member_die;
          retval = read_dwarf_siblingof(rdc, prev_member_die, &member_die);
          read_dwarf_free_die(rdc, prev_member_die);
          while(retval == DW_DLV_OK)
          {
              if (add_member(rdc, member_list, member_die) == DW_DLV_ERROR)
              {
                  printf("Couldn't add sibling member!\n");
                  read_dwarf_free_die(rdc, member_die);
                  return DW_DLV_ERROR;
              }

              /* Get the next sibling, if there is one */
              prev_member_die = member_die;
              retval = read_dwarf_siblingof(rdc, prev_member_die, &member_die);
              read_dwarf_free_die(rdc, prev_member_die);
          }

          if (retval == DW_DLV_ERROR)
          {
              printf("Error getting sibling of!\n");
              return DW_DLV_ERROR;
          }

          /* We successfully got to the end of the struct members at this level*/
          break;

      case DW_DLV_NO_ENTRY:
          /* There's no child at this level, so just stop */
          return DW_DLV_NO_ENTRY;

      case DW_DLV_ERROR:

          printf("Error getting child!\n");
          return DW_DLV_ERROR;
      }

    return DW_DLV_OK;
}

/**
 * Return a dictionary containing the value-symbol mapping of an enumeration
 * @param rdc read_dwarf context
 * @param enum_die DIE of the enumeration
 * @return Value-symbol dictionary; or NULL otherwise
 */
static PyObject *parse_enumerated_type(read_dwarf_ctx *rdc, Dwarf_Die enum_die)
{
    PyObject *enum_dict = NULL;
    Dwarf_Die etr_die, etr_die2;
    int retval;

    switch(read_dwarf_child(rdc, enum_die, &etr_die))
    {
      case DW_DLV_OK:

          /* There is at least one child. Add it and its address to the type_dict,
           * and then get its size in order to get the address of the next member */

          enum_dict = PyDict_New();
          if (enum_dict == NULL)
          {
              return NULL;
          }

          if (add_enumerator(rdc, etr_die, enum_dict) != DW_DLV_OK)
          {
              Py_DECREF(enum_dict);
              return NULL;
          }

          /* Now get each sibling, doing the same thing */
          etr_die2 = etr_die;
          retval = read_dwarf_siblingof(rdc, etr_die2, &etr_die);
          while(retval == DW_DLV_OK)
          {
              if (add_enumerator(rdc, etr_die, enum_dict) != DW_DLV_OK)
              {
                  Py_DECREF(enum_dict);
                  return NULL;
              }

              /* Get the next sibling, if there is one */
              etr_die2 = etr_die;
              retval = read_dwarf_siblingof(rdc, etr_die2, &etr_die);
          }

          if (retval == DW_DLV_ERROR)
          {
              Py_DECREF(enum_dict);
              return NULL;
          }

          /* We successfully got to the end of the struct members at this level*/
          break;

      case DW_DLV_NO_ENTRY:
          /* There's no child at this level, so just stop */
          return NULL;

      case DW_DLV_ERROR:

          printf("Error getting child DIE");
          return NULL;
      }

    return enum_dict;

}

/**
 * Insert information about the supplied type into the supplied dictionary.
 * We're interested in its name, its size and, depending on the type of type,
 * as it were, its members.
 *
 * \param rdc read_dwarf context
 * \param type_die DIE of struct we want to analyse
 * \param type_dict Python dictionary to contain informatoin about this type
 * \param start_offset Pointer to the byte offset of the parent variable in a
 * structure.
 * \param type_byte_size (returned).  Total size of this type in bytes.
 * \param is_pointed_to Flag indicating that the type being parsed is pointed
 * to by a structure rather than being part of the structure directly (which
 * affects whether it's appropriate to put an address offset field in the dict)
 * \param typename_start Pointer to the start of the currently constructed
 * typename fragment. This level should write additional name fragments to the
 * left and update \c *typename_len
 * \param typename_len Current length of the typename string.  Should be updated
 * as this function adds more fragments to the left.
 * \return DW_DLV_OK unless something went wrong
 *
 * \ingroup parsing
 */
static int parse_type(read_dwarf_ctx *rdc,
                      Dwarf_Die type_die,
                      PyObject *dict,
                      unsigned *type_byte_size,
                      int is_pointed_to,
                      char *typename_start,
                      size_t *typename_len)
{
    Dwarf_Half tag;
    PyObject *tag_obj;
    int byte_size;

    char *typename, *local_typename_start;
    Dwarf_Bool is_typedef_name = FALSE;
    Dwarf_Die element_type, resolved_type, pointed_to_type;
    unsigned qualifiers;
    Dwarf_Off array_length;
    size_t local_typename_len, frag_len;
    unsigned pointed_to_size, element_type_size;

    PyObject *pointed_to_dict;
    PyObject *element_type_dict;
    PyObject *enum_dict;
    PyObject *void_obj;
    PyObject *param_list;

    /* Flag to set if a typeless item (i.e. a void pointer) is encountered*/
    int halt_recursion = FALSE;
    int got_typename;

    typename = NULL;
    /* Get as much information about the type as we can at this level */
    switch (read_dwarf_resolve_type(rdc, type_die, &typename, &is_typedef_name,
                                    &resolved_type,
                                    &tag,
                                    &qualifiers, &array_length))
    {
    case DW_DLV_ERROR:
        printf("Can't resolve type\n");
        return DW_DLV_ERROR;
    default:
        break;
    }

    /* If we've we got something from the call above, we don't need to keep
     * constructing it in the recursive call: we'll supply a separate memory
     * area for building the pointed-to typename instead */
    got_typename = typename != NULL;

    frag_len = 0;
    local_typename_len = 0;
    local_typename_start = typename_start;

    read_dwarf_construct_typename_fragment(rdc, typename, is_typedef_name,
                                           qualifiers,
                                           tag,
                                           array_length,
                                           local_typename_start,
                                           &frag_len);

    local_typename_len += frag_len;
    local_typename_start -= frag_len;
    frag_len = 0;

    tag_obj = Py_BuildValue("i", tag);
    if (PyDict_SetItemString(dict, "type_tag", tag_obj) < 0)
    {
        Py_DECREF(tag_obj);
        return DW_DLV_ERROR;
    }
    Py_DECREF(tag_obj);


    /* Now add information to the dictionary relating to the type of the
     * variable*/
    switch(tag)
    {
    case DW_TAG_pointer_type:
    /* If it's a pointer, we have to deal with both the pointer itself (the
     * resolved type) and the type pointed to, which we do by recursing. */

        /* Details of the pointer type itself are set later */

        /* Recurse to parse the pointed-to type. */
        pointed_to_dict = PyDict_New();

        if (PyDict_SetItemString(dict, "pointed_to", pointed_to_dict) < 0)
        {
            Py_DECREF(pointed_to_dict);
            return DW_DLV_ERROR;
        }
        /* dict's reference is enough now */
        Py_DECREF(pointed_to_dict);

        switch (read_dwarf_get_immediate_type(rdc, resolved_type,
                                               &pointed_to_type))
        {
        case DW_DLV_NO_ENTRY:
            /* It's a void pointer. Pretend we've parsed the type, at least
             * for naming purposes */
            void_obj = Py_BuildValue("s","void");
            if (PyDict_SetItemString(pointed_to_dict, "type_name",
                                                                void_obj) < 0)
            {
                Py_DECREF(void_obj);
                return DW_DLV_ERROR;
            }
            Py_DECREF(void_obj);

            memcpy(local_typename_start-4,"void",4);
            frag_len += 4;
            halt_recursion = TRUE;
            break;
        case DW_DLV_ERROR:
            printf("Failed to get pointer's immediate type DIE\n");
            return DW_DLV_ERROR;
        }

        if (! halt_recursion)
        {
            /* Recurse... */
            char recursive_typename[256];
            char *recursive_typename_start = recursive_typename +
                                                sizeof(recursive_typename) - 1;
            size_t recursive_frag_len;
            if (parse_type(rdc, pointed_to_type, pointed_to_dict,
                           &pointed_to_size, TRUE,
                           got_typename ? recursive_typename_start :
                                                           local_typename_start,
                           got_typename ? &recursive_frag_len : &frag_len) !=
                                                                      DW_DLV_OK)
            {
                printf("Failed to parse pointed-to type\n");
                return DW_DLV_ERROR;
            }
        }
        break;

    /* If it's an array, we have to construct the details of the array itself
     * (the resolved type) and the length by recursing to resolve the element
     * type and the length.
     */
    case DW_TAG_array_type:

        /* Store the number of elements if known */
        if (array_length != READ_DWARF_NO_ARRAY_LENGTH)
        {
            PyObject *array_length_obj = Py_BuildValue("i", array_length);
            if(PyDict_SetItemString(dict, "num_elements",
                                                        array_length_obj) < 0)
            {
                Py_DECREF(array_length_obj);
                return DW_DLV_ERROR;
            }
            Py_DECREF(array_length_obj);
        }

        /* Recurse to parse the element type. */
        element_type_dict = PyDict_New();

        if (PyDict_SetItemString(dict, "element_type", element_type_dict) < 0)
        {
            Py_DECREF(element_type_dict);
            return DW_DLV_ERROR;
        }
        /* dict's reference is enough now */
        Py_DECREF(element_type_dict);

        if (read_dwarf_get_immediate_type(rdc, resolved_type,
                                          &element_type) != DW_DLV_OK)
        {
            printf("Failed to get array's immediate type DIE\n");
            return DW_DLV_ERROR;
        }

        {
            /* Recurse... */
            char recursive_typename[256];
            char *recursive_typename_start = recursive_typename +
                                                sizeof(recursive_typename) - 1;
            size_t recursive_frag_len;
            if (parse_type(rdc, element_type, element_type_dict,
                           &element_type_size, FALSE,
                           got_typename ? recursive_typename_start :
                                                           local_typename_start,
                           got_typename ? &recursive_frag_len : &frag_len) !=
                                                                      DW_DLV_OK)
            {
                printf("Failed to parse element type\n");
                return DW_DLV_ERROR;
            }
        }
        break;

    /*
     * Otherwise, if it's a structure recurse, if not, just get the size and
     * update the offset and stuff.  We already have the full name in this
     * case.
     */
    case DW_TAG_structure_type:
    case DW_TAG_union_type:
        /* Do special things to parse the structure */
        switch (parse_type_structure(rdc, resolved_type, dict))
        {
        case DW_DLV_ERROR:
            printf("Error parsing structure details\n");
            return DW_DLV_ERROR;
        case DW_DLV_NO_ENTRY:
            /* TODO: If this function returns DW_DLV_NO_ENTRY, it implies there
             * were no child DIEs, suggesting that this is an incomplete type.
             * We really need to raise an exception here and let the caller
             * try an instance of the parent structure definition from another
             * translation unit (see B-190361).  */
            break;
        default:
            break;
        }
        break;

    case DW_TAG_enumeration_type:
        /* The DIE's children are all its enumerators.  We want these in a
         * dictionary so we can look up names of enumerator values */
        if ((enum_dict = parse_enumerated_type(rdc, resolved_type)) == NULL)
        {
            printf("Error parsing enumeration\n");
            return DW_DLV_ERROR;
        }
        if (PyDict_SetItemString(dict, "enumerators", enum_dict) < 0)
        {
            Py_DECREF(enum_dict);
            return DW_DLV_ERROR;
        }
        Py_DECREF(enum_dict);
        break;

    case DW_TAG_subroutine_type:
    {
        Dwarf_Die return_type_die;
        /* Add the DIE's formal parameters to the dictionary */
        param_list = get_formal_param_list(rdc, resolved_type);
        if (PyDict_SetItemString(dict, "params", param_list) < 0)
        {
            Py_DECREF(param_list);
            return DW_DLV_ERROR;
        }
        Py_DECREF(param_list);
        /* Add the DIE's return type */
        switch (read_dwarf_get_immediate_type(rdc, type_die, &return_type_die))
        {
        case DW_DLV_OK:
        {
            Dwarf_Symbol_Ref *symbol_ref;
            if ((symbol_ref = create_symbol_ref(
                                  read_dwarf_die_offset(rdc, return_type_die)))
                                                                       == NULL)
            {
                PyErr_SetString(PyExc_RuntimeError,
                                "Error initialising Dwarf_Symbol_Ref");
                read_dwarf_free_die(rdc, return_type_die);
                return DW_DLV_ERROR;
            }
            read_dwarf_free_die(rdc, return_type_die);
            if (PyDict_SetItemString(dict, "return_type", (PyObject *)symbol_ref) < 0)
            {
                Py_DECREF(symbol_ref);
                return DW_DLV_ERROR;
            }
            break;
        }
        case DW_DLV_NO_ENTRY:
            break;
        case DW_DLV_ERROR:
            set_exception(PyExc_RuntimeError, rdc,
                          "Failed to get return type of subroutine_type DIE");
            return DW_DLV_ERROR;
        }
        break;
    }

    default:
        break;
    }

    /* We should now have a complete typename, but we need to copy it somewhere
     * else and add the required null terminator.*/
    local_typename_start -= frag_len;
    local_typename_len += frag_len;
    frag_len = 0;

    {
        char fullname[256];
        PyObject *fullname_obj;
        assert(local_typename_len < sizeof(fullname)-1);
        /* We're just interested in the local part of the typename */
        memcpy(fullname, local_typename_start, local_typename_len);
        fullname[local_typename_len] = (char)0;

        fullname_obj = Py_BuildValue("s", fullname);
        if (PyDict_SetItemString(dict, "type_name", fullname_obj) < 0)
        {
            Py_DECREF(fullname_obj);
            return DW_DLV_ERROR;
        }
        Py_DECREF(fullname_obj);
    }

    *typename_len += local_typename_len;

    /* Include the name of the base type. */
    {
        PyObject *basename_obj;
        PyObject *basename_name_obj;
        char *name="";
        const char *prefix="";

        if (tag != 0)
        {
            switch (read_dwarf_diename(rdc, resolved_type, &name))
            {
                case DW_DLV_NO_ENTRY:
                    /* Anonymous type */
                    name = "<anonymous>";
                    break;
                case DW_DLV_ERROR:
                    printf("Failed to get name of variable's base type\n");
                    return DW_DLV_ERROR;
                default:
                    if (strcmp(name,"") != 0)
                    {
                        prefix = read_dwarf_typename_prefix(tag);
                    }
                    else
                    {
                        name = "<anonymous>";
                    }
                    break;
            }
        }
        else
        {
            /* tag == 0 implies there is no resolved type, i.e. the type name
             * is void */
            name = "void";
        }
        basename_obj = Py_BuildValue("s", prefix);
        basename_name_obj = Py_BuildValue("s",name);
        PyString_ConcatAndDel(&basename_obj, basename_name_obj);
        if (PyDict_SetItemString(dict, "base_type_name", basename_obj) < 0)
        {
            Py_DECREF(basename_obj);
            return DW_DLV_ERROR;
        }
        Py_DECREF(basename_obj);
    }

    if (! tag)
    {
        /* Void "type" */
        byte_size = READ_DWARF_INT_RETURN_NO_ENTRY;
    }
    else if (tag != DW_TAG_array_type)
    {
        /* Record byte size */
        byte_size = read_dwarf_get_byte_size(rdc, resolved_type);
    }
    else
    {
        if (array_length != READ_DWARF_NO_ARRAY_LENGTH)
        {
            byte_size = (int)(element_type_size * array_length);
        }
        else
        {
            byte_size = element_type_size;
        }
    }

    switch(byte_size)
    {
    case READ_DWARF_INT_RETURN_ERROR:
        /* Error */
        return DW_DLV_ERROR;
    case READ_DWARF_INT_RETURN_NO_ENTRY:
      /* Attribute doesn't exist */
      {
          PyObject *not_present_obj = Py_BuildValue("s","<not present>");
          if (PyDict_SetItemString(dict, "byte_size", not_present_obj) < 0)
          {
              Py_DECREF(not_present_obj);
              return DW_DLV_ERROR;
          }
          Py_DECREF(not_present_obj);
      }
      /* We should only get here when parsing the (non existent) type pointed
       * to by a void *, in which case the type_byte_size doesn't matter */
      *type_byte_size = 0;
      break;
    default:
        /* Successfully got a value */
        {
            PyObject *byte_size_obj = Py_BuildValue("i", byte_size);
            if (PyDict_SetItemString(dict, "byte_size", byte_size_obj) < 0)
            {
                Py_DECREF(byte_size_obj);
                return DW_DLV_ERROR;
            }
            Py_DECREF(byte_size_obj);
        }
        *type_byte_size = byte_size;
        break;
    }
    return DW_DLV_OK;
}

