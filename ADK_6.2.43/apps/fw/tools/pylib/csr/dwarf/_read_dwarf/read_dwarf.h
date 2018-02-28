/* Copyright (c) 2016 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */

#include "libelf.h"
#include "libdwarf.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define EM_KALIMBA 0xdb
#define EM_XAP 0x9ba0

/**
 * Defined verbosity levels for library output
 */
typedef enum
{
    READ_DWARF_VERBOSITY_SILENT = 0,  /**< Never print anything */
    READ_DWARF_VERBOSITY_ERRORS = 1,  /**< Print errors only */
    READ_DWARF_VERBOSITY_WARNINGS = 2,/**< Print warnings and errors */
    READ_DWARF_VERBOSITY_INFO = 3     /**< Print intresting info too */
} READ_DWARF_VERBOSITY;

#define READ_DWARF_INVALID_DIE_OFFSET (Dwarf_Off)(-1)

/**
 * Context structure for the Python-free level of the read_dwarf extension
 * module
 */
struct read_dwarf_ctx
{
  /** Descriptor of elf file */
  int fd;
  /** libdwarf context */
  Dwarf_Debug dbg;
  /** libdwarf error */
  Dwarf_Error err;

  /** Data for stack frame unwinding */
  Dwarf_Cie *cies;
  Dwarf_Signed n_cies;
  Dwarf_Fde *fdes;
  Dwarf_Signed n_fdes;

  /* TODO Move the following into a separate struct we can just allocate
   * when we need to? */

  /** flag to set to terminate a traversal of a DIE tree */
  unsigned char cancel_traversal;
  /** space for holding random const things */
  const void *scratch_in;
  void *scratch_out;

  /**
   * We have to remember the offset of the next CU when iterating through the
   * CUs
   */
  Dwarf_Unsigned next_cu_hdr_offset;

  /** Indicates whether to write error messagse or not */
  READ_DWARF_VERBOSITY verbosity;
};

typedef struct read_dwarf_ctx read_dwarf_ctx;


/**
 * \defgroup init [C layer] Initialisation and de-initialisation
 */

/*@{*/
/**
 * Initialise the dwarf reading context.
 *
 * \param elf_filename Filename of the ELF file containing DWARF information
 * \param elf_opened Pointer to an already opened ELF context
 * \return Opaque read_dwarf context.  NULL if there was an error.
 */
extern read_dwarf_ctx *read_dwarf_init(Elf *elf_opened, const char *elf_filename,
                                       READ_DWARF_VERBOSITY verbosity_level);

/**
 * Indicates whether the given architecture code is supported by this build of
 * read_dwarf
 */
extern int read_dwarf_arch_supported(Elf32_Half em);

/**
 * Properly close the DWARF context
 *
 * \param rdc read_dwarf context being closed
 */
extern void read_dwarf_finish(read_dwarf_ctx *rdc);

/**
 * Reset the verbosity level.  Returns the old level.
 */
extern READ_DWARF_VERBOSITY read_dwarf_set_verbosity(read_dwarf_ctx *rdc,
                                              READ_DWARF_VERBOSITY new_level);

/**
 * Return pointer to the last libdwarf error message
 * @param rdc read_dwarf context
 * @return Pointer to the last libdwarf error message
 */
extern char *read_dwarf_errmsg(read_dwarf_ctx *rdc);
/*@}*/

/**
 * \defgroup searching [C layer] High level searches
 */

/*@{*/

extern int read_dwarf_get_next_cu(read_dwarf_ctx *rdc, Dwarf_Die *next_cu);


/**
 * Search the DWARF info for the specific compilation unit
 *
 * \param rdc read_dwarf context
 * \param target_cu_name Full name (relative to fw/src in the Curator case) of
 * the required compilation unit
 * \param target_cu_die Pointer to return the retrieved DIE in.  If the value
 * is not required, set to NULL
 * \return DW_DLV_OK if CU found; DW_DLV_NO_ENTRY if not found; DW_DLV_ERROR if
 * something went wrong
 */
extern int read_dwarf_get_cu_die(read_dwarf_ctx *rdc,
                                 const char *target_cu_name,
                                 Dwarf_Die *target_cu_die);

