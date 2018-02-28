/****************************************************************************
Copyright (c) 2017 Qualcomm Technologies International, Ltd.

FILE NAME
    audio_output_broadcast.c

DESCRIPTION
    Audio output broadcast implementation.
*/
#include <string.h>

#include <message.h>
#include <stream.h>
#include <print.h>
#include <panic.h>

#include <vmtypes.h>
#include <audio_plugin_forwarding.h>
#include <audio_music_processing.h>
#include <broadcast_packetiser.h>
#include <broadcast_context.h>
#include <gain_utils.h>
#include <rtime.h>
#include <audio_config.h>
#include <gain_utils.h>
#include <operators.h>

#include "audio_plugin_output_variants.h"
#include "audio_output_broadcast_chain_config.h"
#include "audio_output_broadcast.h"

/* The erasure coded broadcast packet targets 2-DH5 packets */
#define BT_PACKET_2DH5_MAX_OCTETS 679

/* This is the minimum time before the TTP before which the receiver needs to receive
   the broadcast packet in order to decode/process the data and render */
#define RX_LATENCY_US 50000


typedef struct
{
    Task input_task;
    broadcast_packetiser_t *bp;
    ec_handle_tx_t ec;
    audio_plugin_forwarding_ports_t fw_ports;
    kymera_chain_handle_t chain;
} ba_output_ctx_t;

static ba_output_ctx_t ctx;

static TaskData broadcaster_plugin = {AudioOutputBroadcastMessageHandler};

static unsigned splitter_buffer_size = SPLITTER_BUFFER_SIZE;
static unsigned broadcast_pass_buffer_size = BROADCAST_BUFFER_SIZE;
static unsigned local_pass_buffer_size = LOCAL_BUFFER_SIZE;
static unsigned pre_processing_pass_buffer_size = PRE_PROCESSING_BUFFER_SIZE;

#ifdef TUNE_BUFFERS

void SetSplitterBufferSize(unsigned size);

void SetSplitterBufferSize(unsigned size)
{
    splitter_buffer_size = size;
}

void SetBroadcastPassBufferSize(unsigned size);

void SetBroadcastPassBufferSize(unsigned size)
{
    broadcast_pass_buffer_size = size;
}

void SetLocalPassBufferSize(unsigned size);

void SetLocalPassBufferSize(unsigned size)
{
    local_pass_buffer_size = size;
}

void SetPreProcessingPassBufferSize(unsigned size);

void SetPreProcessingPassBufferSize(unsigned size)
{
    pre_processing_pass_buffer_size = size;
}

#endif

Task AudioPluginOutputGetBroadcastPluginTask(void)
{
    return &broadcaster_plugin;
}

Task AudioPluginOutputGetTwsPluginTask(audio_plugin_output_mode_t mode)
{
    UNUSED(mode);

    return (Task)NULL;
}

/****************************************************************************
DESCRIPTION
    Converts content protection bit to pecketiser's type.
*/
static packetiser_helper_scmst_t getScmstFromContentProtection(bool cp)
{
    if(cp)
    {
        return packetiser_helper_scmst_copy_prohibited;
    }
    else
    {
        return packetiser_helper_scmst_copy_allowed;
    }
}

/****************************************************************************
DESCRIPTION
    Converts sample rate from Hz to rtime constants used by the packetiser.
*/
static rtime_sample_rate_t convertSampleRate(unsigned sample_rate)
{
    if(sample_rate == 44100)
    {
        return rtime_sample_rate_44100;
    }
    else if(sample_rate == 48000)
    {
        return rtime_sample_rate_48000;
    }
    else
    {
        Panic();
        return rtime_sample_rate_unknown;
    }
}

