/****************************************************************************
Copyright (c) 2017 Qualcomm Technologies International, Ltd.
Part of 6.2
 
FILE NAME
    audio_output_broadcast_chain_config.c
 
DESCRIPTION
    Kymera chain configuration for audio_output_broadcast.
*/

#include <operators.h>

#include "audio_output_broadcast_chain_config.h"

static const operator_config_t op_config[] =
{
    MAKE_OPERATOR_CONFIG(capability_id_passthrough, pre_processing_role),
    MAKE_OPERATOR_CONFIG(capability_id_compander, compander_role),
    MAKE_OPERATOR_CONFIG(capability_id_peq, peq_role),
    MAKE_OPERATOR_CONFIG(capability_id_vse, vse_role),
    MAKE_OPERATOR_CONFIG(capability_id_bass_enhance, dbe_role),
    MAKE_OPERATOR_CONFIG(capability_id_passthrough, cancel_headroom_role),
    MAKE_OPERATOR_CONFIG(capability_id_celt_encoder, encoder_role),
    MAKE_OPERATOR_CONFIG(capability_id_splitter, splitter_role),
    MAKE_OPERATOR_CONFIG(capability_id_celt_decoder, decoder_role),
    MAKE_OPERATOR_CONFIG(capability_id_passthrough, broadcast_buffer_role),
    MAKE_OPERATOR_CONFIG(capability_id_passthrough, local_buffer_role)
};

static const operator_endpoint_t inputs[] =
{
    {pre_processing_role, audio_input_left, 0},
    {pre_processing_role, audio_input_right, 1}
};

static const operator_endpoint_t outputs[] =
{
    {local_buffer_role, audio_output_left, 0},
    {local_buffer_role, audio_output_right, 1},
    {broadcast_buffer_role, ba_output, 0},
};

static const operator_connection_t connections[] =
{
    {pre_processing_role, 0, compander_role, 0, 2},
    {compander_role, 0, peq_role, 0, 2},
    {peq_role, 0, vse_role, 0, 2},
    {vse_role, 0, dbe_role, 0, 2},
    {dbe_role, 0, cancel_headroom_role, 0, 2},
    {cancel_headroom_role, 0, encoder_role, 0, 2},
    {encoder_role, 0, splitter_role, 0, 1},
    {splitter_role, 0, decoder_role, 0, 1},
    {splitter_role, 1, broadcast_buffer_role, 0, 1},
    {decoder_role, 0, local_buffer_role, 0, 2}
};

static const chain_config_t chain_config =
    MAKE_CHAIN_CONFIG(chain_id_audio_input_a2dp,
        audio_ucid_output_broadcast, op_config, inputs, outputs, connections);

const chain_config_t* AudioOutoutBaGetChainConfig(void)
{
    return &chain_config;
}
