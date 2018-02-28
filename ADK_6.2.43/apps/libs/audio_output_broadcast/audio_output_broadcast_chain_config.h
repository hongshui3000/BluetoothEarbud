/****************************************************************************
Copyright (c) 2017 Qualcomm Technologies International, Ltd.
Part of 6.2
 
FILE NAME
    audio_output_broadcast_chain_config.h
 
DESCRIPTION
    Kymera chain configuration for audio_output_broadcast.
*/

#ifndef AUDIO_OUTPUT_BROADCAST_CHAIN_CONFIG_H_
#define AUDIO_OUTPUT_BROADCAST_CHAIN_CONFIG_H_

#include <chain.h>

enum
{
    pre_processing_role,
    compander_role,
    peq_role,
    vse_role,
    dbe_role,
    cancel_headroom_role,
    encoder_role,
    splitter_role,
    decoder_role,
    broadcast_buffer_role,
    local_buffer_role
};

typedef enum
{
    audio_input_left,
    audio_input_right,
    audio_output_left,
    audio_output_right,
    ba_output
} chain_endpoint_role_t;

/****************************************************************************
DESCRIPTION
    Return the chain configuration.
*/
const chain_config_t* AudioOutoutBaGetChainConfig(void);

#define SPLITTER_BUFFER_SIZE (6000)
#define BROADCAST_BUFFER_SIZE (2048)
#define LOCAL_BUFFER_SIZE (1024)
#define PRE_PROCESSING_BUFFER_SIZE (1600)

#endif /* AUDIO_OUTPUT_BROADCAST_CHAIN_CONFIG_H_ */
