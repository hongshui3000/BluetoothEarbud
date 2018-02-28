/* Copyright (c) 2016 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */
/************************************************************************
FILE
     voice_if.h

CONTAINS
     The global definitions for the voice codecs.

DESCRIPTION
     This file is seen by the stack, and VM applications, and
     contains things that are common between them.

************************************************************************/

#ifndef __APP_VOICE_IF_H__
#define __APP_VOICE_IF_H__

/* The BCCMD voice_codec goes by these values. */
enum voice_codec_enum {
    VOICE_CODEC_ADPCM_4BITS_PER_SAMPLE = 1, /*!< Select ADPCM with 4 bits per sample. */
    VOICE_CODEC_ADPCM_2BITS_PER_SAMPLE = 2  /*!< Select ADPCM with 2 bits per sample. */

#ifndef HIDE_TROUBLESOME_VOICE_CODEC_SETTINGS
    /*
       Subsequent settings are for debug only, or fundamentally
       broken. We don't publish them, and they are generaly only
       enabled in debug firmware builds with special #define's in
       effect.

       See B-19704.
    */
#ifdef DEBUG_CVSD_CODEC_SELECTION
    ,VOICE_CODEC_CVSD = 0x4000
#endif
#ifdef DEBUG_ODD_ADPCM_CODEC_SELECTION
    ,VOICE_CODEC_ADPCM_3BITS_PER_SAMPLE = 0x4001
    ,VOICE_CODEC_ADPCM_5BITS_PER_SAMPLE = 0x4002
#endif
#endif
};

typedef enum voice_codec_enum voice_codec;

#endif