/****************************************************************************
DESCRIPTION
    Helper function to set the UCID of the passthrough, compander, mixer,
    vse and dbe
*/
static void musicProcessingSetUcid(kymera_chain_handle_t chain)
{
    Operator pass_op;
    Operator compander_op;
    Operator vse_op;
    Operator dbe_op;
    Operator peq_op;

    pass_op = ChainGetOperatorByRole(chain, pre_processing_role);
    if (pass_op)
    {
        OperatorsStandardSetUCID(pass_op, ucid_passthrough_processing);
    }

    compander_op = ChainGetOperatorByRole(chain, compander_role);
    if (compander_op)
    {
        OperatorsStandardSetUCID(compander_op, ucid_compander_processing);
    }

    vse_op = ChainGetOperatorByRole(chain, vse_role);
    if (vse_op)
    {
        OperatorsStandardSetUCID(vse_op, ucid_vse_processing);
    }

    dbe_op = ChainGetOperatorByRole(chain, dbe_role);
    if (dbe_op)
    {
        OperatorsStandardSetUCID(dbe_op, ucid_dbe_processing);
    }

    peq_op = ChainGetOperatorByRole(chain, peq_role);
    if (peq_op)
    {
        OperatorsStandardSetUCID(peq_op, ucid_peq_resampler_0);
    }
}

/****************************************************************************
DESCRIPTION
    Helper function to register music processing operators with music processing lib
*/
static void musicProcessingRegisterOperators(kymera_chain_handle_t chain)
{
    Operator compander_op = ChainGetOperatorByRole(chain, compander_role);
    Operator peq_op = ChainGetOperatorByRole(chain, peq_role);
    Operator vse_op = ChainGetOperatorByRole(chain, vse_role);
    Operator dbe_op = ChainGetOperatorByRole(chain, dbe_role);

    AudioMusicProcessingRegisterRole(audio_music_processing_compander_role, compander_op);
    AudioMusicProcessingRegisterRole(audio_music_processing_user_peq_role, peq_op);
    AudioMusicProcessingRegisterRole(audio_music_processing_volume_spatial_enhancement_role, vse_op);
    AudioMusicProcessingRegisterRole(audio_music_processing_dynamic_bass_enhancement_role, dbe_op);
}

/****************************************************************************
DESCRIPTION
    Unregister music processing operators from music processing lib
*/
static void musicProcessingUnregisterOperators(void)
{
    AudioMusicProcessingUnregisterRole(audio_music_processing_compander_role);
    AudioMusicProcessingUnregisterRole(audio_music_processing_user_peq_role);
    AudioMusicProcessingUnregisterRole(audio_music_processing_volume_spatial_enhancement_role);
    AudioMusicProcessingUnregisterRole(audio_music_processing_dynamic_bass_enhancement_role);
}

/****************************************************************************
DESCRIPTION
    Removes -12dB headroom. Headroom exists to avoid clipping in music processing operators.
    Headroom is cancelled in cancel_headroom_role and
    then restored in restore_headroom_role.
*/
static void cancelHeadroomBetweenOperators(kymera_chain_handle_t chain,
        unsigned cancel_headroom_role, unsigned restore_headroom_role)
{
    Operator op;

    op = ChainGetOperatorByRole(chain, cancel_headroom_role);
    OperatorsStandardSetUCID(op, ucid_passthrough_speaker);

    op = ChainGetOperatorByRole(chain, restore_headroom_role);
    OperatorsStandardSetUCID(op, ucid_passthrough_processing);
}

