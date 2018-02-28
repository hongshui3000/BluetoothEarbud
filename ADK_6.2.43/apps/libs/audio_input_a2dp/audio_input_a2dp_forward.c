/****************************************************************************
Copyright (c) 2017 Qualcomm Technologies International, Ltd.
Part of 6.2

FILE NAME
    audio_input_a2dp_forward.c

DESCRIPTION
    Handler for AUDIO_PLUGIN_START_FORWARDING_MSG message for a2dp source.
*/

#include <audio.h>
#include <audio_config.h>
#include <audio_plugin_forwarding.h>
#include <audio_mixer.h>
#include <operators.h>

#include "audio_config.h"
#include "audio_input_a2dp_chain_config.h"
#include "audio_input_a2dp_task.h"
#include "audio_input_a2dp_connect.h"
#include "audio_input_a2dp_forward.h"


static bool codecCanBeForwarded(audio_codec_t codec)
{
    if(codec == audio_codec_sbc || codec == audio_codec_aptx || 
            codec == audio_codec_aac)
        return TRUE;

    return FALSE;
}

static void sendLatencyUpdateToAppTask(audio_input_context_t* ctx, uint16 target_ttp_in_tenths_of_ms)
{
    MAKE_AUDIO_MESSAGE(AUDIO_LATENCY_REPORT, message);
    message->latency = target_ttp_in_tenths_of_ms;
    message->estimated = TRUE;
    message->sink = StreamSinkFromSource(ctx->left_source);
    MessageSend(ctx->app_task, AUDIO_LATENCY_REPORT, message);
}

void audioInputA2dpForwardCreate(Task task, AUDIO_PLUGIN_START_FORWARDING_MSG_T* req, audio_input_context_t* ctx)
{
    audio_plugin_forwarding_params_t params;
    params.source_codec = AudioPluginA2dpTaskGetCodec(task);

    if(codecCanBeForwarded(params.source_codec))
    {
        Operator rtp_op = ChainGetOperatorByRole(ctx->chain, rtp_role);
        ctx->tws.plugin = req->output_plugin;

        params.forwarding.codec_source = ChainGetOutput(ctx->chain, stream_forwarding);
        params.forwarding_sink = req->forwarding_sink;
        params.content_protection = req->content_protection;
        params.sample_rate = ctx->sample_rate;
        params.transcode.codec = audio_codec_none;
        params.ttp_latency = AudioConfigGetTWSTtpLatency();
        AudioPluginForwardingCreate(task, ctx->tws.plugin, &params);

        audioInputConfigureRTPLatency(rtp_op, &params.ttp_latency);

        SetAudioBusy(task);
    }
}

void audioInputA2dpForwardStart(Task task, audio_input_context_t* ctx, AUDIO_PLUGIN_FORWARDING_CREATE_CFM_T* cfm)
{
    if(cfm->status == audio_output_success)
    {
        uint16 target_ttp_in_tenths_of_ms = (uint16)(AudioConfigGetTWSTtpLatency().target_in_ms * 10);

        AudioPluginForwardingStart(task, ctx->tws.plugin);
        sendLatencyUpdateToAppTask(ctx, target_ttp_in_tenths_of_ms);
        AudioMixerUpdateChannelMode(AudioConfigGetTwsChannelModeLocal());
        return;
    }

    /* Reset tws state, forwarding failed */
    ctx->tws.plugin = NULL;
    SetAudioBusy(NULL);
}

void AudioPluginA2dpForwardEnableSecondOutput(audio_input_context_t* ctx)
{
    Operator splitter = ChainGetOperatorByRole(ctx->chain, splitter_role);
    OperatorsSplitterEnableSecondOutput(splitter, TRUE);
    SetAudioBusy(NULL);
}

void audioInputA2dpForwardDestroy(Task task, audio_input_context_t* ctx)
{
    Operator rtp_op = ChainGetOperatorByRole(ctx->chain, rtp_role);
    ttp_latency_t ttp_latency = AudioConfigGetA2DPTtpLatency();
    uint16 target_ttp_in_tenths_of_ms = (uint16)(ttp_latency.target_in_ms * 10);

    SetAudioBusy(task);
    Operator splitter = ChainGetOperatorByRole(ctx->chain, splitter_role);
    OperatorsSplitterEnableSecondOutput(splitter, FALSE);
    AudioMixerUpdateChannelMode(CHANNEL_MODE_STEREO);
    audioInputConfigureRTPLatency(rtp_op, &ttp_latency);
    sendLatencyUpdateToAppTask(ctx, target_ttp_in_tenths_of_ms);
    AudioPluginForwardingDestroy(task, ctx->tws.plugin);
}

void AudioPluginA2dpForwardHandleDestroyCfm(audio_input_context_t* ctx)
{
    SetAudioBusy(NULL);
    ctx->tws.plugin = NULL;
}