/**
 * Traverse the DIE tree rooted at \c root_die storing a copy of each DIE that
 * describes a symbol of the indicated tag-type with the external attribute
 *
 * Note: this function mallocs the returned array and passes ownership to the
 * caller.
 *
 * @param rdc read_dwarf context
 * @param root_die DIE to start traversing from (e.g. a CU DIE)
 * @param target_tag Tag-type of desired DIEs
 * @param external_dies Pointer to array of external var dies in the tree, of
 * length \c n_vars (returned)
 * @param n_vars Pointer to returned number of such external vars in the tree
 * @return DW_DLV_OK unless memory allocation failed
 */
extern int read_dwarf_get_externals_in_tree(read_dwarf_ctx *rdc,
                                            Dwarf_Die root_die,
                                            Dwarf_Half target_tag,
                                            Dwarf_Die **external_dies,
                                            unsigned *n_vars);

extern int read_dwarf_get_nonexternals_in_tree(read_dwarf_ctx *rdc,
                                               Dwarf_Die root_die,
                                               Dwarf_Half target_tag,
                                               Dwarf_Die **external_dies,
                                               unsigned *n_vars);

/**
 * Find the named variable symbol in the given compile unit
 *
 * @param rdc read_dwarf context
 * @param cu_die DIE of target compile unit
 * @param symbol_name Name of target symbol
 * @param symbol_die Pointer to space to return symbol's DIE in
 * @return The usual
 */

extern int read_dwarf_find_var_symbol_in_cu(read_dwarf_ctx *rdc,
                                            Dwarf_Die cu_die,
                                            const char *symbol_name,
                                            Dwarf_Die *symbol_die);


/**
 * Find the named symbol in the given compile unit
 *
 * @param rdc read_dwarf context
 * @param cu_die DIE of target compile unit
 * @param symbol_name Name of target symbol
 * @param symbol_die Pointer to space to return symbol's DIE in
 * @return The usual
 */

extern int read_dwarf_find_symbol_in_cu(read_dwarf_ctx *rdc,
                                        Dwarf_Die cu_die,
                                        const char *symbol_name,
                                        Dwarf_Die *symbol_die);

/**
 * Typedef defining action functions for DIE tree traversal
 * 
 * \param die The DIE that is to be acted upon
 * \param level. The current (child) recursion level.
 *
 * \return DW_DLV_OK if ownership taken of symbol, DW_DLV_NO_ENTRY if ownership
 * not taken
 */
typedef int (read_dwarf_action_fn) (read_dwarf_ctx *rdc, 
                                    Dwarf_Die die, int level);

/**
 * Action function that gets a copy of a variable symbol's DIE
 * 
 * \param rdc The read_dwarf context. rdc->scratch_in should point to the
 * symbol's name on entry, and rdc->scratch will point to its DIE on exit, or
 * NULL if not found.
 * \param die The die to be acted upon
 * \param level The current recursion level
 *
 * \return DW_DLV_OK if symbol found (and hence ownership taken),
 *         DW_DLV_NO_ENTRY if not
 */
extern int read_dwarf_action_find_var_symbol(read_dwarf_ctx *rdc, Dwarf_Die die,
                                             int level);


/**
 * Action function that gets a copy of an arbitrary symbol's DIE
 *
 * \param rdc The read_dwarf context. rdc->scratch_in should point to the
 * symbol's name on entry, and rdc->scratch will point to its DIE on exit, or
 * NULL if not found.
 * \param die The die to be acted upon
 * \param level The current recursion level
 *
 * \return DW_DLV_OK if symbol found (and hence ownership taken),
 *         DW_DLV_NO_ENTRY if not
 */
extern int read_dwarf_action_find_symbol(read_dwarf_ctx *rdc, Dwarf_Die die,
                                         int level);

/**
 * Depth-first DIE tree traversal
 *
 * \param rdc read_dwarf context
 * \param root_die Base of the tree
 * \param action Action function defining what to do with each node
 * \param level_limit The level at which to stop implementing the action (0
 * means apply it at all levels)
 * \param level The current (child) recursion depth
 * \param include_children Boolean to ignore children altogether
 * \param exclude_root If TRUE don't apply the action to the supplied root DIE
 * \return DW_DLV_OK means the action function took ownership of root_die
 *         DW_DLV_NO_ENTRY means it didn't, so the caller should take action to
 *         free it
 */
extern int read_dwarf_traverse_die_tree(read_dwarf_ctx *rdc,
                                         Dwarf_Die root_die,
                                         read_dwarf_action_fn action,
                                         int level_limit,
                                         int level,
                                         int include_children,
                                         int exclude_root);

