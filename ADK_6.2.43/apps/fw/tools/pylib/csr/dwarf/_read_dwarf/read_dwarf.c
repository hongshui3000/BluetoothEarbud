/* Copyright (c) 2016 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */
#include "read_dwarf.h"
#include "dwarf.h"
#include <errno.h>
#include <string.h>
#undef NDEBUG
#include <assert.h>

#ifdef _WIN32
#include <io.h>
#define open _open
#define close _close
#define OPEN_MODE (_O_RDONLY | _O_BINARY)
#else
#include <unistd.h>
#define OPEN_MODE (O_RDONLY)
#endif
/**
 * \file
 */

#define TRUE 1U
#define FALSE 0U

/* Named constants for the count/store tree traversal mechanism */
#define READ_DWARF_ANY_TAG 0
#define READ_DWARF_ANY_ATTR 0

static unsigned decode_leb128u(Dwarf_Small *encoded);

static int read_dwarf_action_count_tag_attr(read_dwarf_ctx *rdc,
                                             Dwarf_Die die, int level);

static int read_dwarf_action_store_tag_attr(read_dwarf_ctx *rdc,
                                             Dwarf_Die die, int level);

static int read_dwarf_action_count_nonexternals(read_dwarf_ctx *rdc,
                                             Dwarf_Die die, int level);

static int read_dwarf_action_store_nonexternals(read_dwarf_ctx *rdc,
                                             Dwarf_Die die, int level);

static int read_dwarf_get_tag_attr_in_tree(read_dwarf_ctx *rdc,
                                           Dwarf_Die root_die,
                                           Dwarf_Half target_tag,
                                           Dwarf_Half target_attr,
                                           int level_limit,
                                           Dwarf_Die **target_dies,
                                           unsigned *n_dies);


/**
 * Generic error report
 * @param level Minimum verbosity level to output at
 * @param msg Message to output
 * @param err Dwarf_Error which should have been set
 */
static void _report(read_dwarf_ctx *rdc, READ_DWARF_VERBOSITY level,
                    const char *msg)
{
    if (rdc->verbosity >= level)
    {
        printf("%s\n", msg);
        if (level < READ_DWARF_VERBOSITY_INFO && rdc->err != NULL)
        {
            printf("%s\n", dwarf_errmsg(rdc->err));
        }
    }
}

#define _error(rdc, msg) _report(rdc, READ_DWARF_VERBOSITY_ERRORS, msg)
#define _warning(rdc, msg) _report(rdc, READ_DWARF_VERBOSITY_WARNINGS, msg)
#define _info(rdc, msg) _report(rdc, READ_DWARF_VERBOSITY_INFO, msg)

char *read_dwarf_errmsg(read_dwarf_ctx *rdc)
{
    return dwarf_errmsg(rdc->err);
}

/**
 * Close up shop: end the Elf session, close the ELF file descriptor and
 * free the context structure.
 *
 * @param elf Elf object - can be obtained from the Dwarf_Debug context structure
 * @param rdc read_dwarf context
 */
static void _cleanup(Elf *elf, read_dwarf_ctx *rdc)
{
    if (rdc->fd != 0)
    {
        (void)elf_end(elf);
        close(rdc->fd);
    }
    free(rdc);
}


int read_dwarf_arch_supported(Elf32_Half em)
{
    switch (em)
    {
#ifndef READ_DWARF_DISABLE_ARM_SUPPORT
    case EM_ARM:
        break;
#endif
#ifndef READ_DWARF_DISABLE_XAP_SUPPORT
    case EM_XAP:
        break;
#endif
#ifndef READ_DWARF_DISABLE_KALIMBA_SUPPORT
    case EM_KALIMBA:
        break;
#endif
    default:
        return FALSE;
    }
    return TRUE;
}


read_dwarf_ctx *read_dwarf_init(Elf *elf_opened, const char *elf_filename,
                                READ_DWARF_VERBOSITY verbosity_level)
{
  Elf *elf;
  Elf32_Ehdr *ehdr;
  read_dwarf_ctx *rdc;

  /* Create a new read_dwarf context object, to be returned */
  rdc = (read_dwarf_ctx *)malloc(sizeof(read_dwarf_ctx));
  if (rdc == NULL)
  {
      return NULL;
  }

  (void)memset(rdc, 0, sizeof(read_dwarf_ctx));
  rdc->verbosity = verbosity_level;

  /* If we haven't been passed an open ELF object, open one ourselves */
  if (!elf_opened)
  {

      /* Elf version check is mandatory */
      if (elf_version(EV_CURRENT) == EV_NONE)
        {
          printf("Version mismatch!\n");
          return NULL;
        }

      /* Open the supplied Elf file */
      rdc->fd = open(elf_filename,OPEN_MODE);
      if (rdc->fd < 0)
        {
          printf("Opening '%s' failed!\n", elf_filename);
          printf("Error opening file: %s\n", strerror( errno ) );
          free(rdc);
          return NULL;
        }

      /* Initialise the ELF session */
      elf = elf_begin(rdc->fd, ELF_C_READ, NULL);

      if (elf == NULL)
        {
          printf("elf_begin failed!\n");
          printf("%s\n", elf_errmsg(elf_errno()));
          /*(void)elf_end(elf);*/
          free(rdc);
          return NULL;
        }
  }
  else
  {
      elf = elf_opened;
  }

  /* Initialise the DWARF session */
  switch(dwarf_elf_init(elf, DW_DLC_READ, NULL, NULL, &rdc->dbg, &rdc->err))
    {
    case DW_DLV_OK:
      break;
    case DW_DLV_NO_ENTRY:
      printf("ELF file contains no DWARF information\n");
      _cleanup(elf, rdc);
      return NULL;
    case DW_DLV_ERROR:
      printf("DWARF initialisation failed!\n");
      printf("%s\n", dwarf_errmsg(rdc->err));
      _cleanup(elf, rdc);
      return NULL;
    default:
      printf("Unexpected return value from dwarf_elf_init\n");
      _cleanup(elf, rdc);
      return NULL;
    }

  /* Set architecture-specific details */

  if ((ehdr = elf32_getehdr(elf)) == NULL)
  {
      printf("%s\n", elf_errmsg(elf_errno()));
      printf("Failed to get elf header!\n");
      return NULL;
  }

  if (!read_dwarf_arch_supported(ehdr->e_machine))
  {
      printf("Unsupported chip.  Not starting.\n");
      return NULL;
  }
  if (ehdr->e_machine == EM_ARM)
  {
      /* infocenter.arm.com/help/topic/com.arm.doc.ihi0040b/IHI0040B_aadwarf.pdf,
       * Table 1 */
      dwarf_set_frame_rule_table_size(rdc->dbg, 288);
  }

  return rdc;
}


void read_dwarf_finish(read_dwarf_ctx *rdc)
{
  Elf *elf;

  if (rdc == NULL)
  {
      return;
  }

  if (rdc->fdes != NULL)
  {
     dwarf_fde_cie_list_dealloc(rdc->dbg, rdc->cies, rdc->n_cies,
                               rdc->fdes, rdc->n_fdes);
  }

  /* Get the ELF context structure out of the DWARF one */
  if (dwarf_get_elf(rdc->dbg, &elf, &rdc->err) != DW_DLV_OK)
    {
      printf("dwarf_get_elf failed!\n");
      elf = NULL;
    }

  /* Close the DWARF session */
  if (dwarf_finish(rdc->dbg, &rdc->err) != 0)
    {
      printf("dwarf_finish failed!\n");
    }

  _cleanup(elf, rdc);
}

READ_DWARF_VERBOSITY read_dwarf_set_verbosity(read_dwarf_ctx *rdc,
                                              READ_DWARF_VERBOSITY new_level)
{
    READ_DWARF_VERBOSITY old_level = rdc->verbosity;
    rdc->verbosity = new_level;
    return old_level;
}



int read_dwarf_get_cu_die(read_dwarf_ctx *rdc, const char *target_cu_name,
                          Dwarf_Die *target_cu_die)
{
    Dwarf_Unsigned cu_length, cu_offset, cu_next_offset;
    Dwarf_Half cu_version, cu_pointer_size;
    Dwarf_Off cu_abbrev_offset, cu_die_offset;
    char *cu_name;
    Dwarf_Die cu_die;
    unsigned len;
    char *basename;
    int still_looking = TRUE;

    /* Do a linear search through the compilation units DWARF knows about.
     * dwarf_next_cu_header keeps track internally of where it's up to.  We have
     * to cycle through all the CUs every time in order to reset the search
     * for the next time.*/
    cu_offset = 0;
    while (dwarf_next_cu_header(rdc->dbg,
                                &cu_length,
                                &cu_version,
                                &cu_abbrev_offset,
                                &cu_pointer_size,
                                &cu_next_offset,
                                &rdc->err) == DW_DLV_OK)
    {
        if (still_looking)
        {
            /* Get the global offset to the CU DIE */
            if (dwarf_get_cu_die_offset_given_cu_header_offset(rdc->dbg,
                                                               cu_offset,
                                                               &cu_die_offset,
                                                              &rdc->err) != DW_DLV_OK)
            {
                _error(rdc, "Error getting cu_die_offset");
            }

            /* Get the DIE at the specified offset */
            if (dwarf_offdie(rdc->dbg, cu_die_offset, &cu_die, &rdc->err) != DW_DLV_OK)
            {
                _error(rdc, "Error getting DIE at offset");
                return DW_DLV_ERROR;
            }

            /* Check its name against the target name */
            if (dwarf_diename(cu_die, &cu_name, &rdc->err) != DW_DLV_OK)
            {
                _error(rdc, "Error getting DIE name");
                return DW_DLV_ERROR;
            }

            if (strcmp(cu_name,target_cu_name) == 0)
            {
                *target_cu_die = cu_die;
                still_looking = FALSE;
            }

            /* If the full name didn't match, does the basename match? */
            len = (unsigned)strlen(cu_name);

            /* Search backwards through the string for the first separator
             and then compare from the next character*/
            for (basename = cu_name + len-1; basename != cu_name; basename--)
            {
                if (*basename == '/' || *basename == '\\')
                {
                    basename++;
                    break;
                }
            }
            /* If we found a separator, see if the basename matches the
               target name */
            if (basename != cu_name && strcmp(basename, target_cu_name) == 0)
            {
                *target_cu_die = cu_die;
                still_looking = FALSE;
            }

            cu_offset = cu_next_offset;
        }
    }

    if (still_looking)
    {
        /* We've run out of CUs without finding a match. */
        return DW_DLV_NO_ENTRY;
    }
    return DW_DLV_OK;
}                

int read_dwarf_get_next_cu(read_dwarf_ctx *rdc,
                           Dwarf_Die *next_cu)
{
    Dwarf_Off cu_offset;
    Dwarf_Off cu_new_hdr_offset, cu_next_offset;
    Dwarf_Die next_cu_l;
    int retval = DW_DLV_OK;

    cu_new_hdr_offset = rdc->next_cu_hdr_offset;

    /* Before going further, check that this isn't off the end of the list, and
     * get the next CU offset if there is one */
    switch (dwarf_next_cu_header(rdc->dbg,
                                NULL,
                                NULL,
                                NULL,
                                NULL,
                                &cu_next_offset,
                                &rdc->err))
    {
    case DW_DLV_OK:
        rdc->next_cu_hdr_offset = cu_next_offset;
        break;
    case DW_DLV_NO_ENTRY:
        /* Reset for another iteration */
        rdc->next_cu_hdr_offset = 0;
        return DW_DLV_NO_ENTRY;
        break;
    case DW_DLV_ERROR:
        _error(rdc, "Error getting next CU DIE!");
        return DW_DLV_ERROR;
    }

    next_cu_l = NULL;
    /* Get the global offset to the CU DIE */
    if (dwarf_get_cu_die_offset_given_cu_header_offset(rdc->dbg,
                                                       cu_new_hdr_offset,
                                                       &cu_offset,
                                                       &rdc->err) != DW_DLV_OK)
    {
        _error(rdc, "Error getting cu_die_offset");
        return DW_DLV_ERROR;
    }
    /* Get the DIE at the specified offset */
    else if (dwarf_offdie(rdc->dbg, cu_offset, &next_cu_l, &rdc->err) == DW_DLV_ERROR)
    {
        _error(rdc, "dwarf_offdie failed for CU");
        return DW_DLV_ERROR;
    }
    *next_cu = next_cu_l;

    return retval;
}

