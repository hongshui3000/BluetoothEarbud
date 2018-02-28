/****************************************************************************
Copyright (c) 2016 Qualcomm Technologies International, Ltd.

FILE NAME
    audio_mixer_spaker_configs.c

DESCRIPTION
    Implementation of functions to return the appropriate chain configuration.
*/

#include <stddef.h>

#include <panic.h>
#include <operators.h>

#include "audio_mixer.h"
#include "audio_mixer_speaker_chain_configs.h"
#include "audio_mixer_speaker.h"
#include "audio_mixer_channel_roles.h"
#include "audio_mixer_connection.h"

#define VOLUME_INPUT_MAIN1          (0)
#define VOLUME_INPUT_AUX1           (1)
#define VOLUME_INPUT_MAIN2          (2)
#define VOLUME_INPUT_MAIN3          (4)
#define VOLUME_INPUT_MAIN4          (6)

#define VOLUME_OUTPUT1              (0)
#define VOLUME_OUTPUT2              (1)
#define VOLUME_OUTPUT3              (2)
#define VOLUME_OUTPUT4              (3)

#define LEFT_INPUT                  (0)
#define RIGHT_INPUT                 (1)
#define LEFT_OUTPUT                 (0)
#define RIGHT_OUTPUT                (1)

#define PRIMARY_LEFT_INPUT          (0)
#define PRIMARY_RIGHT_INPUT         (1)
#define SECONDARY_LEFT_INPUT        (2)
#define SECONDARY_RIGHT_INPUT       (3)

#define PRIMARY_LEFT_OUTPUT         (0)
#define PRIMARY_RIGHT_OUTPUT        (1)
#define SECONDARY_LEFT_OUTPUT       (2)
#define SECONDARY_RIGHT_OUTPUT      (3)

#define CROSSOVER_LEFT_LOW          (0)
#define CROSSOVER_LEFT_HIGH         (1)
#define CROSSOVER_RIGHT_LOW         (2)
#define CROSSOVER_RIGHT_HIGH        (3)

#define UNUSED_TERMINAL             (0xff)

#define MAKE_SPEAKER_OPS_COMMON() \
    MAKE_OPERATOR_CONFIG(capability_id_volume,      media_volume_role), \
    MAKE_OPERATOR_CONFIG(capability_id_peq,         speaker_peq_role), \
    MAKE_OPERATOR_CONFIG(capability_id_passthrough, post_processing_role)

/* Mono output single channel */
static const operator_config_t speaker_ops_mono[] =
{
    MAKE_SPEAKER_OPS_COMMON(), 
    MAKE_OPERATOR_CONFIG(capability_id_volume, master_volume_role),
    MAKE_OPERATOR_CONFIG(capability_id_mixer,  stereo_to_mono_role)
};

static const operator_stream_node_t left_dmix_primary_left[] =
{
    {media_volume_role,    VOLUME_INPUT_MAIN1, VOLUME_OUTPUT1},
    {speaker_peq_role,     LEFT_INPUT,         LEFT_OUTPUT},
    {stereo_to_mono_role,  LEFT_INPUT,         LEFT_OUTPUT},
    {master_volume_role,   VOLUME_INPUT_MAIN1, VOLUME_OUTPUT1},
    {post_processing_role, PRIMARY_LEFT_INPUT, PRIMARY_LEFT_OUTPUT}
};

static const operator_stream_node_t right_dmix_primary_left[] =
{
    {media_volume_role,   VOLUME_INPUT_MAIN2, VOLUME_OUTPUT2},
    {speaker_peq_role,    RIGHT_INPUT,        RIGHT_OUTPUT},
    {stereo_to_mono_role, RIGHT_INPUT,        UNUSED_TERMINAL}
};

static const operator_stream_node_t prompt_mix[] = 
{
    { media_volume_role, VOLUME_INPUT_AUX1, UNUSED_TERMINAL}
};

static const operator_stream_t streams_mono[] = 
{
    {left_channel,        stream_with_in_and_out, ARRAY_DIM((left_dmix_primary_left)),  left_dmix_primary_left},
    {right_channel,       stream_with_input,      ARRAY_DIM((right_dmix_primary_left)), right_dmix_primary_left},
    {prompt_left_channel, stream_with_input,      ARRAY_DIM((prompt_mix)),              prompt_mix}
};