/*@}*/

/**
 * \defgroup access [C layer] General access
 */

/*@{*/

enum
{
    READ_DWARF_TYPE_QUAL_CONST_MASK = 0x1,
    READ_DWARF_TYPE_QUAL_VOLATILE_MASK = 0x2
};

/**
 * Simple wrapper around libdward's \c dwarf_child.
 * 
 * \param rdc read_dwarf context
 * \param parent_die DIE whose child we want
 * \param child_die Space to store child DIE we're after
 * \return DW_DLV_OK if OK, DW_DLV_NO_ENTRY if no child exists,
 * DW_DLV_ERROR if it all went wrong
 */
extern int read_dwarf_child(read_dwarf_ctx *rdc,
                            Dwarf_Die parent_die,
                            Dwarf_Die *child_die);

/**
 * Simple wrapper around libdwarf's \c dwarf_siblingof.
 * 
 * \param rdc read_dwarf context
 * \param older_sibling DIE whose sibling we want
 * \param younger_sibling Space to store sibling DIE we're after
 * \return DW_DLV_OK if OK, DW_DLV_NO_ENTRY if no younger sibling exists,
 * DW_DLV_ERROR if it all went wrong
 */
extern int read_dwarf_siblingof(read_dwarf_ctx *rdc,
                                Dwarf_Die older_sibling,
                                Dwarf_Die *younger_sibling);

/**
 * Determine if the supplied DIEs are the same one or not
 * @param rdc read_dwarf context
 * @param die1 First DIE to compare
 * @param die2 Other DIE to compare
 * @return 1 if they are the same; 0 if they're not.
 */
extern int read_dwarf_same_die(read_dwarf_ctx *rdc,
                               Dwarf_Die die1, Dwarf_Die die2);

/**
 * Get the tag of a DIE (simple wrapper around libdwarf function)
 * 
 * \param rdc read_dwarf context
 * \param die DIE whose tag we want
 * \param tag Pointer to space for the tag to be returned in
 * \return DW_DLV_OK if everything was OK; else DW_DLV_ERROR.
 */
extern int read_dwarf_tag(read_dwarf_ctx *rdc,
                          Dwarf_Die die,
                          Dwarf_Half *tag);

/**
 * Indicate whether the given DIE has the given attribute
 * @param rdc read_dwarf context
 * @param die DIE we're checking
 * @param attr_id Attribute we're checking for
 * @param has_attr Whether the attribute is present or not
 * @param form If the attribute is present, what its form is (if not present,
 * this pointer's value is undefined; if NULL, it is ignored)
 * @param attr_desc If the attribute is present, its descriptor (if not present,
 * this pointer's value is undefined; if NULL, it is ignored)
 * @return DW_DLV_OK if everything was OK, else DW_DLV_ERROR
 */
extern int read_dwarf_has_attr(read_dwarf_ctx *rdc,
                               Dwarf_Die die,
                               Dwarf_Half attr_id,
                               Dwarf_Bool *has_attr,
                               Dwarf_Half *form,
                               Dwarf_Attribute *attr_desc);

/**
 * Free the memory allocated for the DwarfAttribute
 */
extern void read_dwarf_free_attr(read_dwarf_ctx *rdc, Dwarf_Attribute attr);

/**
 * Free the memory allocated for the Dwarf DIE
 */
extern void read_dwarf_free_die(read_dwarf_ctx *rdc, Dwarf_Die die);

/**
 * Get the value of an unsigned int attribute
 * @param rdc read_dwarf context
 * @param attr Descriptor of the attribute we're checking
 * @param value Pointer to space for the returned attribute value
 * @return DW_DLV_OK if everything was OK, else DW_DLV_ERROR
 */
extern int read_dwarf_get_unsigned_attr(read_dwarf_ctx *rdc,
                                 Dwarf_Attribute attr,
                                 Dwarf_Unsigned *value);

/**
 * Get the value of an address attribute
 * @param rdc read_dwarf context
 * @param attr Descriptor of the attribute we're checking
 * @param value Pointer to space for the returned attribute value
 * @return DW_DLV_OK if everything was OK, else DW_DLV_ERROR
 */
