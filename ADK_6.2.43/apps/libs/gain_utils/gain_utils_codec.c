/****************************************************************************
Copyright (c) 2004 - 2016 Qualcomm Technologies International, Ltd.

FILE NAME
    gain_utils.c
    
DESCRIPTION

*/

#include <stdlib.h>
#include <stream.h>
#include <source.h>
#include <sink.h>
#include <panic.h>

#include "gain_utils.h"

#define db_to_db_60(db)     (db * 60)
#define db_60_to_db(db_60)  (db_60 / 60)

void CodecSetInputGainNow(uint16 volume, codec_channel channel)
{
    Source input_source;
    
    if (channel != right_ch)
    {
        input_source = StreamAudioSource(AUDIO_HARDWARE_CODEC, AUDIO_INSTANCE_0, AUDIO_CHANNEL_A);
        SourceConfigure(input_source, STREAM_CODEC_INPUT_GAIN, volume); 
        SourceClose(input_source);
    }
    if (channel != left_ch)
    {
        input_source = StreamAudioSource(AUDIO_HARDWARE_CODEC, AUDIO_INSTANCE_0, AUDIO_CHANNEL_B);
        SourceConfigure(input_source, STREAM_CODEC_INPUT_GAIN, volume);             
        SourceClose(input_source);
    }
}

void CodecSetOutputGainNow(uint16 volume, codec_channel channel)
{
    Sink output_sink;
    
    if (channel == left_and_right_ch)
    {
        if (!StreamAudioSink(AUDIO_HARDWARE_CODEC, AUDIO_INSTANCE_0, AUDIO_CHANNEL_A))
        {
            output_sink = StreamAudioSink(AUDIO_HARDWARE_CODEC, AUDIO_INSTANCE_0, AUDIO_CHANNEL_A_AND_B);            
            SinkConfigure(output_sink, STREAM_CODEC_OUTPUT_GAIN, volume);
            SinkClose(output_sink);
            return;
        }            
    }
    
    if (channel != right_ch)
    {
        output_sink = StreamAudioSink(AUDIO_HARDWARE_CODEC, AUDIO_INSTANCE_0, AUDIO_CHANNEL_A);
        SinkConfigure(output_sink, STREAM_CODEC_OUTPUT_GAIN, volume);
        SinkClose(output_sink);
    }
    if (channel != left_ch)
    {
        output_sink = StreamAudioSink(AUDIO_HARDWARE_CODEC, AUDIO_INSTANCE_0, AUDIO_CHANNEL_B);
        SinkConfigure(output_sink, STREAM_CODEC_OUTPUT_GAIN, volume);   
        SinkClose(output_sink);
    }
}


uint16 gainUtilsCalculateRawAdcGainAnalogueComponent(uint16 dB_60)
{
    uint16 analogue_component = 0;
    switch(dB_60)
    {
        case db_to_db_60(0):
        case db_to_db_60(3):
        case db_to_db_60(6):
            analogue_component = ((db_60_to_db(dB_60) / 3) + 0x101);
            break;
        case db_to_db_60(9):
        case db_to_db_60(12):
        case db_to_db_60(15):
        case db_to_db_60(18):
            analogue_component = ((db_60_to_db(dB_60) / 3) + 0x111);
            break;
        case db_to_db_60(21):
        case db_to_db_60(24):
        case db_to_db_60(27):
        case db_to_db_60(30):
            analogue_component = ((db_60_to_db(dB_60) / 3) + 0x121);
            break;
        case db_to_db_60(33):
        case db_to_db_60(36):
        case db_to_db_60(39):
        case db_to_db_60(42):
            analogue_component = ((db_60_to_db(dB_60) / 3) + 0x131);
            break;
        default:
            analogue_component = ((0 / 3) + 0x101);
            break;
    }
    return analogue_component;
}