static const chain_config_t speaker_config_mono =
    MAKE_CHAIN_CONFIG_WITH_STREAMS(chain_id_audio_mixer_speaker,
                                   audio_ucid_mixer_speaker_mono,
                                   speaker_ops_mono, 
                                   streams_mono);

/* Stereo output two channels */
static const operator_config_t speaker_ops_stereo[] =
{
    MAKE_SPEAKER_OPS_COMMON()
};

static const operator_stream_node_t left_primary_left[] =
{
    {media_volume_role,    VOLUME_INPUT_MAIN1, VOLUME_OUTPUT1},
    {speaker_peq_role,     LEFT_INPUT,         LEFT_OUTPUT},
    {post_processing_role, PRIMARY_LEFT_INPUT, PRIMARY_LEFT_OUTPUT}
};

static const operator_stream_node_t right_primary_right[] =
{
    {media_volume_role,    VOLUME_INPUT_MAIN2,  VOLUME_OUTPUT2},
    {speaker_peq_role,     RIGHT_INPUT,         RIGHT_OUTPUT},
    {post_processing_role, PRIMARY_RIGHT_INPUT, PRIMARY_RIGHT_OUTPUT}
};

static const operator_stream_t streams_stereo[] = 
{
    {left_channel,        stream_with_in_and_out, ARRAY_DIM((left_primary_left)),   left_primary_left},
    {right_channel,       stream_with_in_and_out, ARRAY_DIM((right_primary_right)), right_primary_right},
    {prompt_left_channel, stream_with_input,      ARRAY_DIM((prompt_mix)),          prompt_mix}
};

static const chain_config_t speaker_config_stereo =
    MAKE_CHAIN_CONFIG_WITH_STREAMS(chain_id_audio_mixer_speaker,
                                   audio_ucid_mixer_speaker_stereo,
                                   speaker_ops_stereo, 
                                   streams_stereo);

/* Stereo output two channels + bass channel */
static const operator_config_t speaker_ops_stereo_bass[] =
{
    MAKE_SPEAKER_OPS_COMMON(), 
    MAKE_OPERATOR_CONFIG(capability_id_volume,          master_volume_role),
    MAKE_OPERATOR_CONFIG(capability_id_crossover_2band, crossover_role),
    MAKE_OPERATOR_CONFIG(capability_id_mixer,           stereo_to_mono_role),
    MAKE_OPERATOR_CONFIG(capability_id_compander,       compander_role)
};

static const operator_stream_node_t xover_high_freq_pri_left[] =
{
    {media_volume_role,    VOLUME_INPUT_MAIN1, VOLUME_OUTPUT1},
    {speaker_peq_role,     LEFT_INPUT,         LEFT_OUTPUT},
    {crossover_role,       LEFT_INPUT,         CROSSOVER_LEFT_HIGH},
    {master_volume_role,   VOLUME_INPUT_MAIN1, VOLUME_OUTPUT1},
    {post_processing_role, PRIMARY_LEFT_INPUT, PRIMARY_LEFT_OUTPUT}
};

static const operator_stream_node_t xover_high_freq_pri_right[] =
{
    {media_volume_role,    VOLUME_INPUT_MAIN2,  VOLUME_OUTPUT2},
    {speaker_peq_role,     RIGHT_INPUT,         RIGHT_OUTPUT},
    {crossover_role,       RIGHT_INPUT,         CROSSOVER_RIGHT_HIGH},
    {master_volume_role,   VOLUME_INPUT_MAIN2,  VOLUME_OUTPUT2},
    {post_processing_role, PRIMARY_RIGHT_INPUT, PRIMARY_RIGHT_OUTPUT}
};

static const operator_stream_node_t xover_low_freq_left_dmix_sec_left[] =
{
    {crossover_role,        UNUSED_TERMINAL,        CROSSOVER_LEFT_LOW},
    {stereo_to_mono_role,   LEFT_INPUT,             LEFT_OUTPUT},
    {master_volume_role,    VOLUME_INPUT_MAIN3,     VOLUME_OUTPUT3},
    {compander_role,        LEFT_INPUT,             LEFT_OUTPUT},
    {post_processing_role,  SECONDARY_LEFT_INPUT,   SECONDARY_LEFT_OUTPUT}
};