extern int read_dwarf_get_addr_attr(read_dwarf_ctx *rdc,
                                 Dwarf_Attribute attr,
                                 Dwarf_Addr *value);

/**
 * Get the value of an arbitrary int attribute, returned as a signed value (so
 * if it's really a Dwarf_Unsigned, it must fit)
 * @param rdc read_dwarf context
 * @param attr Descriptor of the attribute we're checking
 * @param form_signed Is its form signed rather than unsigned?
 * @param value Pointer to space for the returned attribute value
 * @return DW_DLV_OK if everything was OK, else DW_DLV_ERROR
 */
extern int read_dwarf_get_int_attr(read_dwarf_ctx *rdc,
                                   Dwarf_Attribute attr,
                                   Dwarf_Bool form_signed,
                                   Dwarf_Signed *value);

/**
 * Get the value of a string attribute
 * @param rdc read_dwarf context
 * @param attr Descriptor of the attribute we're getting
 * @param value Pointer to space for the returned char *
 * @return DW_DLV_OK if everything was OK, else DW_DLV_ERROR
 */
extern int read_dwarf_get_string_attr(read_dwarf_ctx *rdc,
                                      Dwarf_Attribute attr,
                                      const char **value);

/** Get the value of a reference attribute
 * @param rdc read_dwarf context
 * @param attr Descriptor of the attribute we're getting
 * @param as_global If TRUE convert to a global offset
 * @param offset Pointer to space for the returned offset value
 * @return DW_DLV_OK if everything was OK, else DW_DLV_ERROR
 */
extern int read_dwarf_get_ref_attr(read_dwarf_ctx *rdc,
                                   Dwarf_Attribute attr,
                                   int as_global,
                                   Dwarf_Off *offset);

/** Get the value of a global reference attribute
 * @param rdc read_dwarf context
 * @param attr Descriptor of the attribute we're getting
 * @param offset Pointer to space for the returned offset value
 * @return DW_DLV_OK if everything was OK, else DW_DLV_ERROR
 */
extern int read_dwarf_get_gbl_ref_attr(read_dwarf_ctx *rdc,
        Dwarf_Attribute attr,
        Dwarf_Off *offset);

/**
 * Get the abstract_origin attribute of a DIE.
 * @param rdc read_dwarf context
 * @param concrete_die The DIE representing a concrete instance
 * @param abstract_die Pointer to pace for returning the associated abstract
 * instance DIE
 * @param allow_die_retrieval_errors With GNU ARM ELFs it appears possible that
 * the abstract_origin attribute may exist but not point to a valid DIE.  This
 * parameter allows control of whether an error from dwarf_offdie should be
 * propagated or suppressed.
 * @return DW_DLV_NO_ENTRY if the (supposedly) concrete DIE doesn't actually
 * have an abstract_origin attribute, DW_DLV_ERROR if something went wrong
 * retrieving it, otherwise DW_DLV_OK.
 */
extern int read_dwarf_abstract_origin(read_dwarf_ctx *rdc,
        Dwarf_Die concrete_die,
        Dwarf_Die *abstract_die);

/**
 * Get the name of a DIE (simple wrapper around libdwarf function)
 *
 * \param rdc read_dwarf context
 * \param die DIE whose name we want
 * \param name Pointer to space for a char * to be returned
 * \return DW_DLV_OK if everything was OK; else DW_DLV_NO_ENTRY if no name
 * attribute; else DW_DLV_ERROR if something else went wrong.
 */
extern int read_dwarf_diename(read_dwarf_ctx *rdc,
                              Dwarf_Die die,
                              char **name);

/**
 * Get either the plain name attribute or the full name (name + compilation dir)
 * of a CU DIE.
 *
 * \note The full name is returned in malloc space in all circumstances and the
 * caller owns the memory
 *
 * \param rdc read_dwarf context
 * \param cu_die DIE representing a CU
 * \param full_name If true, will concatenate the compilation directory to the
 * actual name using whichever kind of path separator is found first in the
 * compilation directory
 * \return A pointer to the string in malloc'd space.  Memory is owned by the
 * caller.
 */
char * read_dwarf_cu_name(read_dwarf_ctx *rdc, Dwarf_Die cu_die, int full_name);

/**
 * Get the global offset of the DIE
 * \param rdc read_dwarf context
 * \param die DIE whose offset we want
 * \return The DIE's global (not CU-relative) offset, or READ_DWARF_INVALID_DIE_OFFSET
 * in case of error
 */