/****************************************************************************
DESCRIPTION
    Create Kymera chain.
*/
static kymera_chain_handle_t createChain(unsigned sample_rate)
{
    kymera_chain_handle_t chain;
    celt_encoder_params_t celt_params;
    Operator op;


    celt_params.sample_rate = sample_rate;
    celt_params.frame_size = sample_rate == 44100 ? CELT_CODEC_FRAME_SIZE_44100HZ : CELT_CODEC_FRAME_SIZE_48KHZ;

    chain = PanicNull(ChainCreate(AudioOutoutBaGetChainConfig()));

    musicProcessingSetUcid(chain);
    musicProcessingRegisterOperators(chain);

    cancelHeadroomBetweenOperators(chain, cancel_headroom_role, local_buffer_role);

    op = ChainGetOperatorByRole(chain, encoder_role);
    OperatorsCeltEncoderSetEncoderParams(op, &celt_params);

    op = ChainGetOperatorByRole(chain, splitter_role);
    PRINT(("splitter buffer size is %d\n", splitter_buffer_size));
    OperatorsConfigureSplitter(op, splitter_buffer_size, TRUE, operator_data_format_encoded);

    op = ChainGetOperatorByRole(chain, decoder_role);
    OperatorsCeltEncoderSetEncoderParams(op, &celt_params);
    
    op = ChainGetOperatorByRole(chain, broadcast_buffer_role);
    OperatorsSetPassthroughDataFormat(op, operator_data_format_encoded);
    PRINT(("broadcast pass buffer size is %d\n", broadcast_pass_buffer_size));
    OperatorsStandardSetBufferSize(op, broadcast_pass_buffer_size);

    op = ChainGetOperatorByRole(chain, local_buffer_role);
    OperatorsSetPassthroughDataFormat(op, operator_data_format_pcm);
    PRINT(("local pass buffer size is %d\n", local_pass_buffer_size));
    OperatorsStandardSetBufferSize(op, local_pass_buffer_size);

    op = ChainGetOperatorByRole(chain, pre_processing_role);
    PRINT(("pre-processing pass buffer size is %d\n", pre_processing_pass_buffer_size));
    OperatorsStandardSetBufferSize(op, pre_processing_pass_buffer_size);

    ChainConnect(chain);

    return chain;
}

/****************************************************************************
DESCRIPTION
    Populate packetiser configuration.
*/
static void setupPacketiserConfig(broadcast_packetiser_config_t *bp_config, bool content_protection, uint32 sample_rate)
{
    broadcast_encr_config_t *aes_config;
    ec_config_tx_t ec_config;


    memset(&ec_config, 0, sizeof(ec_config));
    ec_config.sink = BroadcastContextGetSink();
    ec_config.csb_interval = US_PER_SLOT * CSB_INTERVAL_SLOTS;
    ec_config.extra_latency = RX_LATENCY_US;
    ec_config.stream_id = (uint8)BroadcastContextGetStreamId();
    ec_config.params = &ec_params_2_5;
    ec_config.mtu = BT_PACKET_2DH5_MAX_OCTETS;
    ctx.ec = ErasureCodingTxInit(&ec_config);
    PanicNull(ctx.ec);
    BroadcastContextSetEcTxHandle(ctx.ec);

    memset(bp_config, 0, sizeof(*bp_config));
    bp_config->client_task = AudioPluginOutputGetBroadcastPluginTask();
    bp_config->ec_handle.tx = ctx.ec;
    bp_config->port.source = ChainGetOutput(ctx.chain, ba_output);
    bp_config->scmst = getScmstFromContentProtection(content_protection);
    bp_config->sample_rate = convertSampleRate(sample_rate);
    if(rtime_sample_rate_44100 == bp_config->sample_rate)
    {
        bp_config->frame_length = CELT_CODEC_FRAME_SIZE_44100HZ;
        bp_config->frame_samples = CELT_CODEC_FRAME_SAMPLES_44100HZ;
    }
    else
    {
        bp_config->frame_length = CELT_CODEC_FRAME_SIZE_48KHZ;
        bp_config->frame_samples = CELT_CODEC_FRAME_SAMPLES_48KHZ;
    }
    bp_config->stats_interval = US_PER_SEC * DEFAULT_STATS_INTERVAL_IN_SEC;

    aes_config = BroadcastContextGetEncryptionConfig();
    if(aes_config)
    {
        bp_config->aesccm_disabled = FALSE;
        memcpy(bp_config->aesccm.key, &aes_config->seckey[1], sizeof(bp_config->aesccm.key));
        memcpy(bp_config->aesccm.fixed_iv, aes_config->fixed_iv, sizeof(bp_config->aesccm.fixed_iv));
        bp_config->aesccm.dynamic_iv = aes_config->variant_iv;
    }
    else
    {
        bp_config->aesccm_disabled = TRUE;
    }
}