int read_dwarf_get_externals_in_tree(read_dwarf_ctx *rdc,
                                     Dwarf_Die root_die,
                                     Dwarf_Half target_tag,
                                     Dwarf_Die **external_dies,
                                     unsigned *n_dies)
{
    return read_dwarf_get_tag_attr_in_tree(rdc, root_die, target_tag,
                                           DW_AT_external,
                                           0, /* No level limit */
                                           external_dies,
                                           n_dies);
}

int read_dwarf_get_tag_attr_in_tree(read_dwarf_ctx *rdc,
                                    Dwarf_Die root_die,
                                    Dwarf_Half target_tag,
                                    Dwarf_Half target_attr,
                                    int level_limit,
                                    Dwarf_Die **tag_attr_dies,
                                    unsigned *n_dies)
{
    Dwarf_Half args[2];

    assert(n_dies != NULL);

    args[0] = target_tag;
    args[1] = target_attr;
    rdc->scratch_in = (void *)args;

    *n_dies = 0;
    rdc->scratch_out = (void *)n_dies;
    (void)read_dwarf_traverse_die_tree(rdc, root_die,
                                 read_dwarf_action_count_tag_attr,
                                 level_limit, 0, FALSE, TRUE);

    /* That should have filled ext_var_count with the number of external vars
     * in the CU, so allocate space for them and  */
    if (*n_dies == 0)
    {
        return DW_DLV_OK;
    }

    *tag_attr_dies = malloc(*n_dies * sizeof(Dwarf_Die));
    if (*tag_attr_dies == NULL)
    {
        printf ("Error allocating space for %d Dwarf_Dies", *n_dies);
        return DW_DLV_ERROR;
    }

    /* Now traverse the tree again copying the DIEs into the storage */
    rdc->scratch_out = (void *)(*tag_attr_dies);
    (void)read_dwarf_traverse_die_tree(rdc, root_die,
                                 read_dwarf_action_store_tag_attr,
                                 level_limit, 0, FALSE, TRUE);

    return DW_DLV_OK;
}


int read_dwarf_get_nonexternals_in_tree(read_dwarf_ctx *rdc,
                                        Dwarf_Die root_die,
                                        Dwarf_Half target_tag,
                                        Dwarf_Die **nonexternal_dies,
                                        unsigned *n_dies)
{
    assert(n_dies != NULL);

    *n_dies = 0;
    rdc->scratch_in = (void *)&target_tag;
    rdc->scratch_out = (void *)n_dies;
    (void)read_dwarf_traverse_die_tree(rdc, root_die,
                                 read_dwarf_action_count_nonexternals, 0, 0,
                                 FALSE, TRUE);

    /* That should have filled ext_var_count with the number of external vars
     * in the CU, so allocate space for them and  */
    if (*n_dies == 0)
    {
        return DW_DLV_OK;
    }

    *nonexternal_dies = (Dwarf_Die *)malloc(*n_dies * sizeof(Dwarf_Die));
    if (*nonexternal_dies == NULL)
    {
        printf ("Error allocating space for %d Dwarf_Dies", *n_dies);
        return DW_DLV_ERROR;
    }

    /* Now traverse the tree again copying the DIEs into the storage */
    rdc->scratch_out = (void *)*nonexternal_dies;
    (void)read_dwarf_traverse_die_tree(rdc, root_die,
                                 read_dwarf_action_store_nonexternals, 0, 0,
                                 FALSE, TRUE);

    return DW_DLV_OK;
}

int read_dwarf_get_globals(read_dwarf_ctx *rdc, Dwarf_Global **globals,
                           Dwarf_Signed *nglobals)
{
    int ret = dwarf_get_globals(rdc->dbg, globals, nglobals, &rdc->err);
    if (ret == DW_DLV_ERROR)
    {
        _error(rdc, "Failed getting globals list");
    }
    return ret;
}

void read_dwarf_free_globals(read_dwarf_ctx *rdc, Dwarf_Global *globals,
                            Dwarf_Signed ngbls)
{
    dwarf_globals_dealloc(rdc->dbg, globals, ngbls);
}

int read_dwarf_get_global_die(read_dwarf_ctx *rdc,
                               Dwarf_Global global,
                               Dwarf_Die *global_die)
{
    Dwarf_Off offset;

    if (dwarf_global_die_offset(global, &offset, &rdc->err) != DW_DLV_OK)
    {
        _error(rdc, "Error getting global's offset");
        return DW_DLV_ERROR;
    }

    switch (read_dwarf_offdie(rdc, offset, global_die))
    {
    case DW_DLV_OK:
        break;
    case DW_DLV_NO_ENTRY:
        _error(rdc, "No debugging info in expected location for global");
        return DW_DLV_ERROR;
    case DW_DLV_ERROR:
        _error(rdc, "Error retrieving global DIE from global");
        return DW_DLV_ERROR;
    }
    return DW_DLV_OK;
}

int read_dwarf_get_globname(read_dwarf_ctx *rdc,
                            Dwarf_Global global,
                            char **name)
{
    if (dwarf_globname(global, name, &rdc->err) != DW_DLV_OK)
    {
        _error(rdc, "Error getting name of global");
        return DW_DLV_ERROR;
    }
    return DW_DLV_OK;
}

void read_dwarf_free_globname(read_dwarf_ctx *rdc, char *name)
{
    dwarf_dealloc(rdc->dbg, name, DW_DLA_STRING);
}


/* Get siblings.  For each sibling, get child and recurse */
int read_dwarf_traverse_die_tree(read_dwarf_ctx *rdc,
                  Dwarf_Die root_die, 
                  read_dwarf_action_fn action,
                  int level_limit,
                  int level,
                  int include_children,
                  int exclude_root)
{
    Dwarf_Die sibling_die = 0;
    Dwarf_Die child_die = 0;
    int root_ret = DW_DLV_NO_ENTRY; /* Unless the action explicitly tells us
    it has stored the DIE, it hasn't */
    int sibling_ret = DW_DLV_ERROR; /* set to an invalid value first */

    rdc->cancel_traversal = FALSE;

    /* First, execute the requested action on this node */
    if (!exclude_root && (level_limit == 0 || level < level_limit))
    {
        root_ret = action(rdc, root_die, level);
    }

    /* If we got what we came for, jump out */
    if (rdc->cancel_traversal)
    {
        return root_ret;
    }

    /* Otherwise, recurse on the DIE's child, if any */
    if ((level == 0 || include_children) &&
                dwarf_child(root_die, &child_die, &rdc->err) == DW_DLV_OK)
    {
        if (read_dwarf_traverse_die_tree(rdc, child_die, action, level_limit,
                                     level+1, include_children,
                                     FALSE) == DW_DLV_NO_ENTRY)
        {
            read_dwarf_free_die(rdc, child_die);
        }
    }

    /* Propagate cancellation up the call stack */
    if (rdc->cancel_traversal)
    {
        return root_ret;
    }

    /* Finally loop over the sibling DIEs, if any */
    if (level > 0)
    {
        Dwarf_Die older_sibling_die = root_die;
        while(dwarf_siblingof(rdc->dbg, older_sibling_die,
                &sibling_die, &rdc->err) == DW_DLV_OK)
        {
            /* Don't free the root die first time through: that's the
             * original root die that was passed in, so we're not responsible
             * for it.  But free all its siblings that we encounter where they
             * haven't been stored (including
             * the very last - see below) */
            if (older_sibling_die != root_die && sibling_ret == DW_DLV_NO_ENTRY)
            {
                read_dwarf_free_die(rdc, older_sibling_die);
            }


            if (level_limit == 0 || level < level_limit)
            {
                sibling_ret = action(rdc, sibling_die, level);
            }
            /* If we got what we came for, jump out */
            if (rdc->cancel_traversal)
            {
                if (sibling_ret == DW_DLV_NO_ENTRY)
                {
                    read_dwarf_free_die(rdc, sibling_die);
                }
                return root_ret;
            }

            /* Otherwise, recurse on the DIE's child, if any */
            if (include_children &&
                        dwarf_child(sibling_die, &child_die, &rdc->err) == DW_DLV_OK)
            {
                if (read_dwarf_traverse_die_tree(rdc, child_die, action, level_limit,
                                             level+1, include_children,
                                             FALSE) == DW_DLV_NO_ENTRY)
                {
                    read_dwarf_free_die(rdc, child_die);
                }
            }
            /* Propagate cancellation up the call stack */
            if (rdc->cancel_traversal)
            {
                if (sibling_ret == DW_DLV_NO_ENTRY)
                {
                    read_dwarf_free_die(rdc, sibling_die);
                }
                return root_ret;
            }
            older_sibling_die = sibling_die;
        }
        if (sibling_die != 0 && sibling_ret == DW_DLV_NO_ENTRY)
        {
            read_dwarf_free_die(rdc, sibling_die);
        }
    }
    return root_ret;
}


int read_dwarf_action_find_var_symbol(read_dwarf_ctx *rdc, Dwarf_Die die,
                                      int level)
{
    Dwarf_Half tag;

    /* Is this a variable DIE? */
    if (read_dwarf_tag(rdc, die, &tag) == DW_DLV_OK && tag == DW_TAG_variable)
    {
        /* Does the DIE name match the request name? */
        return read_dwarf_action_find_symbol(rdc, die, level);
    }
    return DW_DLV_NO_ENTRY;
} 

int read_dwarf_action_find_symbol(read_dwarf_ctx *rdc, Dwarf_Die die,
                                  int level)
{
    const char *target_name;
    char *die_name;
    int dres;

    /* Does the DIE name match the request name? */

    target_name = (const char *)rdc->scratch_in;

    dres = dwarf_diename(die, &die_name, &rdc->err);
    if (dres == DW_DLV_OK && strcmp(target_name, die_name) == 0)
    {
        /* Store a copy of the DIE in scratch space */
        /* This will be free'd by the calling function */
        rdc->scratch_out = malloc(sizeof(Dwarf_Die));
        *(Dwarf_Die *)rdc->scratch_out = die;
        /* Set the cancel_traversal flag to break out of the tree
         * traversal */
        rdc->cancel_traversal = TRUE;
        return DW_DLV_OK;
    }
    return DW_DLV_NO_ENTRY;
}

/**
 * Increment scratch_out (which should be pointing to an integer) if this DIE
 * is a variable with the supplied tag and attribute (in scratch_in).
 *
 * \c scratch_in should point to a \c Dwarf_Half[2]: the tag we're looking for
 * is the first element and the attribute we're looking for is the second.
 * Both can be set to 0 to mean "any".
 *
 * @param rdc read_dwarf context
 * @param die The DIE to examine
 * @param level The current recursion level (unused)
 * @return Always returns DW_DLV_NO_ENTRY as ownership never taken
 */
static int read_dwarf_action_count_tag_attr(read_dwarf_ctx *rdc,
                                            Dwarf_Die die, int level)
{
    Dwarf_Half tag, search_tag, search_attr;
    Dwarf_Bool has_attr;

    (void)level;

    search_tag = ((Dwarf_Half *)rdc->scratch_in)[0];
    search_attr = ((Dwarf_Half *)rdc->scratch_in)[1];

    if (search_tag == 0 ||
        (dwarf_tag(die, &tag, &rdc->err) == DW_DLV_OK && tag == search_tag))
    {
        if (search_attr == 0 ||
            (dwarf_hasattr(die, search_attr, &has_attr, &rdc->err) == DW_DLV_OK
                                                                && has_attr))
        {
            (*(unsigned *)rdc->scratch_out)++;
        }
    }
    return DW_DLV_NO_ENTRY;
}

static int read_dwarf_action_count_nonexternals(read_dwarf_ctx *rdc,
                                                Dwarf_Die die, int level)
{
    Dwarf_Half tag;
    Dwarf_Bool has_attr;

    (void)level;

    if (dwarf_tag(die, &tag, &rdc->err) == DW_DLV_OK &&
                                    tag == *((Dwarf_Half *)rdc->scratch_in))
    {
        if (dwarf_hasattr(die, DW_AT_external, &has_attr, &rdc->err) == DW_DLV_OK
                && !has_attr)
        {
            (*(unsigned *)rdc->scratch_out)++;
        }
    }
    return DW_DLV_NO_ENTRY;
}

/**
 * Copy the DIE into scratch_out (which should be pointing to an array of
 * \c Dwarf_Dies) if it is a variable with the tag and attribute specified in
 * scratch_in (see read_dwarf_action_count_tag_attr)
 * @param rdc read_dwarf context
 * @param die The DIE to examine
 * @param level The current recursion level (unused)
 * @return Return OK if DIE stored, else NO_ENTRY
 */
