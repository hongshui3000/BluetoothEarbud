/****************************************************************************
Copyright (c) 2017 Qualcomm Technologies International, Ltd.

DESCRIPTION
    Chain library bundle management implementation.
*/

#include "chain.h"
#include "chain_list.h"
#include "chain_config.h"
#include "chain_bundle_management.h"
#include "chain_dkcs_reader.h"

#include <panic.h>
#include <stdlib.h>
#include <operators.h>
#include <print.h>
#include <string.h>
#include <file.h>

typedef struct bundle_map_tag bundle_map_t;

/* Map between a bundle definition and the loaded bundle handle. 
   Count the number of bundle users. */
struct bundle_map_tag
{
    /* A bundle handle loaded by this library */
    BundleID handle;
    /* The definition of the bundle parameters */
    const capability_bundle_t *definition;
    /* The number of users of the bundle */
    unsigned users;
    /* A linked list */
    bundle_map_t *next;
};

static bundle_map_t *bundle_map_list = NULL;

/* This default config is added as a temporary measure to make the sink application work
   with a downloadable cVc send capability. Once bundle configs are supported by the normal
   configuration system, this can be removed, and there will be no default bundle.*/
static const capability_bundle_t default_bundle = {"download_cvc_send.dkcs",
                                                   capability_bundle_available_p0_and_p1};

static const capability_bundle_config_t default_bundle_config = {&default_bundle, 1};

static const capability_bundle_config_t *bundle_config = &default_bundle_config;

/* Add to head of the list */
static void bundleMapListAdd(bundle_map_t *bundle)
{
    bundle->next = bundle_map_list;
    bundle_map_list = bundle;
}

static void bundleMapListRemove(bundle_map_t *bundle)
{
    bundle_map_t **b;
    for (b = &bundle_map_list; *b != NULL; b = &(*b)->next)
    {
        if (*b == bundle)
        {
            *b = bundle->next;
            break;
        }
    }
}

/* Get the bundle map from the bundle definition */
static bundle_map_t *capabilityBundleToMap(const capability_bundle_t *bundle)
{
    bundle_map_t *bundle_map;
    for (bundle_map = bundle_map_list; bundle_map != NULL; bundle_map = bundle_map->next)
    {
        if (bundle_map->definition == bundle)
        {
            return bundle_map;
        }
    }
    return NULL;
}

static void bundleLoad(const capability_bundle_t *bundle)
{
    bundle_map_t *bundle_map = capabilityBundleToMap(bundle);
    if (!bundle_map)
    {
        const char *file_name = bundle->file_name;
        FILE_INDEX index = FileFind(FILE_ROOT, file_name, (uint16)strlen(file_name));
        PanicFalse(index != FILE_NONE);
        bundle_map = PanicNull(calloc(1, sizeof(*bundle_map)));
        bundle_map->handle = OperatorBundleLoad(index, bundle->processors);
        PanicFalse(bundle_map->handle != BUNDLE_ID_INVALID);
        bundle_map->definition = bundle;
        bundleMapListAdd(bundle_map);
        PRINT(("Chain loaded %s\n", bundle->file_name));
    }
    bundle_map->users++;
}

static void bundleUnload(const capability_bundle_t *bundle)
{
    bundle_map_t *bundle_map = capabilityBundleToMap(bundle);
    if (bundle_map)
    {
        bundle_map->users--;
        if (bundle_map->users == 0)
        {
            PanicFalse(OperatorBundleUnload(bundle_map->handle));
            bundleMapListRemove(bundle_map);
            free(bundle_map);
            PRINT(("Chain unloaded %s\n", bundle->file_name));
        }
        return;
    }
    /* Attempting to unload, unloaded bundle */
    Panic();
}

/* Return TRUE if any of the capabilities defined in the operator configs are found in the bundle */
static bool capabilitiesInBundle(const kymera_chain_t *chain, const capability_bundle_t *bundle)
{
    bool found = FALSE;
    dkcs_header_t *dkcs_header = dkcsHeaderRead(bundle->file_name);
    
    if (dkcs_header)
    {
        unsigned i;
        
        /* Iterate through operator configs in chain config */
        for (i = 0; (!found) && (i < chain->config->number_of_operators); i++)
        {
            uint16 *bundle_cap_id;
            const operator_config_t *op_config = chainConfigGetOperatorConfig(chain, i);
            if (op_config)
            {
                uint16 config_cap_id = op_config->capability_id;
                for (bundle_cap_id = dkcs_header->capability_ids; 
                     bundle_cap_id < dkcs_header->capability_ids + dkcs_header->num_cap_ids;
                     bundle_cap_id++)
                {
                    if (config_cap_id == *bundle_cap_id)
                    {
                        found = TRUE;
                        break;
                    }
                }
            }
        }
        dkcsHeaderFree(dkcs_header);
    }
    else
    {
        PRINT(("Chain: failed to read dkcs header for %s\n", bundle->file_name));
    }
    return found;
}

static void bundlesLoadUnload(const kymera_chain_t *chain, bool load_unload)
{
    const capability_bundle_t *bundle;

    if (!bundle_config)
    {
        return;
    }

    /* Iterate through capability bundles */
    for (bundle = bundle_config->capability_bundles;
         bundle < bundle_config->capability_bundles + bundle_config->number_of_capability_bundles;
         bundle++)
    {
        if (capabilitiesInBundle(chain, bundle))
        {
            load_unload ? bundleLoad(bundle) : bundleUnload(bundle);
        }
    }
}

void ChainCapabilityBundlesLoad(const kymera_chain_t *chain)
{
    bundlesLoadUnload(chain, TRUE);
}

void ChainCapabilityBundlesUnload(const kymera_chain_t *chain)
{
    bundlesLoadUnload(chain, FALSE);
}

/* Set the bundle config */
void ChainSetDownloadableCapabilityBundleConfig(const capability_bundle_config_t *config)
{
    bundle_config = config;
}