/****************************************************************************
DESCRIPTION
    Create handler.
*/
static void handleCreate(Message message)
{
    AUDIO_PLUGIN_FORWARDING_CREATE_REQ_T *msg = (AUDIO_PLUGIN_FORWARDING_CREATE_REQ_T *)message;
    broadcast_packetiser_config_t config;

    memset(&ctx, 0, sizeof(ctx));

    ctx.chain = createChain(msg->params.sample_rate);

    ctx.fw_ports.left_source = ChainGetOutput(ctx.chain, audio_output_left);
    ctx.fw_ports.right_source = ChainGetOutput(ctx.chain, audio_output_right);

    ctx.input_task = msg->input_task;

    StreamConnect(msg->params.forwarding.pcm.left_source, ChainGetInput(ctx.chain, audio_input_left));
    StreamConnect(msg->params.forwarding.pcm.right_source, ChainGetInput(ctx.chain, audio_input_right));

    setupPacketiserConfig(&config, msg->params.content_protection, msg->params.sample_rate);

    PanicFalse(SourceMapInit(config.port.source, STREAM_TIMESTAMPED, AUDIO_FRAME_METADATA_LENGTH));

    BroadcastPacketiserInit(&config);
}

/****************************************************************************
DESCRIPTION
    Handler for packetiser's init confirmation.
*/
static void handleInitCfm(Message message)
{
    BROADCAST_PACKETISER_INIT_CFM_T *msg = (BROADCAST_PACKETISER_INIT_CFM_T *)message;

    audio_plugin_forwarding_status_t status;

    if(msg->initialisation_success)
    {
        status = audio_output_success;
        ctx.bp = msg->broadcast_packetiser;
        BroadcastContextSetBroadcastBusy(TRUE);
    }
    else
    {
        status = audio_output_failed_packetiser;
    }

    AudioPluginForwardingCreateCfm(AudioPluginOutputGetBroadcastPluginTask(),
                ctx.input_task, &ctx.fw_ports, status);
}

/****************************************************************************
DESCRIPTION
    Start handler.
*/
static void handleStart(Message message)
{
    AUDIO_PLUGIN_FORWARDING_START_REQ_T *msg = (AUDIO_PLUGIN_FORWARDING_START_REQ_T *)message;

    ChainStart(ctx.chain);

    AudioPluginForwardingStartCfm(AudioPluginOutputGetBroadcastPluginTask(), msg->input_task);
}

/****************************************************************************
DESCRIPTION
    Stop handler.
*/
static void handleStop(Message message)
{
    AUDIO_PLUGIN_FORWARDING_STOP_REQ_T *msg = (AUDIO_PLUGIN_FORWARDING_STOP_REQ_T *)message;

    ChainStop(ctx.chain);

    AudioPluginForwardingStopCfm(AudioPluginOutputGetBroadcastPluginTask(), msg->input_task);
}

/****************************************************************************
DESCRIPTION
    Destroy handler.
*/
static void handleDestory(Message message)
{
    AUDIO_PLUGIN_FORWARDING_DESTROY_REQ_T *msg = (AUDIO_PLUGIN_FORWARDING_DESTROY_REQ_T *)message;

    ErasureCodingTxDestroy(ctx.ec);
    ctx.ec = NULL;
    BroadcastContextSetEcTxHandle(ctx.ec);

    BroadcastPacketiserDestroy(ctx.bp);
    ctx.bp= NULL;

    SourceUnmap(ChainGetOutput(ctx.chain, ba_output));

    BroadcastContextSetBroadcastBusy(FALSE);

    musicProcessingUnregisterOperators();

    if(ctx.chain)
    {
       ChainDestroy(ctx.chain);
       ctx.chain = 0;
    }

    AudioPluginForwardingDestroyCfm(AudioPluginOutputGetBroadcastPluginTask(), msg->input_task);
}

/****************************************************************************
DESCRIPTION
    Volume change indication handler.
*/
static void handleVolumeChange(void)
{
    int16 volume_in_db = (int16)AudioConfigGetMasterVolume();
    uint16 volume_in_ba_steps = ConvertdBToBroadcastVolume(volume_in_db, BroadcastContextGetVolumeTable());
    PRINT(("volume update dB %d, steps %d\n", volume_in_db, volume_in_ba_steps));
    BroadcastPacketiserSetVolume(ctx.bp, volume_in_ba_steps);
}