extern Dwarf_Off read_dwarf_die_offset(read_dwarf_ctx *rdc, Dwarf_Die die);

/**
 * Check if the entry is "abstract", i.e. doesn't correspond to a real entity
 * in the executable code, but (presumably) just an entity in the source code.
 * The main purpose of this is to detect inlined functions so as to avoid
 * adding them to the function dictionary (their inlined instances are
 * accessible via the function they are inlined into).
 */
extern int read_dwarf_is_abstract_entry(read_dwarf_ctx *rdc, Dwarf_Die die);


/**
 * Get the DIE at the given global offset
 * \param rdc read_dwarf context
 * \param offset global DIE offset
 * \param die Space to return the DIE in
 * \return DW_DLV_OK if all good, DW_DLV_ERROR if not
 */
extern int read_dwarf_offdie(read_dwarf_ctx *rdc, Dwarf_Off offset, Dwarf_Die *die);

/**
 * Get the value attribute of an enumerator DIE
 * @param rdc read_dwarf context
 * @param etr_die Enumerator DIE whose value we want
 * @param value Pointer to the value
 * @return DW_DLV_NO_ENTRY if the DIE doesn't have a "const_value" attribute;
 * otherwise DW_DLV_OK if OK and DW_DLV_ERROR if not.
 */
extern int read_dwarf_enumerator_value(read_dwarf_ctx *rdc,
                                       Dwarf_Die etr_die,
                                       Dwarf_Signed *value);


/**
 * Get the type DIE associated with the supplied DIE and
 * return in target_die.  It is safe for target_die to point to
 * source_die on entry.  It will only be overwritten on
 * successful completion.
 *
 * \param rdc read_dwarf context
 * \param source_die The Die whose type is sought
 * \param target_die Pointer to space to write the result to (if any)
 * \return DW_DLV_OK if successfully found the type DIE;
 * DW_DLV_NO_ENTRY if the source_die has no type attribute;
 * DW_DLV_ERROR if something goes horribly wrong.
 */
extern int read_dwarf_get_immediate_type(read_dwarf_ctx *rdc,
                                         Dwarf_Die source_die,
                                         Dwarf_Die *target_die);

/**
 * Get the length of a one-dimensional array
 * @param rdc read_dwarf context
 * @param array_type_die DIE of one-dimensional array
 * @param length Pointer to returned length
 * @return DW_DLV_OK if OK, DW_DLV_ERROR if not.
 */
extern int read_dwarf_get_array_size(read_dwarf_ctx *rdc,
                                     Dwarf_Die array_type_die,
                                     Dwarf_Unsigned *length);

/**
 * For the given type DIE, get the name of the top-level typedef, the DIE and
 * tag of the resolved type (the first non-qualifier, non-typedef DIE in the
 * type tree), associated qualifiers of the resolved type (const, volatile),
 * and (if an array) the length.
 *
 * If the resolved_type_tag is DW_TAG_array_type or DW_TAG_pointer_type, the
 * decl_name parameter will not be set, because the resolved type is only known
 * to be an array/pointer - what it is an array of/a pointer to can only be
 * resolved by calling the function again on the resolved_type, continuing to
 * do so until the tag is something else.
 *
 * To use this function to get the details of a variable's type, call
 * \c read_dwarf_get_immediate_type_die first to get the input DIE to this
 * function.
 *
 * Any pointer parameter can be NULL if the value is not of interest.
 *
 * @param rdc read_dwarf context
 * @param die Type DIE whose details we're interested in
 * @param decl_name Returned string containing the declared type name (may be a
 * typedef)
 * @oaram is_typedef_name Flag indicating if it is in fact a typedef
 * @param resolved_type First non-typedef and non-qualifier that is encountered
 * in the type chain
 * @param resolved_type_tag DWARF tag of the resolved_type, for convenience
 * @param qualifiers Bitmap indicating qualifiers (see READ_DWARF_TYPE_QUAL_*)
 * @param Pointer to returned array length.  Only set if type has array qualifier.
 * @return DW_DLV_OK if OK; DW_DLV_ERROR if not
 */
extern int read_dwarf_resolve_type(read_dwarf_ctx *rdc, Dwarf_Die die,
                                   char **decl_name, Dwarf_Bool *is_typedef_name,
                                   Dwarf_Die *resolved_type,
                                   Dwarf_Half *resolved_type_tag,
                                   unsigned *qualifiers,
                                   Dwarf_Unsigned *array_size);

