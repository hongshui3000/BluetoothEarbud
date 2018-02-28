/****************************************************************************
Copyright (c) 2017 Qualcomm Technologies International, Ltd.
*/

#ifndef CHAIN_STREAM_H_
#define CHAIN_STREAM_H_

#include "chain_list.h"

/****************************************************************************
DESCRIPTION
    Get the first valid input terminal in the stream with a role which matches
    input_role
*/
Sink chainStreamGetInput(kymera_chain_t *chain, unsigned input_role);

/****************************************************************************
DESCRIPTION
    Get the first valid output terminal in the stream with a role which matches
    output_role
*/
Source chainStreamGetOutput(kymera_chain_t *chain, unsigned output_role);

/****************************************************************************
DESCRIPTION
    Connect all streams in chain
*/
void chainStreamConnect(kymera_chain_t *chain);

#endif /* CHAIN_STREAM_H_ */
