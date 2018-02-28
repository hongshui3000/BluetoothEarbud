/****************************************************************************
Copyright (c) 2016 - 2017 Qualcomm Technologies International, Ltd.
Part of 6.2

FILE NAME
    audio_input_a2dp_connect.h

DESCRIPTION
    Handler for AUDIO_PLUGIN_CONNECT_MSG message for a2dp source.
*/

#ifndef AUDIO_INPUT_A2DP_CONNECT_H_
#define AUDIO_INPUT_A2DP_CONNECT_H_

#include <message.h>
#include <audio_plugin_if.h>
#include <audio_input_common.h>

void AudioInputA2dpConnectHandler(audio_input_context_t *ctx, Task task,
        const AUDIO_PLUGIN_CONNECT_MSG_T *msg);

void AudioInputA2dpTestReset(void);

uint16 audioInputA2dpGetLatency(audio_input_context_t *ctx, Task audio_plugin, bool *estimated);

void audioInputConfigureRTPLatency(Operator rtp_op, ttp_latency_t *ttp_latency);

#endif /* AUDIO_INPUT_A2DP_CONNECT_H_ */
