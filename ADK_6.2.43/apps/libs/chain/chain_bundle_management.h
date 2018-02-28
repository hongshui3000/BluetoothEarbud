/****************************************************************************
Copyright (c) 2017 Qualcomm Technologies International, Ltd.
*/

#ifndef LIBS_CHAIN_CHAIN_BUNDLE_MANAGEMENT_H_
#define LIBS_CHAIN_CHAIN_BUNDLE_MANAGEMENT_H_

#include "chain.h"

#include <csrtypes.h>
#include <vmtypes.h>

/****************************************************************************
DESCRIPTION
    Load the downloadable capability bundles required for the chain_config.
    This function tracks all users of downloadable capability bundles, only
    loading when there are no other users.
    A bundle will not be loaded a second time if it has already been loaded.
*/
void ChainCapabilityBundlesLoad(const kymera_chain_t *chain);

/****************************************************************************
DESCRIPTION
    Unload the downloadable capability bundles loaded for the chain_config.
    This function tracks all users of downloadable capability bundles, only
    unloading when there are no other users.
    A bundle will not be unloaded if there are other users of the bundle.
*/
void ChainCapabilityBundlesUnload(const kymera_chain_t *chain);

#endif /* LIBS_CHAIN_CHAIN_BUNDLE_MANAGEMENT_H_ */