static const operator_stream_node_t xover_low_freq_right_dmix_sec_left[] =
{
    {crossover_role,       UNUSED_TERMINAL, CROSSOVER_RIGHT_LOW},
    {stereo_to_mono_role,  RIGHT_INPUT,     UNUSED_TERMINAL}
};

static const operator_stream_t streams_stereo_bass[] = 
{
    {left_channel,              stream_with_in_and_out,   ARRAY_DIM((xover_high_freq_pri_left)),            xover_high_freq_pri_left},
    {right_channel,             stream_with_in_and_out,   ARRAY_DIM((xover_high_freq_pri_right)),           xover_high_freq_pri_right},
    {secondary_left_channel,    stream_with_output,       ARRAY_DIM((xover_low_freq_left_dmix_sec_left)),   xover_low_freq_left_dmix_sec_left},
    {secondary_right_channel,   stream_with_no_in_or_out, ARRAY_DIM((xover_low_freq_right_dmix_sec_left)),  xover_low_freq_right_dmix_sec_left},
    {prompt_left_channel,       stream_with_input,        ARRAY_DIM((prompt_mix)),                          prompt_mix}
};

static const chain_config_t speaker_config_stereo_bass =
    MAKE_CHAIN_CONFIG_WITH_STREAMS(chain_id_audio_mixer_speaker,
                                   audio_ucid_mixer_speaker_stereo_bass,
                                   speaker_ops_stereo_bass, 
                                   streams_stereo_bass);

/* Stereo output two channels with separate LF and HF outputs */
static const operator_config_t speaker_ops_stereo_xover[] =
{
    MAKE_SPEAKER_OPS_COMMON(), 
    MAKE_OPERATOR_CONFIG(capability_id_volume,          master_volume_role),
    MAKE_OPERATOR_CONFIG(capability_id_crossover_2band, crossover_role),
    MAKE_OPERATOR_CONFIG(capability_id_compander,       compander_role)
};

static const operator_stream_node_t xover_low_freq_pri_left[] =
{
    {media_volume_role,     VOLUME_INPUT_MAIN1,     VOLUME_OUTPUT1},
    {speaker_peq_role,      LEFT_INPUT,             LEFT_OUTPUT},
    {crossover_role,        LEFT_INPUT,             CROSSOVER_LEFT_LOW},
    {master_volume_role,    VOLUME_INPUT_MAIN3,     VOLUME_OUTPUT3},
    {compander_role,        LEFT_INPUT,             LEFT_OUTPUT},
    {post_processing_role,  PRIMARY_LEFT_INPUT,     PRIMARY_LEFT_OUTPUT}
};

static const operator_stream_node_t xover_low_freq_pri_right[] =
{
    {media_volume_role,     VOLUME_INPUT_MAIN2,     VOLUME_OUTPUT2},
    {speaker_peq_role,      RIGHT_INPUT,            RIGHT_OUTPUT},
    {crossover_role,        RIGHT_INPUT,            CROSSOVER_RIGHT_LOW},
    {master_volume_role,    VOLUME_INPUT_MAIN4,     VOLUME_OUTPUT4},
    {compander_role,        RIGHT_INPUT,            RIGHT_OUTPUT},
    {post_processing_role,  PRIMARY_RIGHT_INPUT,    PRIMARY_RIGHT_OUTPUT}
};

static const operator_stream_node_t xover_high_freq_sec_left[] =
{
    {crossover_role,       UNUSED_TERMINAL,         CROSSOVER_LEFT_HIGH},
    {master_volume_role,   VOLUME_INPUT_MAIN1,      VOLUME_OUTPUT1},
    {post_processing_role, SECONDARY_LEFT_INPUT,    SECONDARY_LEFT_OUTPUT}
};