static int read_dwarf_action_store_tag_attr(read_dwarf_ctx *rdc,
                                                Dwarf_Die die, int level)
{
    Dwarf_Half tag, search_tag, search_attr;
    Dwarf_Bool has_attr;

    (void)level;

    search_tag = ((Dwarf_Half *)rdc->scratch_in)[0];
    search_attr = ((Dwarf_Half *)rdc->scratch_in)[1];

    if (search_tag == 0 ||
        (dwarf_tag(die, &tag, &rdc->err) == DW_DLV_OK && tag == search_tag))
    {
        if (search_attr == 0 ||
            (dwarf_hasattr(die, search_attr, &has_attr, &rdc->err) == DW_DLV_OK
                                                                && has_attr))
        {
            *(Dwarf_Die *)rdc->scratch_out = die;
            rdc->scratch_out = (void *)((Dwarf_Die *)rdc->scratch_out + 1);
            return DW_DLV_OK;
        }
    }
    return DW_DLV_NO_ENTRY;
}

/**
 * Copy the DIE into scratch_out (which should be pointing to an array of
 * \c Dwarf_Dies) if it is a variable with the external attribute
 * @param rdc read_dwarf context
 * @param die The DIE to examine
 * @param level The current recursion level (unused)
 * @return Return OK if DIE stored, else NO_ENTRY
 */
static int read_dwarf_action_store_nonexternals(read_dwarf_ctx *rdc,
                                                Dwarf_Die die, int level)
{
    Dwarf_Half tag;
    Dwarf_Bool has_attr;

    (void)level;

    if (dwarf_tag(die, &tag, &rdc->err) == DW_DLV_OK &&
                                        tag == *((Dwarf_Half *)rdc->scratch_in))
    {
        if (dwarf_hasattr(die, DW_AT_external, &has_attr, &rdc->err) == DW_DLV_OK
                && !has_attr)
        {
            *(Dwarf_Die *)rdc->scratch_out = die;
            rdc->scratch_out = (void *)((Dwarf_Die *)rdc->scratch_out + 1);
            return DW_DLV_OK;
        }
    }
    return DW_DLV_NO_ENTRY;
}

int read_dwarf_find_var_symbol_in_cu(read_dwarf_ctx *rdc,
                 Dwarf_Die cu_die,
                 const char *symbol_name,
                 Dwarf_Die *symbol_die)
{
    rdc->scratch_in = symbol_name;
    rdc->scratch_out = NULL;
    read_dwarf_traverse_die_tree(rdc, cu_die,
                     read_dwarf_action_find_var_symbol, 0, 0, FALSE, TRUE);

    /* If scratch_out is still NULL, we didn't find it */
    if ((const char *)rdc->scratch_out == NULL)
    {
        return DW_DLV_NO_ENTRY;
    }

    /* The traversal action function stores the DIE in malloc'd space pointed to
       by scratch */
    *symbol_die = *(Dwarf_Die *)rdc->scratch_out;
    free(rdc->scratch_out);
    return DW_DLV_OK;
}

int read_dwarf_find_symbol_in_cu(read_dwarf_ctx *rdc,
                 Dwarf_Die cu_die,
                 const char *symbol_name,
                 Dwarf_Die *symbol_die)
{
    rdc->scratch_in = symbol_name;
    rdc->scratch_out = NULL;
    read_dwarf_traverse_die_tree(rdc, cu_die,
                     read_dwarf_action_find_symbol, 0, 0, TRUE, TRUE);

    /* If scratch_out is still NULL, we didn't find it */
    if ((const char *)rdc->scratch_out == NULL)
    {
        return DW_DLV_NO_ENTRY;
    }

    /* The traversal action function stores the DIE in malloc'd space pointed to
       by scratch */
    *symbol_die = *(Dwarf_Die *)rdc->scratch_out;
    free(rdc->scratch_out);
    return DW_DLV_OK;
}


int read_dwarf_child(read_dwarf_ctx *rdc,
                     Dwarf_Die parent_die,
                     Dwarf_Die *child_die)
{
    int retval;
    Dwarf_Die tmp_child_die;
    retval = dwarf_child(parent_die, &tmp_child_die, &rdc->err);
    if (retval == DW_DLV_OK && child_die != NULL)
    {
        *child_die = tmp_child_die;
    }
    else if (retval == DW_DLV_ERROR)
    {
        _error(rdc, "Failed getting child DIE");
    }
    return retval;
}

int read_dwarf_siblingof(read_dwarf_ctx *rdc,
                         Dwarf_Die older_sibling,
                         Dwarf_Die *younger_sibling)
{
    int retval;
    Dwarf_Die tmp_sibling;
    retval = dwarf_siblingof(rdc->dbg, older_sibling,
                 &tmp_sibling, &rdc->err);
    if (retval == DW_DLV_OK && younger_sibling != NULL)
    {
        *younger_sibling = tmp_sibling;
    }
    else if (retval == DW_DLV_ERROR)
    {
        _error(rdc, "Failed getting sibling DIE");
    }
    return retval;
}

int read_dwarf_same_die(read_dwarf_ctx *rdc,
                               Dwarf_Die die1, Dwarf_Die die2)
{
    char *name1, *name2;

    if (read_dwarf_diename(rdc, die1, &name1) != DW_DLV_OK)
    {
        return 0;
    }
    if (read_dwarf_diename(rdc, die2, &name2) != DW_DLV_OK)
    {
        return 0;
    }
    return strcmp(name1, name2) == 0 ? 1 : 0;
}

int read_dwarf_tag(read_dwarf_ctx *rdc,
                   Dwarf_Die die,
                   Dwarf_Half *tag)
{
    if (dwarf_tag(die, tag, &rdc->err) != DW_DLV_OK)
    {
        _error(rdc, "Getting tag");
        return DW_DLV_ERROR;
    }
    return DW_DLV_OK;
}

int read_dwarf_has_attr(read_dwarf_ctx *rdc,
                               Dwarf_Die die,
                               Dwarf_Half attr_id,
                               Dwarf_Bool *has_attr,
                               Dwarf_Half *form,
                               Dwarf_Attribute *attr_desc)
{
    if (dwarf_hasattr(die, attr_id, has_attr, &rdc->err) != DW_DLV_OK)
    {
        _error(rdc, "Checking for attribute");
        return DW_DLV_ERROR;
    }

    if (*has_attr)
    {
        if (attr_desc != NULL || form != NULL)
        {
            Dwarf_Attribute val_attr;
            /* Grab the type attribute descriptor */
            if (dwarf_attr(die, attr_id, &val_attr, &rdc->err) != DW_DLV_OK)
            {
                return DW_DLV_NO_ENTRY;
            }

            if (form != NULL)
            {
                /* Get the attribute's form */
                if (dwarf_whatform(val_attr, form, &rdc->err) != DW_DLV_OK)
                {
                    _error(rdc, "Failed getting form of attribute!");
                    return DW_DLV_ERROR;
                }
            }
            if (attr_desc != NULL)
            {
                *attr_desc = val_attr;
            }
        }
    }
    return DW_DLV_OK;
}

void read_dwarf_free_attr(read_dwarf_ctx *rdc, Dwarf_Attribute attr)
{
    dwarf_dealloc(rdc->dbg, attr, DW_DLA_ATTR);
}

void read_dwarf_free_die(read_dwarf_ctx *rdc, Dwarf_Die die)
{
    if (die != 0)
    {
        dwarf_dealloc(rdc->dbg, die, DW_DLA_DIE);
    }
}

int read_dwarf_get_unsigned_attr(read_dwarf_ctx *rdc,
                                 Dwarf_Attribute attr,
                                 Dwarf_Unsigned *value)
{
    if (dwarf_formudata(attr, value, &rdc->err) != DW_DLV_OK)
    {
        _error(rdc, "Failed getting value of unsigned attribute");
        return DW_DLV_ERROR;
    }
    return DW_DLV_OK;
}

int read_dwarf_get_addr_attr(read_dwarf_ctx *rdc,
                                 Dwarf_Attribute attr,
                                 Dwarf_Addr *value)
{
    if (dwarf_formaddr(attr, value, &rdc->err) != DW_DLV_OK)
    {
        _error(rdc, "Failed getting value of address attribute");
        return DW_DLV_ERROR;
    }
    return DW_DLV_OK;
}

int read_dwarf_get_int_attr(read_dwarf_ctx *rdc,
                            Dwarf_Attribute attr,
                            Dwarf_Bool form_signed,
                            Dwarf_Signed *value)
{
    Dwarf_Unsigned uvalue;
    int ret;

    if (form_signed)
    {
        if (dwarf_formsdata(attr, value, &rdc->err) != DW_DLV_OK)
        {
          _error(rdc, "Failed getting value of signed attribute");
          return DW_DLV_ERROR;
        }
    }
    else
    {
        ret = read_dwarf_get_unsigned_attr(rdc, attr, &uvalue);
        if (ret != DW_DLV_OK)
        {
            return ret;
        }
        if (value != NULL)
        {
            *value = (Dwarf_Signed)uvalue;
        }
    }
    return DW_DLV_OK;
}

int read_dwarf_get_string_attr(read_dwarf_ctx *rdc,
                               Dwarf_Attribute attr,
                               const char **value)
{
    if (dwarf_formstring(attr, value, &rdc->err) != DW_DLV_OK)
    {
        _error(rdc, "Failed getting value of string attribute!");
        return DW_DLV_ERROR;
    }
    return DW_DLV_OK;
}

int read_dwarf_get_ref_attr(read_dwarf_ctx *rdc,
                            Dwarf_Attribute attr,
                            int as_global,
                            Dwarf_Off *offset)
{
    Dwarf_Off the_offset;
    if (dwarf_formref(attr, &the_offset, &rdc->err) != DW_DLV_OK)
    {
        _error(rdc, "Failed getting value of reference attribute!");
        return DW_DLV_ERROR;
    }
    if (as_global)
    {
        Dwarf_Off gbl_offset;
        if (dwarf_convert_to_global_offset(attr, the_offset, &gbl_offset, &rdc->err) !=DW_DLV_OK)
        {
            _error(rdc, "Error getting offset of CU containing concrete instance DIE");
            return DW_DLV_ERROR;
        }
        the_offset = gbl_offset;
    }
    *offset = the_offset;
    return DW_DLV_OK;
}

int read_dwarf_get_gbl_ref_attr(read_dwarf_ctx *rdc,
                                Dwarf_Attribute attr,
                                Dwarf_Off *offset)
{
    if (dwarf_global_formref(attr, offset, &rdc->err) != DW_DLV_OK)
    {
        _error(rdc, "Failed getting value of reference attribute!");
        return DW_DLV_ERROR;
    }
    return DW_DLV_OK;
}

int read_dwarf_abstract_origin(read_dwarf_ctx *rdc,
                               Dwarf_Die concrete_die,
                               Dwarf_Die *abstract_die)
{
    /* Try getting the name from the abstract origin DIE,
     * if that exists */
    Dwarf_Bool has_attr;
    Dwarf_Half form;
    Dwarf_Attribute attr;
    Dwarf_Off offset = 0;

    if (read_dwarf_has_attr(rdc, concrete_die, DW_AT_abstract_origin,
            &has_attr, &form, &attr) != DW_DLV_OK)
    {
        return DW_DLV_ERROR;
    }
    if (!has_attr)
    {
        return DW_DLV_NO_ENTRY;
    }
    switch (form)
    {
    case DW_FORM_ref1:
    case DW_FORM_ref2:
    case DW_FORM_ref4:
    case DW_FORM_ref8:
    case DW_FORM_ref_udata:
        if (read_dwarf_get_ref_attr(rdc, attr, TRUE, &offset) != DW_DLV_OK)
        {
            read_dwarf_free_attr(rdc, attr);
            return DW_DLV_ERROR;
        }
        read_dwarf_free_attr(rdc, attr);
        break;
    case DW_FORM_ref_addr:
        if (read_dwarf_get_gbl_ref_attr(rdc, attr, &offset) != DW_DLV_OK)
        {
            read_dwarf_free_attr(rdc, attr);
            return DW_DLV_ERROR;
        }
        read_dwarf_free_attr(rdc, attr);
        break;
    default:
        read_dwarf_free_attr(rdc, attr);
        {
            char msg[64];
            sprintf(msg, "Abstract origin attr had unexpected form 0x%x\n", form);
            _warning(rdc, msg);
        }
        return DW_DLV_NO_ENTRY;
    }
    /* We've now got the offset for the abstract DIE, so look it up */
    switch (read_dwarf_offdie(rdc, offset, abstract_die))
    {
    case DW_DLV_ERROR:
        _error(rdc, "Error retrieving DIE at abstract_origin offset!");
        return DW_DLV_ERROR;
    case DW_DLV_NO_ENTRY:
    {
        char msg[128];
        sprintf(msg, "Couldn't retrieve a DIE at abstract_origin offset 0x%lx", (unsigned long)offset);
        _info(rdc, msg);
        return DW_DLV_NO_ENTRY;
    }
    default:
        break;
    }
    return DW_DLV_OK;
}


