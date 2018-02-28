/****************************************************************************
Copyright (c) 2017 Qualcomm Technologies International, Ltd.

FILE NAME
    audio_input_tws_chain_config.c

DESCRIPTION
    Implementation of function to return the appropriate chain configuration data.
*/

#include <operators.h>
#include <panic.h>
#include "audio_input_tws_chain_config.h"

static const operator_config_t op_config_sbc[] =
{
    MAKE_OPERATOR_CONFIG(capability_id_passthrough, passthrough_role),
    MAKE_OPERATOR_CONFIG(capability_id_sbc_decoder, decoder_role)
};
static const operator_config_t op_config_aptx[] =
{
    MAKE_OPERATOR_CONFIG(capability_id_passthrough, passthrough_role),
    MAKE_OPERATOR_CONFIG(capability_id_aptx_decoder, decoder_role)
};
static const operator_config_t op_config_aac[] =
{
    MAKE_OPERATOR_CONFIG(capability_id_passthrough, passthrough_role),
    MAKE_OPERATOR_CONFIG(capability_id_aac_decoder, decoder_role)
};

static const operator_endpoint_t inputs[] =
{
    {passthrough_role, encoded_audio_input, 0}
};

static const operator_endpoint_t outputs[] =
{
    {decoder_role, decoded_audio_output_left, 0},
    {decoder_role, decoded_audio_output_right, 1},
};

static const operator_connection_t connections[] =
{
    {passthrough_role, 0, decoder_role, 0, 1},
};

static const chain_config_t tws_chain_config_sbc =
    MAKE_CHAIN_CONFIG(chain_id_audio_input_tws, audio_ucid_input_a2dp,
                      op_config_sbc, inputs, outputs, connections);

static const chain_config_t tws_chain_config_aptx =
    MAKE_CHAIN_CONFIG(chain_id_audio_input_tws, audio_ucid_input_a2dp_hq_codec,
                      op_config_aptx, inputs, outputs, connections);

static const chain_config_t tws_chain_config_aac =
    MAKE_CHAIN_CONFIG(chain_id_audio_input_tws, audio_ucid_input_a2dp,
                      op_config_aac, inputs, outputs, connections);

const chain_config_t* AudioInputTwsGetChainConfig(audio_input_tws_decoder_t decoder)
{
    const chain_config_t *config = NULL;

    switch(decoder)
    {
        case audio_input_tws_sbc:
            config = &tws_chain_config_sbc;
            break;
        case audio_input_tws_aptx:
            config = &tws_chain_config_aptx;
            break;
        case audio_input_tws_aac:
            config = &tws_chain_config_aac;
            break;
        default:
            Panic();
    }
    return config;
}