/****************************************************************************
DESCRIPTION
    Returns operator which performs broadcast mute function.
*/
static Operator getMuteOperator(void)
{
    return ChainGetOperatorByRole(ctx.chain, cancel_headroom_role);
}

/****************************************************************************
DESCRIPTION
    Mute status indication handler.
*/
static void handleMute(Message message)
{
    AUDIO_PLUGIN_FORWARDING_SOFT_MUTE_IND_T *msg = (AUDIO_PLUGIN_FORWARDING_SOFT_MUTE_IND_T *)message;

    PRINT(("Setting broadcast mute %d\n", msg->muted));

    OperatorsStandardFadeOut(getMuteOperator(), msg->muted);
}

/****************************************************************************
DESCRIPTION
    Returns true when message was sent by an audio input plugin.
*/
static bool isItAudioForwardingMessage(MessageId id)
{
    if(id >= AUDIO_PLUGIN_FORWARDING_BASE && id < AUDIO_PLUGIN_FORWARDING_TOP)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/****************************************************************************
DESCRIPTION
    Handler for messages form an audio input plugin.
*/
static void handleAudioForwardingMessages(MessageId id, Message message)
{
    switch(id)
    {
    case AUDIO_PLUGIN_FORWARDING_CREATE_REQ:
        handleCreate(message);
        break;

    case AUDIO_PLUGIN_FORWARDING_START_REQ:
        handleStart(message);
        break;

    case AUDIO_PLUGIN_FORWARDING_STOP_REQ:
        handleStop(message);
        break;

    case AUDIO_PLUGIN_FORWARDING_DESTROY_REQ:
        handleDestory(message);
        break;

    case AUDIO_PLUGIN_FORWARDING_VOLUME_CHANGE_IND:
        handleVolumeChange();
        break;

    case AUDIO_PLUGIN_FORWARDING_SOFT_MUTE_IND:
        handleMute(message);
        break;

    default:
        PRINT(("Unhandled audio forwarding msg id 0x%x\n", id));
        break;
    }
}

/****************************************************************************
DESCRIPTION
    Returns true when message was sent by the packetiser.
*/
static bool isItPacketiserMessage(MessageId id)
{
    if(id >= BROADCAST_PACKETISER_MESSAGE_BASE && id < BROADCAST_PACKETISER_MESSAGE_TOP)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/****************************************************************************
DESCRIPTION
    Print out packetiser's statistics.
*/
static void handleStats(Message message)
{
#ifdef DEBUG_PRINT_ENABLED
    BROADCAST_PACKETISER_STATS_BROADCASTER_IND_T *msg = (BROADCAST_PACKETISER_STATS_BROADCASTER_IND_T *)message;
    PRINT(("packets transmitted %d\n", msg->stats.broadcast_packets_transmitted));
    PRINT(("frames transmitted %d\n", msg->stats.audio_frames_transmitted));
    PRINT(("frames late %d\n", msg->stats.audio_frames_late));
#else
    UNUSED(message);
#endif

}

/****************************************************************************
DESCRIPTION
    Handler for packetiser's messages.
*/
static void handlePacketiserMessages(MessageId id, Message message)
{
    switch(id)
    {
    case BROADCAST_PACKETISER_INIT_CFM:
        handleInitCfm(message);
        break;

    case BROADCAST_PACKETISER_STATS_BROADCASTER_IND:
        handleStats(message);
        break;

    default:
        PRINT(("Unhandled broadcast packetiser msg id 0x%x\n", id));
        break;
    }
}

void AudioOutputBroadcastMessageHandler(Task task, MessageId id, Message message)
{
    UNUSED(task);

    if(isItAudioForwardingMessage(id))
    {
        handleAudioForwardingMessages(id, message);
    }
    else if(isItPacketiserMessage(id))
    {
        handlePacketiserMessages(id, message);
    }
}