int read_dwarf_enumerator_value(read_dwarf_ctx *rdc,
                                Dwarf_Die etr_die,
                                Dwarf_Signed *value)
{
    Dwarf_Attribute val_attr;
    Dwarf_Half form;
    Dwarf_Unsigned uvalue;

    /* Grab the type attribute descriptor */
    if (dwarf_attr(etr_die, DW_AT_const_value, &val_attr, &rdc->err) != DW_DLV_OK)
    {
        return DW_DLV_NO_ENTRY;
    }

    /* Get the attribute's form, and then make an appropriate call to get
       its value */
    if (dwarf_whatform(val_attr, &form, &rdc->err) != DW_DLV_OK)
    {
        read_dwarf_free_attr(rdc, val_attr);
        _error(rdc, "Failed getting form of enumerator value attribute!");
        return DW_DLV_ERROR;
    }

    /* Check the form is what we expect and get the value */
    switch(form)
    {
    case DW_FORM_data1:
    case DW_FORM_data2:
    case DW_FORM_data4:
    case DW_FORM_data8:
    case DW_FORM_udata:
        if (dwarf_formudata(val_attr, &uvalue, &rdc->err) != DW_DLV_OK)
        {
            read_dwarf_free_attr(rdc, val_attr);
            _error(rdc, "Failed getting value of enumerator");
            return DW_DLV_ERROR;
        }
        if (value != NULL)
        {
            *value = (Dwarf_Signed)uvalue;
        }
        break;
    case DW_FORM_sdata:
        if (dwarf_formsdata(val_attr, value, &rdc->err) != DW_DLV_OK)
        {
            read_dwarf_free_attr(rdc, val_attr);
          _error(rdc, "Failed getting value of enumerator");
          return DW_DLV_ERROR;
        }
        break;

    default:
        read_dwarf_free_attr(rdc, val_attr);
        printf("Unexpected form 0x%02x for enumerator value attribute!", form);
        return DW_DLV_ERROR;
    }

    read_dwarf_free_attr(rdc, val_attr);
    return DW_DLV_OK;
}


int read_dwarf_get_immediate_type(read_dwarf_ctx *rdc,
                                  Dwarf_Die source_die,
                                  Dwarf_Die *target_die)
{
    Dwarf_Attribute type;
    Dwarf_Half form;
    Dwarf_Off type_offset;
    Dwarf_Die tmp_target_die;


    /* Grab the type attribute descriptor */
    if (dwarf_attr(source_die, DW_AT_type, &type, &rdc->err) != DW_DLV_OK)
    {
        return DW_DLV_NO_ENTRY;
    }

    /* Get the attribute's form, and then make an appropriate call to get
       its value */
    if (dwarf_whatform(type, &form, &rdc->err) != DW_DLV_OK)
    {
        _error(rdc, "Failed getting form of type attribute!");
        return DW_DLV_ERROR;
    }

    /* Check the form is what we expect */
    if (form < DW_FORM_ref_addr || form > DW_FORM_ref_udata)
    {
        read_dwarf_free_attr(rdc, type);
        _error(rdc, "Unexpected type attribute form: expected a DW_FORM_ref*");
        return DW_DLV_ERROR;
    }

    /* Get the global offset of the type die */
    if (dwarf_global_formref(type, &type_offset, &rdc->err) != DW_DLV_OK)
    {
        read_dwarf_free_attr(rdc, type);
        _error(rdc, "Failed to get type attribute offset!");
        return DW_DLV_ERROR;
    }

    read_dwarf_free_attr(rdc, type);

    /* Get the DIE given its global offset */
    if (dwarf_offdie(rdc->dbg, type_offset, &tmp_target_die,
             &rdc->err) != DW_DLV_OK)
    {
        _error(rdc, "Failed to find type DIE given global offset");
        return DW_DLV_ERROR;
    }

    /* target_die can't be NULL in this case */
    *target_die = tmp_target_die;

    return DW_DLV_OK;
}

void read_dwarf_mem_stats(read_dwarf_ctx *rdc)
{
    dwarf_print_memory_stats(rdc->dbg);
}

int read_dwarf_get_array_size(read_dwarf_ctx *rdc,
                              Dwarf_Die array_type_die,
                              Dwarf_Unsigned *length)
{
    Dwarf_Die subranges_die;
    Dwarf_Half tag, form;
    Dwarf_Attribute attr_desc;

    /* The array_type's child DIE is a DIE of "subrange" type which gives us the
     * upper bound */
    switch (read_dwarf_child(rdc, array_type_die, &subranges_die))
    {
    case DW_DLV_NO_ENTRY:
        printf("Didn't find a child DIE of the array type DIE\n");
        return DW_DLV_ERROR;
    case DW_DLV_ERROR:
        return DW_DLV_ERROR;
    default:
        break;
    }

    /* Check this is indeed a subrange_type DIE */
    if (read_dwarf_tag(rdc, subranges_die, &tag) != DW_DLV_OK)
    {
        printf("Error getting tag of array type's child DIE\n");
        return DW_DLV_ERROR;
    }

    if (tag != DW_TAG_subrange_type)
    {
        printf("Unexpected array type child DIE tag: 0x%02x", tag);
        return DW_DLV_ERROR;
    }

    /* Grab the upper_bound attribute */
    switch (dwarf_attr(subranges_die, DW_AT_upper_bound, &attr_desc, &rdc->err))
    {
    case DW_DLV_NO_ENTRY:
        _warning(rdc, "Didn't find upper bound attr in subranges DIE\n");
        return DW_DLV_NO_ENTRY;
    case DW_DLV_ERROR:
        printf("Error retrieving upper bound attr in subranges DIE\n");
        return DW_DLV_ERROR;
    default:
        break;
    }

    if (dwarf_whatform(attr_desc, &form, &rdc->err) != DW_DLV_OK)
    {
        read_dwarf_free_attr(rdc, attr_desc);
        _error(rdc, "Failed getting form of byte_size attribute!");
        return DW_DLV_ERROR;
    }

    switch(form)
    {
    case DW_FORM_data1:
    case DW_FORM_data2:
    case DW_FORM_data4:
    case DW_FORM_data8:
    case DW_FORM_udata:
        if (dwarf_formudata(attr_desc, length, &rdc->err) != DW_DLV_OK)
        {
            read_dwarf_free_attr(rdc, attr_desc);
          _error(rdc, "Failed getting value of byte_size attribute");
          return DW_DLV_ERROR;
        }
        /* The attribute is actually the upper bound of the array, so the length
         * is one greater. */
        (*length)++;
        break;

    default:
        read_dwarf_free_attr(rdc, attr_desc);
        printf("Unexpected form 0x%02x for byte_size attribute!", form);
        return DW_DLV_ERROR;
    }

    read_dwarf_free_attr(rdc, attr_desc);
    return DW_DLV_OK;
}




int read_dwarf_get_byte_size(read_dwarf_ctx *rdc, Dwarf_Die die)
{
    Dwarf_Attribute attr_desc;
    Dwarf_Unsigned value;
    Dwarf_Half form;

    /* Get the attribute */
    switch(dwarf_attr(die, DW_AT_byte_size, &attr_desc, &rdc->err))
    {
    case DW_DLV_NO_ENTRY:
        return READ_DWARF_INT_RETURN_NO_ENTRY;
    case DW_DLV_ERROR:
        return READ_DWARF_INT_RETURN_ERROR;
    default:
        break;
    }

    /* Get the attribute's form, and then make an appropriate call to get
       its value */
    if (dwarf_whatform(attr_desc, &form, &rdc->err) != DW_DLV_OK)
    {
        read_dwarf_free_attr(rdc, attr_desc);
        _error(rdc, "Failed getting form of byte_size attribute!");
        return READ_DWARF_INT_RETURN_ERROR;
    }


    switch(form)
    {
    case DW_FORM_data1:
    case DW_FORM_data2:
    case DW_FORM_data4:
    case DW_FORM_data8:
    case DW_FORM_udata:
        if (dwarf_formudata(attr_desc, &value, &rdc->err) != DW_DLV_OK)
        {
            read_dwarf_free_attr(rdc, attr_desc);
          _error(rdc, "Failed getting value of byte_size attribute");
          return READ_DWARF_INT_RETURN_ERROR;
        }
        break;

    default:
        read_dwarf_free_attr(rdc, attr_desc);
        printf("Unexpected form 0x%02x for byte_size attribute!", form);
        return READ_DWARF_INT_RETURN_ERROR;
    }

    read_dwarf_free_attr(rdc, attr_desc);
    return (int)value;
}

int read_dwarf_get_member_offset(read_dwarf_ctx *rdc,
                                 Dwarf_Die member_die,
                                 Dwarf_Unsigned *member_offset)
{
    Dwarf_Attribute attr_desc;
    Dwarf_Unsigned value;
    Dwarf_Half form;
    Dwarf_Block *block;
    Dwarf_Small opcode;

    /* Get the attribute */
    switch(dwarf_attr(member_die, DW_AT_data_member_location,
                      &attr_desc, &rdc->err))
    {
    case DW_DLV_NO_ENTRY:
        return DW_DLV_NO_ENTRY;
    case DW_DLV_ERROR:
        _error(rdc, "Failed getting data_member_location attribute!");
        return DW_DLV_ERROR;
    default:
        break;
    }

    /* Get the attribute's form, and then make an appropriate call to get
       its value */
    if (dwarf_whatform(attr_desc, &form, &rdc->err) != DW_DLV_OK)
    {
        read_dwarf_free_attr(rdc, attr_desc);
        _error(rdc, "Failed getting form of data_member_location attribute!");
        return DW_DLV_ERROR;
    }

    switch(form)
    {
    case DW_FORM_data1:
    case DW_FORM_data2:
    case DW_FORM_data4:
    case DW_FORM_data8:
    case DW_FORM_udata:
        if (dwarf_formudata(attr_desc, &value, &rdc->err) != DW_DLV_OK)
        {
            read_dwarf_free_attr(rdc, attr_desc);
          _error(rdc, "Failed getting value of data_member_location attribute");
          return DW_DLV_ERROR;
        }
        break;

    case DW_FORM_block:
    case DW_FORM_block1:
    case DW_FORM_block2:
    case DW_FORM_block4:
        if (dwarf_formblock(attr_desc, &block, &rdc->err) != DW_DLV_OK)
        {
            read_dwarf_free_attr(rdc, attr_desc);
            _error(rdc, "Failed get block data for data_member_location attribute");
            return DW_DLV_ERROR;
        }
        /* The block contains a location stack expression in general, but
         * probably all we need to read is an unsigned LEB128 offset.  We'll
         * just bomb out if that's not the opcode we've got.  */
        opcode = *(Dwarf_Small *)block->bl_data;

        if (opcode != DW_OP_plus_uconst)
        {
            char msg[128];
            read_dwarf_free_attr(rdc, attr_desc);
            sprintf(msg,"Unexpected opcode 0x%02x for member offset!", opcode);
            _error(rdc, msg);
            return DW_DLV_ERROR;
        }

        value = decode_leb128u((Dwarf_Small *)block->bl_data + 1);

        dwarf_dealloc(rdc->dbg, block, DW_DLA_BLOCK);
        break;

    default:
        read_dwarf_free_attr(rdc, attr_desc);
        printf("Unexpected form 0x%02x for data_member_location attribute!", form);
        return DW_DLV_ERROR;
    }

    read_dwarf_free_attr(rdc, attr_desc);

    if (member_offset != NULL)
    {
        *member_offset = value;
    }

    return DW_DLV_OK;

}

int read_dwarf_diename(read_dwarf_ctx *rdc, Dwarf_Die die, char **name)
{
    int ret = dwarf_diename(die, name, &rdc->err);
    switch(ret)
    {
    case DW_DLV_ERROR:
        _error(rdc, "Error getting DIE name");
        break;
    default:
        break;
    }
    return ret;
}