/**
 * Return a string containing the typename of the supplied variable DIE.
 *
 * Note: the string is in malloc memory ownership of which is passed to the
 * caller.  The length of the array malloc'd is always exactly the length of
 * the string plus one for the null terminator.
 *
 * @param rdc read_dwarf context
 * @param die Variable DIE whose typename we want
 * @return malloc'd string containing the typename
 */
extern char *read_dwarf_var_typename(read_dwarf_ctx *rdc, Dwarf_Die die);

/**
 * Return a string containing the typename of the supplied type DIE.
 *
 * Note: the string is in malloc memory ownership of which is passed to the
 * caller.  The length of the array malloc'd is always exactly the length of
 * the string plus one for the null terminator.
 *
 * @param rdc read_dwarf context
 * @param die Type DIE whose typename we want
 * @return malloc'd string containing the typename
 */
extern char *read_dwarf_typename(read_dwarf_ctx *rdc, Dwarf_Die type_die);

/**
 * Get the prefix for the name of a type with the given tag - "struct " for
 * structs, etc.
 */
extern const char * read_dwarf_typename_prefix(Dwarf_Half tag);


/**
 * Add string fragments to the left of a given char * based on type qualifiers,
 * the DWARF tag, the base type name and (for struct/enum/union) whether this is
 * the name of a typedef pointing at the type or the name of the type itself: in
 * the latter case "struct"/"enum"/"union" is prepended to the supplied name.
 *
 * Note: this function is intended to be called in parallel with \c
 * read_dwarf_resolve_type.  If the latter returns a name and the resolved type
 * is (say) a pointer, the name must be a typedef which refers to the pointer
 * type.  Hence this function doesn't add a "*" to the name string of a type
 * resolved to a pointer unless the \c name parameter is NULL (in which case a
 * subsequent call will be required to complete the type name)
 *
 * @param rdc read_dwarf context
 * @param name Base type name.  Null if this isn't available.
 * @param is_typedef_name Is the name the name of a typedef?
 * @param qualifiers Bitmap of qualifiers (const, volatile)
 * @param tag DWARF tag of type - we care about pointer_type and array_type
 * @param array_length Length of array for tag == array_type
 * @param name_start Pointer to current start of name - fragment will be added
 * to left, so there had better be room :)
 * @param frag_len Pointer to returned length of added fragment.
 */
extern void read_dwarf_construct_typename_fragment(read_dwarf_ctx *rdc,
                                                   const char *name,
                                                   Dwarf_Bool is_typedef_name,
                                                   unsigned qualifiers,
                                                   Dwarf_Half tag,
                                                   Dwarf_Unsigned array_length,
                                                   char *name_start,
                                                   size_t *frag_len);

#define READ_DWARF_INT_RETURN_NO_ENTRY (-1)
#define READ_DWARF_INT_RETURN_ERROR (-2)
#define READ_DWARF_NO_ARRAY_LENGTH ((unsigned)-1)

/**
 * Get the byte_size of the given DIE
 *
 * \param rdc read_dwarf context
 * \param die DIE whose byte size we want
 * \return Byte size (0 or more) if successfully found; \c
 * READ_DWARF_INT_RETURN_NO_ENTRY if DIE didn't have byte_size attribute;
 * \c READ_DWARF_INT_RETURN_ERROR if something else went wrong
 *
 */
extern int read_dwarf_get_byte_size(read_dwarf_ctx *rdc, Dwarf_Die die);


/**
 * Read the offset within the struct of the given member, if given
 * @param rdc read_dwarf context
 * @param member_die DIE whose offset we want (must be a struct member DIE)
 * @param member_offset Pointer to return the offset in
 * @return DW_DLV_NO_ENTRY if there is no DATA_AT_data_member_location
 * attribute; else DW_DLV_OK or DW_DLV_ERROR if something bad happens.
 */
extern int read_dwarf_get_member_offset(read_dwarf_ctx *rdc,
                                        Dwarf_Die member_die,
                                        Dwarf_Unsigned *member_offset);

/*@}*/

/**
 * \defgroup globals [C layer] Globals access
 */

/*@{*/

