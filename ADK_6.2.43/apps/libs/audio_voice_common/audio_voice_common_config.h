/****************************************************************************
Copyright (c) 2017 Qualcomm Technologies International, Ltd.

FILE NAME
    audio_voice_common_config.h

DESCRIPTION
    Audio voice common configuration.
*/

#ifndef _AUDIO_VOICE_COMMON_CONFIG_H_
#define _AUDIO_VOICE_COMMON_CONFIG_H_

#include <chain.h>
#include <audio_plugin_voice_variants.h>

#include "audio_voice_common.h"


typedef enum
{
    stream_receive,
    stream_send_mic1,
    stream_send_mic2,
    stream_aec_ref
} stream_channel_t;

typedef enum
{
    receive_role = audio_voice_receive_role,
    send_role = audio_voice_send_role,
    cvc_receive_role,
    cvc_send_role
} hfp_operator_roles_t;

typedef enum
{
    hfp_receive_input,
    hfp_receive_output,
    hfp_aec_ref_input,
    hfp_mic_a_input,
    hfp_mic_b_input,
    hfp_send_output
} hfp_ep_roles_t;


/****************************************************************************
DESCRIPTION
    Get the chain configuration for HFP
RETURNS
    Audio Input HFP chain configuration
*/
const chain_config_t* AudioVoiceCommonGetChainConfig(const audio_voice_context_t* ctx);


#endif /* _AUDIO_VOICE_COMMON_CONFIG_H_ */