char * read_dwarf_cu_name(read_dwarf_ctx *rdc, Dwarf_Die cu_die, int full_name)
{
    char *name;
    const char *comp_dir = "";
    char *name_ret = NULL;
    char sep_char = 0;
    Dwarf_Bool has_attr;
    Dwarf_Half form;
    Dwarf_Attribute attr;
    size_t name_len, comp_dir_len;
    int cat_with_sep;

    /* The name attribute typically contains the path of the source file relative
     * to the directory it was compiled from, but not necessarily. */
    if (read_dwarf_diename(rdc, cu_die, &name) != DW_DLV_OK)
    {
        _error(rdc, "Error getting CU DIE name");
        return NULL;
    }

    /* Take a leading slash (Unix) or a leading drive letter (Windows) to
     * indicate an absolute path */
    if (strlen(name) > 0 && (name[0] == '/' || name[1] == ':'))
    {
        /* the name attribute is absolute so don't try to prepend anything */
        full_name = FALSE;
    }

    if (full_name)
    {
        /* Try to construct the full name by prepending the compilation
         * directory, if we can get hold of it */
        switch(read_dwarf_has_attr(rdc, cu_die, DW_AT_comp_dir, &has_attr, &form,
                                    &attr))
        {
        case DW_DLV_ERROR:
            _error(rdc, "Error looking for compdir attribute");
            return NULL;
        case DW_DLV_NO_ENTRY:
            comp_dir = "";
            break;
        default:
            if (!has_attr)
            {
                comp_dir = "";
            }
            else
            {
                int ret = read_dwarf_get_string_attr(rdc, attr, &comp_dir);
                size_t i;
                read_dwarf_free_attr(rdc, attr);
                if (ret != DW_DLV_OK)
                {
                    _error(rdc, "Error retrieving compdir attribute");
                    return NULL;
                }
                /* Find the first separator in the compilation dir path: we'll
                 * use that as the separator for joining the paths */
                for (i = 0; i < strlen(comp_dir) && sep_char == 0; ++i)
                {
                    switch(comp_dir[i])
                    {
                    case '/':
                    case '\\':
                        sep_char = comp_dir[i];
                        break;
                    default:
                        break;
                    }
                }
                /* If there was no separator in the compilation dir, pick '/' by
                 * default */
                if (sep_char == 0)
                {
                    sep_char = '/';
                }
            }
        }
    }
    /* Now allocate space to return the combined strings and return them*/
    name_len = strlen(name);
    comp_dir_len = strlen(comp_dir);
    /* We need to add a separator when we join the strings so long as there is
     * something to join and it doesn't end with a separator already */
    cat_with_sep = (comp_dir_len > 0 && comp_dir[comp_dir_len-1] != sep_char);
    if (cat_with_sep)
    {
        comp_dir_len++;
    }

    name_ret = malloc(name_len + comp_dir_len + 1);
    if (name_ret == NULL)
    {
        char msg[128];
        sprintf(msg, "Failed to malloc %ld bytes for full compilation unit name",
                (unsigned long)(name_len + comp_dir_len + 1));
        _error(rdc, msg);
        return NULL;
    }
    /* Write the different parts of the name into the returned string */
    strcpy(name_ret,comp_dir);
    if (cat_with_sep)
    {
        name_ret[comp_dir_len-1] = sep_char;
    }
    strcpy(name_ret+comp_dir_len, name);
    return name_ret;
}

Dwarf_Off read_dwarf_die_offset(read_dwarf_ctx *rdc, Dwarf_Die die)
{
    Dwarf_Off ret_offset;
    int ret = dwarf_dieoffset(die, &ret_offset, &rdc->err);
    if (ret != DW_DLV_OK)
    {
        return READ_DWARF_INVALID_DIE_OFFSET;
    }
    return ret_offset;

}

int read_dwarf_offdie(read_dwarf_ctx *rdc, Dwarf_Off offset, Dwarf_Die *die)
{
    return dwarf_offdie(rdc->dbg, offset, die, &rdc->err);
}

int read_dwarf_resolve_type(read_dwarf_ctx *rdc, Dwarf_Die die,
                            char **decl_name,
                            Dwarf_Bool *is_typedef_name,
                            Dwarf_Die *resolved_type,
                            Dwarf_Half *resolved_type_tag,
                            unsigned *qualifiers,
                            Dwarf_Unsigned *array_length)
{
    Dwarf_Half tag;
    Dwarf_Die type_die, prev_type_die;
    int type_resolved = FALSE;
    int need_name = FALSE;

    if (qualifiers != NULL)
    {
        *qualifiers = 0;
    }

    if (decl_name != NULL)
    {
        /* Set decl_name to point to NULL in case the name is never set (because
         * we get a resolved type that needs further resolution) */
        *decl_name = NULL;
        need_name = TRUE;
    }

    type_die = die;

    while (!type_resolved)
    {
        if (read_dwarf_tag(rdc, type_die, &tag) != DW_DLV_OK)
        {
            _error(rdc, "Error getting type tag");
            return DW_DLV_ERROR;
        }

        switch(tag)
        {
        /* Just add to the list of qualifiers and go round again */
        case DW_TAG_volatile_type:
            if (qualifiers != NULL)
            {
                *qualifiers |= READ_DWARF_TYPE_QUAL_VOLATILE_MASK;
            }
            break;
        case DW_TAG_const_type:
            if (qualifiers != NULL)
            {
                *qualifiers |= READ_DWARF_TYPE_QUAL_CONST_MASK;
            }
            break;

        /* If we hit a pointer or an array, we've resolved the type, although
         * we don't have a name */
        case DW_TAG_array_type:
            /* Also get the length */
            if(array_length != NULL)
            {
                switch(read_dwarf_get_array_size(rdc, type_die, array_length))
                {
                case DW_DLV_ERROR:
                    return DW_DLV_ERROR;
                case DW_DLV_NO_ENTRY:
                    *array_length = READ_DWARF_NO_ARRAY_LENGTH;
                    break;
                default:
                    break;
                }
            }
            /* FALLTHROUGH */
        case DW_TAG_pointer_type:
            type_resolved = TRUE;
            if (resolved_type != NULL)
            {
                *resolved_type = type_die;
            }
            if (resolved_type_tag != NULL)
            {
                *resolved_type_tag = tag;
            }
            break;

        /* If we hit a real type or a typedef and we don't already have a name,
         * we get the name; if we hit a real type, we also drop out because
         * we're done.*/
        case DW_TAG_subroutine_type:
            need_name = FALSE;
            /* FALLTHROUGH */
        case DW_TAG_base_type:
        case DW_TAG_structure_type:
        case DW_TAG_union_type:
        case DW_TAG_enumeration_type:
        case DW_TAG_class_type:
            type_resolved = TRUE;
            if (resolved_type != NULL)
            {
                *resolved_type = type_die;
            }
            if (resolved_type_tag != NULL)
            {
                *resolved_type_tag = tag;
            }
            /* FALLTHROUGH */
        case DW_TAG_typedef:
            if (need_name)
            {
                /* This is the name  we want: the first-encountered typedef */
                switch (read_dwarf_diename(rdc, type_die, decl_name))
                {
                case DW_DLV_NO_ENTRY:
                    /* Anonymous type */
                    *decl_name = "<anonymous>";
                    break;
                case DW_DLV_ERROR:
                    _error(rdc, "Failed to get declared name of variable's type");
                    return DW_DLV_ERROR;
                default:
                    break;
                }
                need_name = FALSE;
                if (is_typedef_name != NULL)
                {
                    *is_typedef_name = !type_resolved;
                }
            }
            break;
        case DW_TAG_unspecified_type:
            break;
        default:
            printf("Unexpected tag 0x%02x in variable's type hierarchy\n", tag);
            return DW_DLV_ERROR;
        }

        if (!type_resolved)
        {
            /* Go round the loop again */
            prev_type_die = type_die;
            switch (read_dwarf_get_immediate_type(rdc,
                                              prev_type_die,
                                              &type_die))
            {
            case DW_DLV_ERROR:
                _error(rdc, "Error getting immediate type");
                return DW_DLV_ERROR;
            case DW_DLV_NO_ENTRY:
                /* We've resolved down to "void" */
                if (need_name)
                {
                    *decl_name = "void";
                }
                if (resolved_type_tag != NULL)
                {
                    *resolved_type_tag = (Dwarf_Half)0;
                }
                return DW_DLV_NO_ENTRY;
            }
        }
    }

    return DW_DLV_OK;
}


char *read_dwarf_var_typename(read_dwarf_ctx *rdc, Dwarf_Die die)
{
    Dwarf_Die type_die;

    /*1. Get immediate type of variable DIE. */
    if (read_dwarf_get_immediate_type(rdc, die, &type_die) != DW_DLV_OK)
    {
        _error(rdc, "Couldn't get type DIE of variable DIE");
        return NULL;
    }
    return read_dwarf_typename(rdc, type_die);
}

char *read_dwarf_typename(read_dwarf_ctx *rdc, Dwarf_Die type_die)
{
    Dwarf_Die resolved_type;
    Dwarf_Half resolved_type_tag;
    char *name, *returned_name;
    char tmp_name[256];
    int cur_name_start = 255;
    size_t frag_len = 0;
    unsigned qualifiers = 0;
    Dwarf_Off array_length;
    int fully_constructed = FALSE;
    Dwarf_Bool is_typedef_name = FALSE;

    /*2. Resolve this type.  If the resolution is complete, construct the
     * string.  If not, resolve again, and keep going until the typename can
     * be fully constructed.
     */

    tmp_name[cur_name_start] = 0;
    while(!fully_constructed)
    {
        switch(read_dwarf_resolve_type(rdc, type_die, &name, &is_typedef_name,
                                       &resolved_type,
                                       &resolved_type_tag, &qualifiers,
                                       &array_length))
        {
        case DW_DLV_NO_ENTRY:
            returned_name = (char *)malloc(strlen("<anonymous>")+1);
            if (returned_name == NULL)
            {
                return NULL;
            }
            sprintf(returned_name,"<anonymous>");
            return returned_name;

        case DW_DLV_ERROR:
            _error(rdc, "Error getting variable's type details");
            return NULL;
        default:
            break;
        }

        read_dwarf_construct_typename_fragment(rdc, name, is_typedef_name,
                                               qualifiers,
                                               resolved_type_tag,
                                               array_length,
                                               tmp_name+cur_name_start,
                                               &frag_len);
        cur_name_start -= (unsigned)frag_len;
        assert(cur_name_start >= 0);

        if (resolved_type_tag != DW_TAG_pointer_type &&
            resolved_type_tag != DW_TAG_array_type)
        {
            fully_constructed = TRUE;
        }
        else
        {
            Dwarf_Die underlying_type_die;
            if (read_dwarf_get_immediate_type(rdc, type_die,
                                              &underlying_type_die) != DW_DLV_OK)
            {
                /* It's a void pointer */
                cur_name_start -= 4;
                assert(cur_name_start >= 0);
                memcpy(tmp_name+cur_name_start,"void",4);
                fully_constructed = TRUE;
            }
            type_die = underlying_type_die;
        }

    }

    /* Now copy into a malloc'd string for returning */
    returned_name = (char *)malloc(256 - cur_name_start);
    if (returned_name == NULL)
    {
        return NULL;
    }
    memcpy(returned_name, tmp_name+cur_name_start, 255 - cur_name_start);
    returned_name[255 - cur_name_start] = (char)0;
    return returned_name;
}

const char * read_dwarf_typename_prefix(Dwarf_Half tag)
{
    const char *prefix;
    switch(tag)
    {
    case DW_TAG_structure_type:
        prefix = "struct ";
        break;
    case DW_TAG_enumeration_type:
        prefix = "enum ";
        break;
    case DW_TAG_union_type:
        prefix = "union ";
        break;
    default:
        prefix = "";
        break;
    }
    return prefix;
}

    /* Construct a full or partial name string from all the qualifiers and the
     * declared name */
