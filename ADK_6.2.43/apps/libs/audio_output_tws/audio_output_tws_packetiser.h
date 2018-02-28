/****************************************************************************
Copyright (c) 2017 Qualcomm Technologies International, Ltd.
Part of 6.2

FILE NAME
    audio_output_tws_packetiser.h

DESCRIPTION
    TWS packetiser functionality
*/

#ifndef AUDIO_OUTPUT_TWS_PACKETISER_H_
#define AUDIO_OUTPUT_TWS_PACKETISER_H_

bool AudioOutputTwsPacketiserInit(Task output_task, Source forwarding_source, const audio_plugin_forwarding_params_t* params);

void AudioOutputTwsPacketiserDestroy(void);

void AudioOutputTwsPacketiserReset(void);

#endif /* AUDIO_OUTPUT_TWS_PACKETISER_H_ */