/**
 * Get a list of global object handles for insertion into subsequent functions
 *
 * \param rdc read_dwarf context
 * \param globals Returned pointer to list of global objects
 * \param nglobals Pointer to size of returned list
 * \return DW_DLV_OK or DW_DLV_ERROR
 */
extern int read_dwarf_get_globals(read_dwarf_ctx *rdc, Dwarf_Global **globals,
                                  Dwarf_Signed *nglobals);

/**
 * Free the memory associated with the list of globals returned by
 * \c read_dwarf_get_globals.  This function should be called instead of free()!
 *
 * @param rdc read_dwarf context
 * @param globals List to free
 * @param ngbls Length of list to free
 */
extern void read_dwarf_free_globals(read_dwarf_ctx *rdc, Dwarf_Global *globals,
                                    Dwarf_Signed ngbls);

/**
 * Get the name of a global
 *
 * @param rdc read_dwarf context
 * @param global The global whose name is required
 * @param name Pointer to a NULL-terminated string containing the name.  This
 * should be freed later by calling \c read_dwarf_free_globname
 * @return DW_DLV_OK or DW_DLV_ERROR.
 */
extern int read_dwarf_get_globname(read_dwarf_ctx *rdc,
                                   Dwarf_Global global,
                                   char **name);

/**
 * Free the string containing a globname
 *
 * @param rdc read_dwarf context
 * @param name String to free
 */
extern void read_dwarf_free_globname(read_dwarf_ctx *rdc, char *name);

/**
 * Populate a pre-allocated list with the DIEs corresponding to the globals
 * given.
 *
 * @param rdc read_dwarf context
 * @param global Dwarf_Global we want the DIE for
 * @param global_die Space to pass the DIE back in
 * @return DW_DLV_OK if OK, else DW_DLV_ERROR
 */
extern int read_dwarf_get_global_die(read_dwarf_ctx *rdc,
                                      Dwarf_Global global,
                                      Dwarf_Die *global_die);

/*@}*/

/** \defgroup stack [C layer] Stack backtrace support */

/*@{*/

/**
 * Look for the DW_AT_calling_convention attribute and check whether it is set
 * to "normal". Sets \c *has_normal_cc to True if either the attribute isn't
 * present or it has the value DW_CC_normal; else sets it to False.  If the
 * return value is not DW_DLV_OK, the pointer is not set.
 * @param rdc read_dwarf context
 * @param func_die DIE of function we're interested in
 * @param has_normal_cc Pointer to space for returned flag
 * @return DW_DLV_OK if no errors occurred, else DW_DLV_ERROR.
 */
extern int read_dwarf_has_normal_calling_convention(read_dwarf_ctx *rdc,
                                                    Dwarf_Die func_die,
                                                    Dwarf_Bool *has_normal_cc);
/**
 * Get the low program counter of the supplied DIE
 * @param rdc read_dwarf context
 * @param func_die DIE of function in question
 * @param low_pc Pointer to returned low PC
 * @return DW_DLV_NO_ENTRY if the attribute isn't present, DW_DLV_ERROR if
 * something went wrong, else DW_DLV_OK
 */
extern int read_dwarf_get_low_pc(read_dwarf_ctx *rdc, Dwarf_Die func_die,
                                 Dwarf_Addr *low_pc);

/**
 * @param rdc read_dwarf context
 * @param func_die DIE of function in question
 * @param ranges Pointer to space for the ranges array pointer
 * @return DW_DLV_NO_ENTRY if the ranges attribute isn't present, DW_DLV_ERROR if
 * something went wrong, else DW_DLV_OK
 */
extern int read_dwarf_get_ranges(read_dwarf_ctx *rdc,
        Dwarf_Die func_die,
        Dwarf_Addr **ranges,
        unsigned *len_ranges);


/**
 * Get the producer string of the supplied DIE's containing CU
 * @param rdc read_dwarf context
 * @param die DIE whose producer we're interested in
 * @param producer_string Space for returning a const char * containing the
 * producer string; can be NULL
 * @return DW_DLV_ERROR if an error looking up the CU DIE or getting the producer
 * attribute from it; DW_DLV_NO_ENTRY if CU DIE has no producer attribute, in
 * which case *producer_string will be set to the empty string; DW_DLV_OK if
 * OK.
 */
extern int read_dwarf_get_producer(read_dwarf_ctx *rdc, Dwarf_Die die,
                                    const char **producer_string);