void read_dwarf_construct_typename_fragment(read_dwarf_ctx *rdc,
                                            const char *name,
                                            Dwarf_Bool is_typedef_name,
                                            unsigned qualifiers,
                                            Dwarf_Half tag,
                                            Dwarf_Unsigned array_length,
                                            char *name_start,
                                            size_t *frag_len)
{
    assert(frag_len != NULL);

    *frag_len = 0;

    /* If we have a named type we ignore pointer/array-ness, because the way
     * read_dwarf_resolve_type works, if the name is set as well as one of those
     * attributes the name is a typedef which implies the attribute, so we
     * shouldn't explicitly add it to the name.  */
    if (name != NULL)
    {
        unsigned len_name = (unsigned)strlen(name);
        *frag_len += len_name;
        name_start -= len_name;
        memcpy(name_start, name, len_name);
        if (!is_typedef_name)
        {
            const char * prefix;
            size_t prefix_len;
            prefix = read_dwarf_typename_prefix(tag);
            prefix_len = strlen(prefix);
            *frag_len += prefix_len;
            name_start -= prefix_len;
            memcpy(name_start, prefix, prefix_len);
        }
    }
    else if (tag == DW_TAG_pointer_type)
    {
        *frag_len += 2 /*strlen(" *")*/;
        name_start -= 2;
        memcpy(name_start, " *", 2);
    }
    else if (tag == DW_TAG_array_type)
    {
        char numstr[8];
        unsigned len_numstr;
        if (array_length != READ_DWARF_NO_ARRAY_LENGTH)
        {
            sprintf(numstr,"[%u]",(unsigned)array_length);
        }
        else
        {
            sprintf(numstr,"[]");
        }
        len_numstr = (unsigned)strlen(numstr);
        name_start -= len_numstr;
        *frag_len += len_numstr;
        memcpy(name_start, numstr, len_numstr);
    }


    /* Then the qualifiers go on the left */
    if (qualifiers & READ_DWARF_TYPE_QUAL_CONST_MASK)
    {
        *frag_len += 6 /*strlen("const ")*/;
        name_start -= 6;
        memcpy(name_start, "const ", 6);
    }
    if (qualifiers & READ_DWARF_TYPE_QUAL_VOLATILE_MASK)
    {
        *frag_len += 9 /*strlen("volatile ")*/;
        name_start -= 9;
        memcpy(name_start, "volatile ", 9);
    }
}

int read_dwarf_get_low_pc(read_dwarf_ctx *rdc, Dwarf_Die func_or_cu_die,
                          Dwarf_Addr *low_pc)
{
    Dwarf_Attribute low_pc_attr;
    Dwarf_Half form;
    Dwarf_Addr *ranges;
    unsigned num_ranges;

    /* Grab the type attribute descriptor */
    if (dwarf_attr(func_or_cu_die, DW_AT_low_pc, &low_pc_attr, &rdc->err) == DW_DLV_OK ||
            dwarf_attr(func_or_cu_die, DW_AT_entry_pc, &low_pc_attr, &rdc->err) == DW_DLV_OK)
    {

        /* Get the attribute's form, and then make an appropriate call to get
           its value */
        if (dwarf_whatform(low_pc_attr, &form, &rdc->err) != DW_DLV_OK)
        {
            read_dwarf_free_attr(rdc, low_pc_attr);
            _error(rdc, "Failed getting form of low PC attribute!");
            return DW_DLV_ERROR;
        }

        /* Check the form is what we expect and get the value */
        switch(form)
        {
        case DW_FORM_addr:
            break;
        default:
            {
                char error_string[256];
                sprintf(error_string, "Low PC attribute did not have form "
                                      "addr (0x%02x)", form);
                _report(rdc, READ_DWARF_VERBOSITY_ERRORS, error_string);
            }
            read_dwarf_free_attr(rdc, low_pc_attr);
            return DW_DLV_ERROR;
        }

        if (dwarf_formaddr(low_pc_attr, low_pc, &rdc->err) != DW_DLV_OK)
        {
            read_dwarf_free_attr(rdc, low_pc_attr);
            _error(rdc, "Failed getting value of low PC attribute");
            return DW_DLV_ERROR;
        }

        read_dwarf_free_attr(rdc, low_pc_attr);
        return DW_DLV_OK;
    }
    else if (read_dwarf_get_ranges(rdc, func_or_cu_die, &ranges, &num_ranges) == DW_DLV_OK &&
            num_ranges > 0)
    {
        if (low_pc)
        {
            *low_pc = ranges[0];
        }
        return DW_DLV_OK;
    }
    else
    {
        char msg[256];
        char *name;
        strcpy(msg, "Couldn't find the low pc of ");
        if (read_dwarf_diename(rdc, func_or_cu_die, &name) == DW_DLV_OK)
        {
            assert(strlen(msg)+strlen(name)+1 < sizeof(msg));
            strcpy(msg+strlen(msg), name);
        }
        else
        {
            strcpy(msg+strlen(msg), "<unnamed function>");
        }
        _warning(rdc, msg);
        return DW_DLV_NO_ENTRY;
    }
}

int read_dwarf_get_die_cu_start_addr(read_dwarf_ctx *rdc,
                                     Dwarf_Die func_die,
                                     Dwarf_Addr *cu_start_addr)
{
    Dwarf_Off cu_offset;
    Dwarf_Die cu_die;

    if (dwarf_CU_dieoffset_given_die(func_die, &cu_offset, &rdc->err) != DW_DLV_OK)
    {
        _error(rdc, "Error getting CU DIE offset from function DIE");
        return DW_DLV_ERROR;
    }
    if (dwarf_offdie(rdc->dbg, cu_offset, &cu_die, &rdc->err) != DW_DLV_OK)
    {
        _error(rdc, "Error getting CU DIE from offset");
        return DW_DLV_ERROR;
    }
    if (read_dwarf_get_low_pc(rdc, cu_die, cu_start_addr) != DW_DLV_OK)
    {
        _error(rdc, "Error getting low_pc attribute of CU die\n");
        return DW_DLV_ERROR;
    }
    return DW_DLV_OK;
}

int read_dwarf_get_ranges(read_dwarf_ctx *rdc,
                          Dwarf_Die func_die,
                          Dwarf_Addr **ranges,
                          unsigned *num_ranges)
{
    Dwarf_Off ranges_offset;
    Dwarf_Unsigned ranges_offset_data;
    Dwarf_Ranges *ranges_buf, *cur_range_entry;
    Dwarf_Signed list_len;
    Dwarf_Bool has_attr;
    Dwarf_Half form;
    Dwarf_Attribute attr;
    Dwarf_Addr die_cu_start_addr, current_range_base, *cur_addr_entry;
    const Dwarf_Addr CURRENT_RANGE_BASE_NOT_SET = 0xffffffff;

    /* First get the range attributes - this is an offset */
    if (read_dwarf_has_attr(rdc, func_die, DW_AT_ranges, &has_attr, &form,
                                &attr) != DW_DLV_OK || !has_attr)
    {
        return DW_DLV_NO_ENTRY;
    }
    switch(form)
    {
    case DW_FORM_data2:
    case DW_FORM_data4:
    case DW_FORM_data8:
    case DW_FORM_udata:
        if (dwarf_formudata(attr, &ranges_offset_data, &rdc->err) != DW_DLV_OK)
        {
            read_dwarf_free_attr(rdc, attr);
            _error(rdc, "Error getting value of ranges offset in .debug_ranges");
        }
        ranges_offset = ranges_offset_data;
        break;
    case DW_FORM_sec_offset:
        if (dwarf_global_formref(attr, &ranges_offset, &rdc->err) != DW_DLV_OK)
        {
            read_dwarf_free_attr(rdc, attr);
            _error(rdc, "Error getting value of ranges offset in .debug_ranges");
            return DW_DLV_ERROR;
        }
        break;
    default:
    {
        char msg[128];
        sprintf(msg, "Error: unexpected form 0x%02x for DW_AT_ranges attribute", form);
        _error(rdc, msg);
        read_dwarf_free_attr(rdc, attr);
        return DW_DLV_ERROR;
    }
    }
    read_dwarf_free_attr(rdc, attr);

    /* next call dwarf_get_ranges_a to retrieve a Dwarf_Ranges array */

    if(dwarf_get_ranges_a(rdc->dbg, ranges_offset, func_die, &ranges_buf,
            &list_len, NULL, &rdc->err) != DW_DLV_OK)
    {
        _error(rdc, "Error getting ranges for function\n");
        return DW_DLV_ERROR;
    }

    *num_ranges = 0;
    if (list_len == 0)
    {
        *ranges = NULL;
        dwarf_ranges_dealloc(rdc->dbg, ranges_buf, list_len);
        return DW_DLV_OK;
    }
    /* Now loop over the ranges structure getting the start/end pairs */
    /* First count how many range pairs there will be to return */
    cur_range_entry = ranges_buf;
    while (cur_range_entry->dwr_type != DW_RANGES_END)
    {
        if (cur_range_entry->dwr_type == DW_RANGES_ENTRY)
        {
            (*num_ranges)++;
        }
        cur_range_entry ++;
        /* assert if we are disappearing off the end of the array */
        assert ((cur_range_entry - ranges_buf) <= list_len);
    }
    /* Allocate the storage for them*/
    *ranges = (Dwarf_Addr*)malloc(2*sizeof(Dwarf_Addr)*(*num_ranges));

    /* Write the addresses into the returned array, adding the current base
     * offset to the stored values, and updating the former as new values are
     * specified */
    current_range_base = CURRENT_RANGE_BASE_NOT_SET;
    cur_range_entry = ranges_buf;
    cur_addr_entry = *ranges;
    while (cur_range_entry->dwr_type != DW_RANGES_END)
    {
        switch (cur_range_entry->dwr_type)
        {
        case DW_RANGES_ENTRY:
            if (current_range_base == CURRENT_RANGE_BASE_NOT_SET)
            {
                /* we need the low PC of the containing CU DIE in order to interpret the
                 * entries in the ranges list */
                if (read_dwarf_get_die_cu_start_addr(rdc, func_die, &die_cu_start_addr) != DW_DLV_OK)
                {
                    return DW_DLV_ERROR;
                }
                current_range_base = die_cu_start_addr; /* Default */
            }
            cur_addr_entry[0] = cur_range_entry->dwr_addr1 + current_range_base;
            cur_addr_entry[1] = cur_range_entry->dwr_addr2 + current_range_base;
            cur_addr_entry += 2;
            /* assert if we are disappearing off the end of the array */
            assert (cur_addr_entry <= *ranges + 2*(*num_ranges));
            break;
        case DW_RANGES_ADDRESS_SELECTION:
            current_range_base = cur_range_entry->dwr_addr2;
            break;
        default:
            _error(rdc, "Unexpected dwr_type in ranges structure.  Corruption?");
            free(*ranges);
            dwarf_ranges_dealloc(rdc->dbg, ranges_buf, list_len);
            return DW_DLV_ERROR;
        }
        cur_range_entry ++;
    }
    dwarf_ranges_dealloc(rdc->dbg, ranges_buf, list_len);
    return DW_DLV_OK;
}

int read_dwarf_is_abstract_entry(read_dwarf_ctx *rdc, Dwarf_Die die)
{
    Dwarf_Half tag;
    Dwarf_Bool has_attr;

    if (read_dwarf_tag(rdc, die, &tag) != DW_DLV_OK)
    {
        return FALSE;
    }

    switch(tag)
    {
    case DW_TAG_subprogram:

       if ((dwarf_hasattr(die, DW_AT_low_pc, &has_attr, &rdc->err) != DW_DLV_OK || !has_attr) &&
           (dwarf_hasattr(die, DW_AT_entry_pc, &has_attr, &rdc->err) != DW_DLV_OK || !has_attr) &&
           (dwarf_hasattr(die, DW_AT_entry_pc, &has_attr, &rdc->err) != DW_DLV_OK || !has_attr))
       {
           /* A subprogram with no indication of address is */
           return TRUE;
       }
    default:
        break;
    }
    return FALSE;
}

