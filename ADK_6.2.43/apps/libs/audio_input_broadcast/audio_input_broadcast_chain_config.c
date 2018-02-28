/******************************************************************************
Copyright (c) 2017 Qualcomm Technologies International, Ltd.

FILE NAME
    audio_input_broadcast_chain_connect.c

DESCRIPTION
    Definition of the operator chain for the broadcast audio receiver plugin.
*/

#include <operators.h>

#include "audio_input_broadcast_chain_config.h"


static const operator_config_t op_config_celt[] =
{
    MAKE_OPERATOR_CONFIG(capability_id_passthrough, input_buffer_role),
    MAKE_OPERATOR_CONFIG(capability_id_celt_decoder, decoder_role),
    MAKE_OPERATOR_CONFIG(capability_id_passthrough, pre_processing_role)
};

static const operator_endpoint_t inputs[] =
{
    {input_buffer_role, encoded_audio_input, 0}
};

static const operator_endpoint_t outputs[] =
{
    {pre_processing_role, decoded_audio_output_left, 0},
    {pre_processing_role, decoded_audio_output_right, 1}
};

static const operator_connection_t connections[] =
{
    {input_buffer_role, 0, decoder_role, 0, 1},
    {decoder_role, 0, pre_processing_role, 0, 2}
};

static const chain_config_t broadcast_chain_config_celt =
    MAKE_CHAIN_CONFIG(chain_id_audio_input_broadcast,
      audio_ucid_input_broadcast, op_config_celt, inputs, outputs, connections);


const chain_config_t* audioInputBroadcastGetChainConfig(void)
{
    return &broadcast_chain_config_celt;
}