static const operator_stream_node_t xover_high_freq_sec_right[] =
{
    {crossover_role,       UNUSED_TERMINAL,         CROSSOVER_RIGHT_HIGH},
    {master_volume_role,   VOLUME_INPUT_MAIN2,      VOLUME_OUTPUT2},
    {post_processing_role, SECONDARY_RIGHT_INPUT,   SECONDARY_RIGHT_OUTPUT}
};

static const operator_stream_t streams_stereo_xover[] = 
{
    {left_channel,              stream_with_in_and_out,   ARRAY_DIM((xover_low_freq_pri_left)),     xover_low_freq_pri_left},
    {right_channel,             stream_with_in_and_out,   ARRAY_DIM((xover_low_freq_pri_right)),    xover_low_freq_pri_right},
    {secondary_left_channel,    stream_with_output,       ARRAY_DIM((xover_high_freq_sec_left)),    xover_high_freq_sec_left},
    {secondary_right_channel,   stream_with_output,       ARRAY_DIM((xover_high_freq_sec_right)),   xover_high_freq_sec_right},
    {prompt_left_channel,       stream_with_input,        ARRAY_DIM((prompt_mix)),                  prompt_mix}
};

static const chain_config_t speaker_config_stereo_xover =
    MAKE_CHAIN_CONFIG_WITH_STREAMS(chain_id_audio_mixer_speaker,
                                   audio_ucid_mixer_speaker_stereo_xover,
                                   speaker_ops_stereo_xover, 
                                   streams_stereo_xover);

/* Speaker chain voice only */
static const operator_config_t speaker_ops_low_power[] =
{
    MAKE_OPERATOR_CONFIG(capability_id_volume, media_volume_role)
};

static const operator_stream_node_t left_low_power[] =
{
    {media_volume_role,    VOLUME_INPUT_MAIN1, VOLUME_OUTPUT1}
};

static const operator_stream_t streams_voice_only[] = 
{
    {left_channel,              stream_with_in_and_out,   ARRAY_DIM((left_low_power)),  left_low_power},
    {prompt_left_channel,       stream_with_input,        ARRAY_DIM((prompt_mix)),      prompt_mix}
};

static const chain_config_t speaker_config_voice_only =
    MAKE_CHAIN_CONFIG_WITH_STREAMS(chain_id_audio_mixer_speaker,
                                   audio_ucid_mixer_speaker_voice_only,
                                   speaker_ops_low_power, 
                                   streams_voice_only);

/* Stereo output two channels */
static const operator_config_t speaker_ops_low_power_filter[] = 
{
    MAKE_OPERATOR_CONFIG(capability_id_none, speaker_peq_role),
    MAKE_OPERATOR_CONFIG(capability_id_none, post_processing_role)
};

static const operator_filters_t speaker_low_power_filter = 
{
    ARRAY_DIM((speaker_ops_low_power_filter)),  speaker_ops_low_power_filter
};

/******************************************************************************/
const chain_config_t *mixerSpeakerGetChainConfig(audio_mixer_speaker_configuration_t chain_type)
{
    switch(chain_type)
    {
        case audio_mixer_speaker_mono:
            return &speaker_config_mono;
            
        case audio_mixer_speaker_stereo:
            return &speaker_config_stereo;
            
        case audio_mixer_speaker_stereo_bass:
            return &speaker_config_stereo_bass;
            
        case audio_mixer_speaker_stereo_xover:
            return &speaker_config_stereo_xover;
            
        case audio_mixer_speaker_voice_only:
            return &speaker_config_voice_only;
        
        default:
            Panic();
            return NULL;
    }
}

const operator_filters_t *mixerSpeakerGetChainFilter(connection_type_t connection_type)
{
    switch(connection_type)
    {
        case CONNECTION_TYPE_VOICE:
            /* Voice chain is independent and never needs filtering */
            return NULL;
        
        case CONNECTION_TYPE_TONES:
            /* Stand alone tones never need passthrough. If tones are being
               mixed the speaker chain will already have been created */
            return &speaker_low_power_filter;
        
        default:
            /* Apply filters for low power music */
            if(audioMixerConnectionOfLowPowerMusicEnabled())
                return &speaker_low_power_filter;
    }
    
    return NULL;
}