int read_dwarf_has_normal_calling_convention(read_dwarf_ctx *rdc,
                                             Dwarf_Die func_die,
                                             Dwarf_Bool *has_normal_cc)
{
    Dwarf_Bool has_attr;
    Dwarf_Attribute cc_attr_desc;
    Dwarf_Half form;
    Dwarf_Unsigned uval;

    if (dwarf_hasattr(func_die, DW_AT_calling_convention, &has_attr, &rdc->err)
                                                                != DW_DLV_OK)
    {
        _error(rdc, "Error checking for calling convention attribute in CU");
        return DW_DLV_ERROR;
    }
    if (!has_attr)
    {
        *has_normal_cc = TRUE;
        return DW_DLV_OK;
    }
    if (dwarf_attr(func_die, DW_AT_calling_convention, &cc_attr_desc, &rdc->err)
                                                                != DW_DLV_OK)
    {
        _error(rdc, "Error getting calling convention attribute descriptor");
        return DW_DLV_ERROR;
    }

    /* Get the attribute's form, and then make an appropriate call to get
       its value */
    if (dwarf_whatform(cc_attr_desc, &form, &rdc->err) != DW_DLV_OK)
    {
        read_dwarf_free_attr(rdc, cc_attr_desc);
        _error(rdc, "Failed getting form of calling convention attribute!");
        return DW_DLV_ERROR;
    }

    /* Check the form is what we expect and get the value */
    switch(form)
    {
    case DW_FORM_data1:
    case DW_FORM_data2:
    case DW_FORM_data4:
    case DW_FORM_data8:
    case DW_FORM_udata:
        if (dwarf_formudata(cc_attr_desc, &uval, &rdc->err) != DW_DLV_OK)
        {
            read_dwarf_free_attr(rdc, cc_attr_desc);
            _error(rdc, "Failed getting value of producer attribute!");
            return DW_DLV_ERROR;
        }
        break;
    default:
        read_dwarf_free_attr(rdc, cc_attr_desc);
        _error(rdc, "Producer attribute did not have 'unsigned constant' "
                "form!");
        return DW_DLV_ERROR;
    }

    read_dwarf_free_attr(rdc, cc_attr_desc);
    *has_normal_cc = (uval == DW_CC_normal);
    return DW_DLV_OK;
}

int read_dwarf_get_producer(read_dwarf_ctx *rdc, Dwarf_Die die,
                                                   const char **producer_string)
{
    Dwarf_Off cu_die_offset;
    Dwarf_Die cu_die;
    Dwarf_Bool has_attr;
    Dwarf_Half form;
    Dwarf_Attribute attr;

    /* Get the CU DIE of the given DIE*/
    if (dwarf_CU_dieoffset_given_die(die, &cu_die_offset, &rdc->err)
                                                                != DW_DLV_OK)
    {
        _error(rdc, "Error getting CU DIE offset from DIE");
        return DW_DLV_ERROR;
    }

    if (dwarf_offdie(rdc->dbg, cu_die_offset, &cu_die, NULL) != DW_DLV_OK)
    {
        _error(rdc, "Error getting CU DIE from offset");
        return DW_DLV_ERROR;
    }

    switch(read_dwarf_has_attr(rdc, cu_die, DW_AT_producer, &has_attr, &form,
                                &attr))
    {
    case DW_DLV_ERROR:
        read_dwarf_free_die(rdc, cu_die);
        return DW_DLV_ERROR;
    case DW_DLV_NO_ENTRY:
        read_dwarf_free_die(rdc, cu_die);
        if (producer_string)
        {
            *producer_string = 0;
        }
        return DW_DLV_NO_ENTRY;
    default:
        break;
    }
    read_dwarf_free_die(rdc, cu_die);
    if (producer_string)
    {
        int ret = read_dwarf_get_string_attr(rdc, attr, producer_string);
        read_dwarf_free_attr(rdc, attr);
        return ret;
    }
    return DW_DLV_OK;

}

int read_dwarf_unwind_data(read_dwarf_ctx *rdc, Dwarf_Die func_die,
                           Dwarf_Addr addr_offset, Dwarf_Signed *reg_nums,
                           Dwarf_Signed num_regs, Dwarf_Signed **rules,
                           Dwarf_Bool add_return_address_rule)
{
    Dwarf_Fde func_fde;
    Dwarf_Cie func_cie;
    Dwarf_Addr low_pc;
    Dwarf_Addr row_pc;
    unsigned i;
    Dwarf_Small value_type;
    Dwarf_Ptr block_ptr;
    Dwarf_Signed offset, register_num, offset_relevant;
    Dwarf_Half return_addr_num;
    Dwarf_Signed num_returned_regs = add_return_address_rule ?
                                                    num_regs + 1 : num_regs;

    Dwarf_Unsigned cie_byte_len;
    Dwarf_Small version;
    char *augmentation;
    Dwarf_Unsigned caf;
    Dwarf_Signed daf;
    Dwarf_Small *init_inst;
    Dwarf_Unsigned inst_len;
    const char *producer;
    Dwarf_Bool factor_cfa_offset = FALSE;


    /* First we have to find the info relating to this function in the
     * .debug_frame section.  To do that, we need to get hold of the list of
     * FDEs and then find the one corresponding to the PC we're interested in
     *
     * We store the list of FDEs in the context object.
     */

    /* First, get hold of the relevant data */
    if (rdc->fdes == NULL &&
           dwarf_get_fde_list(rdc->dbg,
                              &rdc->cies, &rdc->n_cies,
                              &rdc->fdes, &rdc->n_fdes,
                              &rdc->err) != DW_DLV_OK)
    {
        _error(rdc, "Failed to get FDE list!");
        *rules = NULL;
        return DW_DLV_ERROR;
    }

    /* Then we need the absolute value of the PC, i.e. we need to find the
     * low PC of the function and add it to the specified offset */
    if (read_dwarf_get_low_pc(rdc, func_die, &low_pc) != DW_DLV_OK)
    {
        _error(rdc, "Failed to get function's low PC attribute");
        *rules = NULL;
        return DW_DLV_ERROR;
    }

    /* Now get the relevant FDE */
    switch (dwarf_get_fde_at_pc(rdc->fdes, low_pc + addr_offset, &func_fde,
                            NULL, NULL, &rdc->err))
    {
    case DW_DLV_OK:
        break;
    case DW_DLV_NO_ENTRY:
        *rules = NULL;
        return DW_DLV_NO_ENTRY;
    case DW_DLV_ERROR:
        _error(rdc, "Failed to get FDE at PC");
        *rules = NULL;
        return DW_DLV_ERROR;
    }

    /* We need details from the CIE for a couple of things */

    /* Get the CIE */
    if (dwarf_get_cie_of_fde(func_fde, &func_cie, &rdc->err) != DW_DLV_OK)
    {
        _error(rdc, "Error getting CIE of FDE");
        return DW_DLV_ERROR;
    }
    /* Read the CIE info */
    if (dwarf_get_cie_info(func_cie, &cie_byte_len, &version, &augmentation,
            &caf, &daf, &return_addr_num, (Dwarf_Ptr*)&init_inst, &inst_len,
                                                       &rdc->err) != DW_DLV_OK)
    {
        _error(rdc, "Error getting CIE info");
        return DW_DLV_ERROR;
    }

    switch(read_dwarf_get_producer(rdc, func_die, &producer))
    {
    case DW_DLV_ERROR:
        _error(rdc, "Error getting function CU's producer string");
        return DW_DLV_ERROR;
    case DW_DLV_NO_ENTRY:
    case DW_DLV_OK:
        break;
    }

    /* We need to check for the ARMCC factoring bug: it applies if
     *  - producer starts with "Component: ARM Compiler" and
     *  - the CIE version is 1 (DWARF2) and
     *  - the augmentation is *not* armcc+ .
     *  (We also need to check there are some initial instructions in the CIE) */
    if (memcmp(producer, "Component: ARM Compiler 5",
               strlen("Component: ARM Compiler 5")) == 0 &&
            version == 1 &&
            strcmp(augmentation,"armcc+") != 0 &&
            inst_len > 0)
    {
        /* Is DW_CFA_def_cfa the initial instruction for the CFA rule: if so,
         * assume we have to scale CFA offsets manually */
        if (init_inst[0] == DW_CFA_def_cfa)
        {
            factor_cfa_offset = TRUE;
        }
    }

    *rules = (Dwarf_Signed *)malloc((size_t)(3 * num_returned_regs *
                                                    sizeof(Dwarf_Signed)));
    if (*rules == NULL)
    {
        _error(rdc, "Couldn't allocate register rules return array");
        return DW_DLV_ERROR;
    }

    /* Get the rules for the registers in the main list */
    for (i = 0; i < num_regs; ++i)
    {
        /* The interface to the FDE table columns is quite messy.  In DWARF2,
         * you can *sometimes* use column number 0 in a call to
         * dwarf_get_fde_info_for_reg to get the CFA rule.  But sometimes you
         * can't; in particular things get nasty if you have a register that
         * maps to DWARF register 0, as xapgcc DWARF does.  It appears to be
         * better to use the DWARF3 function dwarf_get_fde_info_for_cfa_reg3,
         * which is backwards compatible to DWARF2.
         *
         * In terms of the interface to read_dwarf, we use DW_FRAME_CFA_COL3
         * from libdwarf.h to indicate that we're asking for the CFA column */
        if (reg_nums[i] == DW_FRAME_CFA_COL3)
        {
            /* Since this library is DWARF2-orientated, we know that we don't
             * need all the information this function returns. However, we do
             * need to supply space to put it in. */
            if (dwarf_get_fde_info_for_cfa_reg3(func_fde,
                                                low_pc + addr_offset,
                                                &value_type,
                                                &offset_relevant,
                                                &register_num,
                                                &offset,
                                                &block_ptr,
                                                &row_pc, &rdc->err) != DW_DLV_OK)
            {
                _error(rdc, "Error getting FDE info for CFA");
                free(*rules);
                *rules = NULL;
                return DW_DLV_ERROR;
            }
            if (factor_cfa_offset)
            {
                /* Apply the fix for the ARM compiler factor bug */
                offset *= -daf;
            }
        }
        else
        {
            if (dwarf_get_fde_info_for_reg3(func_fde, (Dwarf_Half)reg_nums[i],
                                           low_pc + addr_offset,
                                           &value_type,
                                           &offset_relevant,
                                           &register_num,
                                           &offset,
                                           &block_ptr,
                                           &row_pc, &rdc->err) != DW_DLV_OK)
            {
                _error(rdc, "Error getting FDE info for reg");
                free(*rules);
                *rules = NULL;
                return DW_DLV_ERROR;
            }
        }
        if (value_type != DW_EXPR_OFFSET)
        {
            char msg[64];
            sprintf(msg, "Unsupported value type %d!\n", value_type);
            _error(rdc, msg);
            free(*rules);
            *rules = NULL;
            return DW_DLV_ERROR;
        }

        (*rules)[3*i] = offset_relevant;
        (*rules)[3*i + 1] = offset_relevant ? offset : 0;
        (*rules)[3*i + 2] = register_num;
    }

    /* If requested, look up in the CIE which DWARF register number the return
     * address is in and look up the rule for it, adding it to the end of the
     * list (which was allocated for this above) */
    if (add_return_address_rule)
    {
        /* Extract the return address rule */
        if (dwarf_get_fde_info_for_reg3(func_fde, return_addr_num,
                                       low_pc + addr_offset,
                                       &value_type,
                                       &offset_relevant,
                                       &register_num,
                                       &offset,
                                       &block_ptr,
                                       &row_pc, &rdc->err) != DW_DLV_OK)
        {
            _error(rdc, "Error getting FDE info for RA reg");
            free(*rules);
            *rules = NULL;
            return DW_DLV_ERROR;
        }
        if (value_type != DW_EXPR_OFFSET)
        {
            char msg[64];
            sprintf(msg, "Unsupported value type %d!\n", value_type);
            _error(rdc, msg);
            free(*rules);
            *rules = NULL;
            return DW_DLV_ERROR;
        }
        /* Shove it on the end of the list */
        (*rules)[3*num_regs] = offset_relevant;
        (*rules)[3*num_regs + 1] = offset_relevant ? offset : 0;
        (*rules)[3*num_regs + 2] = register_num;
    }

    return DW_DLV_OK;
}

/* This value pops up mysteriously in K32 Audio debug_line tables */
#define READ_DWARF_MAGIC_DEBUG_LINE_PC_VALUE 0xff800000ULL