/**
 * Get the unwind rules for a specified set of registers.  The rules are
 * returned as pairs of integers: the second is a register number if the first
 * is false, and otherwise an offset from the canonical frame address.
 *
 * Note: *rules is malloc'd by this function; ownership of the memory is taken
 * by the caller.  However, it is only malloc'd if the return status is
 * DW_DLV_OK: in other cases, it will be NULL.
 *
 * @param rdc read_dwarf context
 * @param func_die DIE of function we're interested in
 * @param addr_offset Offset of program counter within that function
 * @param reg_nums Array of register numbers for which rules should be returned
 * @param num_regs Length of \c reg_nums
 * @param rules Pointer to an array of rules corresponding to the supplied
 * register numbers.
 * @param add_return_address_rule If TRUE, returns one more rule than requested
 * corresponding to the return address register number indicated by the
 * function's CIE
 * Note: There are two entries in this array per register number
 * @return DW_DLV_OK unless something went wrong, in which case DW_DLV_ERROR
 */
extern int read_dwarf_unwind_data(read_dwarf_ctx *rdc, Dwarf_Die func_die,
                                  Dwarf_Addr addr_offset, Dwarf_Signed *reg_nums,
                                  Dwarf_Signed num_regs, Dwarf_Signed **rules,
                                  Dwarf_Bool add_return_address_rule);


/**
 * Get the source file and line number corresponding to the supplied program
 * address
 *
 * Note: the caller must free the memory containing the source file name by
 * calling \c read_dwarf_free_srcfile
 *
 * @param rdc read_dwarf context
 * @param func_die DIE of function in which the PC is found
 * @param pc Program counter offset within function
 * @param line_no Pointer to returned source file line number
 * @return string containing the source file name, or NULL if source file/line
 * number info couldn't be found.  In this case, if *line_no is set to
 * non-zero, the failure to find any info was legitimate.  If it is
 * set to zero, the failure should be treated as an error.
 */
extern char *read_dwarf_get_source_file_and_line(read_dwarf_ctx *rdc,
                                                 Dwarf_Die func_die,
                                                 Dwarf_Addr pc_offset,
                                                 Dwarf_Unsigned *line_no);

/**
 * read_dwarf_free_srcfile is identical to read_dwarf_free_globname in practice
 */
#define read_dwarf_free_srcfile read_dwarf_free_globname

/**
 * Obtain an array of DIEs corresponding to the list of children of the given
 * type of the given function
 *
 * Note: ownership of the malloc'd array is passed to the caller.
 *
 * @param rdc read_dwarf context
 * @param func_die DIE of function whose formal parameters we are after
 * @param entry_tag Type of child to collect
 * @param child_dies Pointer to array of DIEs (returned)
 * @param n_children Pointer to size of the array (returned)
 * @return DW_DLV_OK on success, DW_DLV_ERROR otherwise
 */
extern int read_dwarf_get_func_child_entries(read_dwarf_ctx *rdc,
                                             Dwarf_Die func_die,
                                             Dwarf_Half entry_tag,
                                             Dwarf_Die **child_dies,
                                             unsigned *n_children);


/**
 * Determine where a given local variable is available on the stack or whether
 * it only lived temporarily in a register
 * @param rdc read_dwarf context
 * @param local DIE of the local variables
 * @param pc PC at which we want to locate the variable
 * @param executing Whether we've stopped before executing this instruction or
 *  during execution (usually just means it's a call instruction in an unwound
 *  frame
 * @param op_list Pointer to an array of DW_OP_* values (Dwarf_Loc.lr_atom)
 * @param arg_list Pointer to an array of argument triples corresponding to the
 * op_list entries (Dwarf_Loc.{lr_number, lr_number2, lr_offset})
 * @param num_ops Length of op_list array (arg_list array is three times as long)
 * @return DW_DLV_OK if a location entry was found covering this PC;
 * DW_DLV_NO_ENTRY if no entry found; DW_DLV_ERROR if something went wrong.
 */
extern
int read_dwarf_get_loc_list(read_dwarf_ctx *rdc, Dwarf_Die local, Dwarf_Addr pc,
                            Dwarf_Bool executing,
                            Dwarf_Small **op_list, Dwarf_Unsigned **arg_list,
                            unsigned *num_ops);


extern void read_dwarf_mem_stats(read_dwarf_ctx *rdc);

/*@}*/