char *read_dwarf_get_source_file_and_line(read_dwarf_ctx *rdc,
                                          Dwarf_Die func_die,
                                          Dwarf_Addr pc_offset,
                                          Dwarf_Unsigned *line_no)
{
    Dwarf_Off cu_die_offset;
    Dwarf_Die cu_die;
    Dwarf_Line *lines;
    Dwarf_Signed nlines, iline;
    Dwarf_Addr pc;
    Dwarf_Addr best_addr = 0;
    Dwarf_Line line = NULL;

    *line_no = 0; /* Set for error exit */

    /* Get the absolute value of the PC */
    if (read_dwarf_get_low_pc(rdc, func_die, &pc) != DW_DLV_OK)
    {
        _error(rdc, "Failed to get function's low PC attribute");
        return NULL;
    }
    pc += pc_offset;


    if (dwarf_CU_dieoffset_given_die(func_die, &cu_die_offset, &rdc->err)
                                                                != DW_DLV_OK)
    {
        _error(rdc, "Error getting CU DIE offset from function DIE");
        return NULL;
    }

    if (dwarf_offdie(rdc->dbg, cu_die_offset, &cu_die, NULL) != DW_DLV_OK)
    {
        _error(rdc, "Error getting CU DIE from offset");
        return NULL;
    }

    /* Get the line information for the CU DIE */
    switch (dwarf_srclines(cu_die, &lines, &nlines, &rdc->err))
    {
    case DW_DLV_NO_ENTRY:
        /* This is legitimate: it could be that we're looking up an
         * uninstrumented function */
        *line_no = 1;
        return NULL;
    case DW_DLV_ERROR:
        _error(rdc, "Error getting Dwarf_Lines for CU");
        return NULL;
    default:
        break;
    }

    /*
     * Now loop through looking for the line that includes the PC, this
     * is the line whose start address is closest to PC without being past
     * PC.
     *
     * The lines are almost stored in order, but the first entry can
     * sometimes be a bit weird such as 0x00ffffff for XAP firmware.
     *
     * The special test against READ_DWARF_MAGIC_DEBUG_LINE_PC_VALUE dates
     * back to when this code used an older algorithm which looked for the
     * first entry in the array greater than or equal to the target
     * address. It's possible that this can now be removed.
     */
    for (iline = 0; iline < nlines; ++iline)
    {
        Dwarf_Addr addr;
        if (dwarf_lineaddr(lines[iline], &addr, &rdc->err) != DW_DLV_OK)
        {
            _error(rdc, "Error getting address of source line");
            return NULL;
        }

        if (addr != READ_DWARF_MAGIC_DEBUG_LINE_PC_VALUE && addr <= pc &&
            (addr > best_addr || line == NULL))
        {
            best_addr = addr;
            line = lines[iline];
        }
    }

    if (line != NULL)
    {
        /* Either the current or the previous line is the one.  We need to get:
         *  - the source filename
         *  - the line number within that file
         *  But what if the assembly instruction doesn't correspond to any
         *  source text (e.g. it's in a function prologue)?
         */
        char *file_name;

        if (dwarf_lineno(line, line_no, &rdc->err) != DW_DLV_OK)
        {
            _error(rdc, "Error getting source line number");
            return NULL;
        }

        if (dwarf_linesrc(line, &file_name, &rdc->err) != DW_DLV_OK)
        {
            _error(rdc, "Error getting name of source file");
            return NULL;
        }

        dwarf_srclines_dealloc(rdc->dbg, lines, nlines);
        return file_name;
    }

    dwarf_srclines_dealloc(rdc->dbg, lines, nlines);
    _warning(rdc, "Instruction address not found in DWARF information");
    *line_no = 1;
    return NULL;
}

int read_dwarf_get_func_child_entries(read_dwarf_ctx *rdc, Dwarf_Die func_die,
                                      Dwarf_Half entry_tag,
                                      Dwarf_Die **child_dies, unsigned *n_children)
{
    if (read_dwarf_get_tag_attr_in_tree(rdc, func_die,
                                        entry_tag,
                                        READ_DWARF_ANY_ATTR,
                                        2, /* Just get params from the first
                                        level children */
                                        child_dies,
                                        n_children) != DW_DLV_OK)
    {
        _error(rdc, "Failed to obtain DIEs for tag");
        return DW_DLV_ERROR;
    }

    return DW_DLV_OK;
}

/**
 * Free the location description entry or buffer (which it is depends on whether
 * form is DW_FORM_exprloc or not)
 */
static void read_dwarf_free_loc_or_loclist(read_dwarf_ctx *rdc, Dwarf_Half form,
                                           Dwarf_Locdesc *loc_desc,
                                           Dwarf_Locdesc **loc_desc_buf)
{
    if (form == DW_FORM_exprloc)
    {
        dwarf_dealloc(rdc->dbg, loc_desc, DW_DLA_LOCDESC);
    }
    else
    {
        dwarf_dealloc(rdc->dbg, loc_desc_buf, DW_DLA_LIST);
    }
}


int read_dwarf_get_loc_list(read_dwarf_ctx *rdc, Dwarf_Die local, Dwarf_Addr pc,
                            Dwarf_Bool executing,
                            Dwarf_Small **op_list, Dwarf_Unsigned **arg_list,
                            unsigned *num_ops)
{
    Dwarf_Bool has_attr;
    Dwarf_Half form;
    Dwarf_Attribute loc_attr_desc;
    Dwarf_Locdesc **loc_desc_buf, *loc_desc;
    Dwarf_Signed n_locs;
    Dwarf_Off cu_die_offset;
    Dwarf_Die cu_die;
    Dwarf_Addr cu_low_pc;
    unsigned i, j;
    const char *prod_string;
    Dwarf_Unsigned exprlen;
    Dwarf_Ptr exprblock;

    if (dwarf_hasattr(local, DW_AT_location, &has_attr, &rdc->err) != DW_DLV_OK)
    {
        _error(rdc, "Error checking for location attribute in local DIE");
        return DW_DLV_ERROR;
    }

    if (!has_attr)
    {
        _warning(rdc, "Local doesn't have location attribute");
        return DW_DLV_NO_ENTRY;
    }

    /* Grab the type attribute descriptor */
    if (dwarf_attr(local, DW_AT_location, &loc_attr_desc, &rdc->err) != DW_DLV_OK)
    {
        return DW_DLV_NO_ENTRY;
    }

    if (dwarf_whatform(loc_attr_desc, &form, &rdc->err) != DW_DLV_OK)
    {
        _error(rdc, "Error checking form of location attribute");
        read_dwarf_free_attr(rdc, loc_attr_desc);
        return DW_DLV_ERROR;
    }

    switch(form)
    {
    /* Later libdwarfs than we have can handle DW_FORM_exprloc inside
     * dwarf_loclist_n: it's essentially the same thing as DW_FORM_block, so
     * you'd expect that.  However, with our libdwarf you have to handle
     * exprloc manually. */
    case DW_FORM_exprloc:
        if(dwarf_formexprloc(loc_attr_desc, &exprlen, &exprblock,
                &rdc->err) != DW_DLV_OK)
        {
            _error(rdc, "Error decoding exprloc form location attribute");
            read_dwarf_free_attr(rdc, loc_attr_desc);
            return DW_DLV_ERROR;
        }
        if(dwarf_loclist_from_expr(rdc->dbg,exprblock, exprlen, &loc_desc,
                                    &n_locs, &rdc->err))
        {
            _error(rdc, "Error interpreting location expression");
            read_dwarf_free_attr(rdc, loc_attr_desc);
            read_dwarf_free_loc_or_loclist(rdc, form, loc_desc, loc_desc_buf);
            return DW_DLV_ERROR;
        }
        loc_desc_buf = &loc_desc;
        break;
    case DW_FORM_data4:
    case DW_FORM_data8:
    case DW_FORM_block1:
    case DW_FORM_block2:
    case DW_FORM_block4:
    case DW_FORM_block:
        switch (dwarf_loclist_n(loc_attr_desc, &loc_desc_buf, &n_locs, &rdc->err))
        {
        case DW_DLV_ERROR:
        {
            char msg[128];
            sprintf(msg, "Error getting loclist (form was 0x%02x)", form);
            _error(rdc, msg);
            read_dwarf_free_attr(rdc, loc_attr_desc);
            return DW_DLV_ERROR;
        }
            break;
        case DW_DLV_NO_ENTRY:
            return DW_DLV_NO_ENTRY;
        case DW_DLV_OK:
            break;
        }
        break;
    default:
    {
        char msg[128];
        sprintf(msg, "Unexpected form 0x%02x for location", form);
        _error(rdc, msg);
        read_dwarf_free_attr(rdc, loc_attr_desc);
        return DW_DLV_ERROR;
    }
    }
    read_dwarf_free_attr(rdc, loc_attr_desc);

    /* Now we have to manual relocate all the PCs in the location ops */

    /* First, get the CU die */
    if (dwarf_CU_dieoffset_given_die(local, &cu_die_offset, &rdc->err)
                                                                != DW_DLV_OK)
    {
        read_dwarf_free_loc_or_loclist(rdc, form, loc_desc, loc_desc_buf);
        _error(rdc, "Error getting CU DIE offset from function DIE");
        return DW_DLV_ERROR;
    }

    if (dwarf_offdie(rdc->dbg, cu_die_offset, &cu_die, NULL) != DW_DLV_OK)
    {
        read_dwarf_free_loc_or_loclist(rdc, form, loc_desc, loc_desc_buf);
        _error(rdc, "Error getting CU DIE from offset");
        return DW_DLV_ERROR;
    }


    /* KCC DWARF has absolute PCs in its loclists because it doesn't necessarily
     * link compilation units in a single address range.  In that case the
     * producer string begins '[A] Kalimba C Compiler' (see B-78587) */
    if (read_dwarf_get_producer(rdc, local, &prod_string) == DW_DLV_ERROR)
    {
        read_dwarf_free_loc_or_loclist(rdc, form, loc_desc, loc_desc_buf);
        _error(rdc, "Error checking for producer attribute in CU");
        return DW_DLV_ERROR;
    }

    /* If we're not dealing with an absolute location expression and we're not
     * looking at a KCC ELF with absolute PCs, then we need to adjust the PCs
     * in the locdescs */
    if (!(n_locs == 1 && !loc_desc_buf[0]->ld_from_loclist) &&
        !(strlen(prod_string) >= 22 && memcmp(prod_string, "[A] Kalimba C Compiler",
                                                                      22) == 0))
    {
        /* Relative locdescs: Add the CU's low_pc to the pc ranges */
        if (read_dwarf_get_low_pc(rdc, cu_die, &cu_low_pc) != DW_DLV_OK)
        {
            read_dwarf_free_loc_or_loclist(rdc, form, loc_desc, loc_desc_buf);
            _error(rdc, "Error getting low PC of compile unit");
            return DW_DLV_ERROR;
        }

        /* Finally, loop over the locdescs adjusting the relevant fields */
        for (i = 0; i < n_locs; ++i)
        {
            Dwarf_Locdesc *desc = loc_desc_buf[i];
            desc->ld_hipc += cu_low_pc;
            desc->ld_lopc += cu_low_pc;
        }
    }

    /* Now find where the given variable is stored at the given PC (if anywhere) */
    for (i = 0; i < n_locs; ++i)
    {
        Dwarf_Locdesc *locdesc = loc_desc_buf[i];

        if (!locdesc->ld_from_loclist ||
              (locdesc->ld_lopc <= pc && pc < locdesc->ld_hipc &&
             (!executing || pc != locdesc->ld_hipc - 1)))
        {
            /* We pass the location descriptions back in two arrays: one contains
             * the operations, the other the triples of arguments (though as yet
             * we only support operations that use the first argument) */
            *num_ops = locdesc->ld_cents;
            *op_list = (Dwarf_Small *)malloc(
                                    locdesc->ld_cents*sizeof(Dwarf_Small));
            *arg_list = (Dwarf_Unsigned *)malloc(
                                    3*locdesc->ld_cents*sizeof(Dwarf_Unsigned));
            for (j = 0; j < locdesc->ld_cents; ++j)
            {
                Dwarf_Loc *loc = locdesc->ld_s + j;
                (*op_list)[j] = loc->lr_atom;
                (*arg_list)[3*j] = loc->lr_number;
                (*arg_list)[3*j+1] = loc->lr_number2;
                (*arg_list)[3*j+2] = loc->lr_offset;
            }
            read_dwarf_free_loc_or_loclist(rdc, form, loc_desc, loc_desc_buf);
            return DW_DLV_OK;
        }
    }
    read_dwarf_free_loc_or_loclist(rdc, form, loc_desc, loc_desc_buf);
    return DW_DLV_NO_ENTRY;
}

/**
 * Decode a stream of bytes containing a LEB128 encoding.  Each byte contains
 * 7 bits of the integer with bit 7 reserved as a continuation flag.
 * @param encoded
 */
static unsigned decode_leb128u(Dwarf_Small *encoded)
{
    unsigned value = (*encoded) & 0x7f;
    unsigned offset = 0;

    while(*encoded & 0x80)
    {
        offset += 7;
        encoded++;
        value |= ((*encoded) & 0x7f) << offset;
    }
    return value;
}


