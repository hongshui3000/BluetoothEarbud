/****************************************************************************
Copyright (c) 2016 - 2018 Qualcomm Technologies International, Ltd.

FILE NAME
    audio_mixer_chain_config.c

DESCRIPTION
    Implementation of functions to return the appropriate chain configuration.
*/

#include <string.h>
#include <operators.h>
#include <csrtypes.h>
#include <hydra_macros.h>

#include "audio_mixer_core.h"
#include "audio_mixer_core_chain_config.h"
#include "audio_mixer_gain.h"
#include "audio_mixer_channel_roles.h"
#include "audio_mixer_latency.h"
#include "audio_mixer_connection.h"

#define CHANNEL_TO_MASK(channel)        ((uint32)1 << channel)

#define ROUTE_TRANSITION_INSTANT        (0)

#define MIXER_IN_MUSIC_LEFT_PORT      (0)
#define MIXER_IN_MUSIC_RIGHT_PORT     (1)
#define MIXER_IN_VOICE_PORT           (2)

#define MIXER_OUT_LEFT_PORT  (0)
#define MIXER_OUT_RIGHT_PORT (1)

#define SOURCE_SYNC_MUSIC_INPUT_LEFT_PORT          (0)
#define SOURCE_SYNC_MUSIC_INPUT_RIGHT_PORT         (1)
#define SOURCE_SYNC_VOICE_INPUT_PORT               (2)
#define SOURCE_SYNC_DUMMY_INPUT1_PORT              (3)
#define SOURCE_SYNC_DUMMY_INPUT2_PORT              (4)

#define SOURCE_SYNC_MUSIC_OUTPUT_LEFT_PORT          (0)
#define SOURCE_SYNC_MUSIC_OUTPUT_RIGHT_PORT         (1)
#define SOURCE_SYNC_VOICE_OUTPUT_PORT               (2)
#define SOURCE_SYNC_MUSIC_DOWNMIX_OUTPUT_RIGHT_PORT (3)
#define SOURCE_SYNC_MUSIC_DOWNMIX_OUTPUT_LEFT_PORT  (4)

/* Mixer sub-chain */
static const operator_setup_item_t mixer_source_sync_setup[] =
{
    OPERATORS_SETUP_SOURCE_SYNC_LATENCY(CONCURRENT_LATENCY_MILLISECONDS),
    OPERATORS_SETUP_STANDARD_BUFFER_SIZE_FROM_LATENCY(CONCURRENT_SOURCE_SYNC_BUFFER_MILLISECONDS)
};

static const operator_config_t mixer_ops[] =
{
    MAKE_OPERATOR_CONFIG_WITH_SETUP(capability_id_source_sync, sync_role, mixer_source_sync_setup),
    MAKE_OPERATOR_CONFIG(capability_id_mixer, mixer_role)
};

static const operator_endpoint_t mixer_inputs[] =
{
    {sync_role, music_left_channel,   MIXER_IN_MUSIC_LEFT_PORT},
    {sync_role, music_right_channel,  MIXER_IN_MUSIC_RIGHT_PORT},
    {sync_role, voice_channel,        MIXER_IN_VOICE_PORT}
};

static const operator_endpoint_t mixer_outputs[] =
{
    {mixer_role, left_channel,  MIXER_OUT_LEFT_PORT},
    {mixer_role, right_channel, MIXER_OUT_RIGHT_PORT}
};

static const operator_connection_t mixer_connections[] =
{
    {sync_role, 0, mixer_role, 0, 5}
};

static const chain_config_t mixer_config =
    MAKE_CHAIN_CONFIG(chain_id_audio_mixer_core,
                      audio_ucid_mixer_core,
                      mixer_ops, mixer_inputs, mixer_outputs,
                      mixer_connections);

/* Mixer sub-chain (voice only)*/
static const operator_setup_item_t mixer_source_sync_setup_voice[] =
{
    OPERATORS_SETUP_SOURCE_SYNC_LATENCY(VOICE_LATENCY_MILLISECONDS),
    OPERATORS_SETUP_STANDARD_BUFFER_SIZE_FROM_LATENCY(VOICE_LATENCY_MILLISECONDS)
};

static const operator_config_t mixer_ops_voice[] =
{
    MAKE_OPERATOR_CONFIG_WITH_SETUP(capability_id_source_sync, sync_role, mixer_source_sync_setup_voice)
};

static const operator_endpoint_t mixer_inputs_voice[] =
{
    {sync_role, voice_channel, SOURCE_SYNC_VOICE_INPUT_PORT}
};

static const operator_endpoint_t mixer_outputs_voice[] =
{
    {sync_role, left_channel, SOURCE_SYNC_VOICE_OUTPUT_PORT}
};

static const chain_config_t mixer_config_voice =
    MAKE_CHAIN_CONFIG_NO_CONNECTIONS(chain_id_audio_mixer_core,
                                     audio_ucid_mixer_core_voice,
                                     mixer_ops_voice, 
                                     mixer_inputs_voice, 
                                     mixer_outputs_voice);

/* Mixer low power sub-chain */
static const operator_setup_item_t mixer_source_sync_setup_low_power[] =
{
    OPERATORS_SETUP_SOURCE_SYNC_LATENCY(MUSIC_LATENCY_MILLISECONDS),
    OPERATORS_SETUP_STANDARD_BUFFER_SIZE_FROM_LATENCY(MUSIC_LATENCY_MILLISECONDS)
};

static const operator_config_t mixer_ops_low_power[] =
{
    MAKE_OPERATOR_CONFIG_WITH_SETUP(capability_id_source_sync, sync_role, mixer_source_sync_setup_low_power)
};

static const operator_endpoint_t mixer_inputs_low_power[] =
{
    {sync_role, music_left_channel,   MIXER_IN_MUSIC_LEFT_PORT},
    {sync_role, music_right_channel,  MIXER_IN_MUSIC_RIGHT_PORT}
};

static const operator_endpoint_t mixer_outputs_low_power[] =
{
    {sync_role, left_channel,  SOURCE_SYNC_MUSIC_OUTPUT_LEFT_PORT},
    {sync_role, right_channel, SOURCE_SYNC_MUSIC_OUTPUT_RIGHT_PORT}
};

static const chain_config_t mixer_config_low_power =
    MAKE_CHAIN_CONFIG_NO_CONNECTIONS(chain_id_audio_mixer_core,
                                     audio_ucid_mixer_core_low_power,
                                     mixer_ops_low_power, 
                                     mixer_inputs_low_power, 
                                     mixer_outputs_low_power);

/* Configuration of source sync groups and routes
   Used for concurrent audio/voice configurations*/
static const source_sync_source_group_t source_group_config[] =
{
    {
        .meta_data_required = TRUE,
        .ttp_required = TRUE,
        .channel_mask = (CHANNEL_TO_MASK(SOURCE_SYNC_MUSIC_OUTPUT_LEFT_PORT)
                       | CHANNEL_TO_MASK(SOURCE_SYNC_MUSIC_OUTPUT_RIGHT_PORT)
                       | CHANNEL_TO_MASK(SOURCE_SYNC_VOICE_OUTPUT_PORT)
                       | CHANNEL_TO_MASK(SOURCE_SYNC_MUSIC_DOWNMIX_OUTPUT_RIGHT_PORT)
                       | CHANNEL_TO_MASK(SOURCE_SYNC_MUSIC_DOWNMIX_OUTPUT_LEFT_PORT))
    }
};

static const source_sync_sink_group_t sink_group_config[] =
{
    {
        .meta_data_required = TRUE,
        .rate_match = TRUE,
        .channel_mask = (CHANNEL_TO_MASK(SOURCE_SYNC_MUSIC_INPUT_LEFT_PORT) | CHANNEL_TO_MASK(SOURCE_SYNC_MUSIC_INPUT_RIGHT_PORT))
    },
    {
        .meta_data_required = TRUE,
        .rate_match = TRUE,
        .channel_mask = CHANNEL_TO_MASK(SOURCE_SYNC_VOICE_INPUT_PORT)
    },
    {
        .meta_data_required = TRUE,
        .rate_match = TRUE,
        .channel_mask = CHANNEL_TO_MASK(SOURCE_SYNC_DUMMY_INPUT1_PORT) | CHANNEL_TO_MASK(SOURCE_SYNC_DUMMY_INPUT2_PORT)
    }
};

static const source_sync_route_t routes_config[] =
{
    {SOURCE_SYNC_MUSIC_INPUT_LEFT_PORT,  SOURCE_SYNC_MUSIC_OUTPUT_LEFT_PORT,          ROUTE_TRANSITION_INSTANT, 0, GAIN_UNITY},
    {SOURCE_SYNC_MUSIC_INPUT_RIGHT_PORT, SOURCE_SYNC_MUSIC_OUTPUT_RIGHT_PORT,         ROUTE_TRANSITION_INSTANT, 0, GAIN_UNITY},
    {SOURCE_SYNC_VOICE_INPUT_PORT,       SOURCE_SYNC_VOICE_OUTPUT_PORT,               ROUTE_TRANSITION_INSTANT, 0, GAIN_UNITY},
    {SOURCE_SYNC_DUMMY_INPUT1_PORT,      SOURCE_SYNC_MUSIC_DOWNMIX_OUTPUT_RIGHT_PORT, ROUTE_TRANSITION_INSTANT, 0, GAIN_UNITY},
    {SOURCE_SYNC_DUMMY_INPUT2_PORT,      SOURCE_SYNC_MUSIC_DOWNMIX_OUTPUT_LEFT_PORT,  ROUTE_TRANSITION_INSTANT, 0, GAIN_UNITY}
};

static const source_sync_route_t routes_left_channel_mode_config[] =
{
    {SOURCE_SYNC_MUSIC_INPUT_LEFT_PORT,  SOURCE_SYNC_MUSIC_OUTPUT_LEFT_PORT,          ROUTE_TRANSITION_INSTANT, 0, GAIN_UNITY},
    {SOURCE_SYNC_MUSIC_INPUT_LEFT_PORT,  SOURCE_SYNC_MUSIC_OUTPUT_RIGHT_PORT,         ROUTE_TRANSITION_INSTANT, 0, GAIN_UNITY},
    {SOURCE_SYNC_VOICE_INPUT_PORT,       SOURCE_SYNC_VOICE_OUTPUT_PORT,               ROUTE_TRANSITION_INSTANT, 0, GAIN_UNITY},
    {SOURCE_SYNC_DUMMY_INPUT1_PORT,      SOURCE_SYNC_MUSIC_DOWNMIX_OUTPUT_RIGHT_PORT, ROUTE_TRANSITION_INSTANT, 0, GAIN_UNITY},
    {SOURCE_SYNC_DUMMY_INPUT2_PORT,      SOURCE_SYNC_MUSIC_DOWNMIX_OUTPUT_LEFT_PORT,  ROUTE_TRANSITION_INSTANT, 0, GAIN_UNITY}
};

static const source_sync_route_t routes_right_channel_mode_config[] =
{
    {SOURCE_SYNC_MUSIC_INPUT_RIGHT_PORT, SOURCE_SYNC_MUSIC_OUTPUT_LEFT_PORT,          ROUTE_TRANSITION_INSTANT, 0, GAIN_UNITY},
    {SOURCE_SYNC_MUSIC_INPUT_RIGHT_PORT, SOURCE_SYNC_MUSIC_OUTPUT_RIGHT_PORT,         ROUTE_TRANSITION_INSTANT, 0, GAIN_UNITY},
    {SOURCE_SYNC_VOICE_INPUT_PORT,       SOURCE_SYNC_VOICE_OUTPUT_PORT,               ROUTE_TRANSITION_INSTANT, 0, GAIN_UNITY},
    {SOURCE_SYNC_DUMMY_INPUT1_PORT,      SOURCE_SYNC_MUSIC_DOWNMIX_OUTPUT_RIGHT_PORT, ROUTE_TRANSITION_INSTANT, 0, GAIN_UNITY},
    {SOURCE_SYNC_DUMMY_INPUT2_PORT,      SOURCE_SYNC_MUSIC_DOWNMIX_OUTPUT_LEFT_PORT,  ROUTE_TRANSITION_INSTANT, 0, GAIN_UNITY}
};

static const source_sync_route_t routes_downmix_channel_mode_config[] =
{
    {SOURCE_SYNC_MUSIC_INPUT_LEFT_PORT,  SOURCE_SYNC_MUSIC_OUTPUT_LEFT_PORT,          ROUTE_TRANSITION_INSTANT, 0, GAIN_DOWNMIX},
    {SOURCE_SYNC_MUSIC_INPUT_RIGHT_PORT, SOURCE_SYNC_MUSIC_OUTPUT_RIGHT_PORT,         ROUTE_TRANSITION_INSTANT, 0, GAIN_DOWNMIX},
    {SOURCE_SYNC_VOICE_INPUT_PORT,       SOURCE_SYNC_VOICE_OUTPUT_PORT,               ROUTE_TRANSITION_INSTANT, 0, GAIN_UNITY},
    {SOURCE_SYNC_MUSIC_INPUT_LEFT_PORT,  SOURCE_SYNC_MUSIC_DOWNMIX_OUTPUT_LEFT_PORT,  ROUTE_TRANSITION_INSTANT, 0, GAIN_DOWNMIX},
    {SOURCE_SYNC_MUSIC_INPUT_RIGHT_PORT, SOURCE_SYNC_MUSIC_DOWNMIX_OUTPUT_RIGHT_PORT, ROUTE_TRANSITION_INSTANT, 0, GAIN_DOWNMIX}
};

/* Configuration of music source sync groups and routes
   Used for non-concurrent music sources */
static const source_sync_source_group_t source_group_config_music[] =
{
    {
        .meta_data_required = TRUE,
        .ttp_required = TRUE,
        .channel_mask = (CHANNEL_TO_MASK(SOURCE_SYNC_MUSIC_OUTPUT_LEFT_PORT)
                       | CHANNEL_TO_MASK(SOURCE_SYNC_MUSIC_OUTPUT_RIGHT_PORT)
                       | CHANNEL_TO_MASK(SOURCE_SYNC_VOICE_OUTPUT_PORT)
                       | CHANNEL_TO_MASK(SOURCE_SYNC_MUSIC_DOWNMIX_OUTPUT_RIGHT_PORT)
                       | CHANNEL_TO_MASK(SOURCE_SYNC_MUSIC_DOWNMIX_OUTPUT_LEFT_PORT))
    }
};

static const source_sync_sink_group_t sink_group_config_music[] =
{
    {
        .meta_data_required = TRUE,
        .rate_match = FALSE,
        .channel_mask = (CHANNEL_TO_MASK(SOURCE_SYNC_MUSIC_INPUT_LEFT_PORT) | CHANNEL_TO_MASK(SOURCE_SYNC_MUSIC_INPUT_RIGHT_PORT))
    },
    {
        .meta_data_required = TRUE,
        .rate_match = FALSE,
        .channel_mask = CHANNEL_TO_MASK(SOURCE_SYNC_VOICE_INPUT_PORT)
    },
    {
        .meta_data_required = TRUE,
        .rate_match = FALSE,
        .channel_mask = CHANNEL_TO_MASK(SOURCE_SYNC_DUMMY_INPUT1_PORT) | CHANNEL_TO_MASK(SOURCE_SYNC_DUMMY_INPUT2_PORT)
    }
};

/* Configuration of voice source sync groups and routes */
static const source_sync_source_group_t source_group_config_voice[] =
{
    {
        .meta_data_required = TRUE,
        .ttp_required = TRUE,
        .channel_mask = CHANNEL_TO_MASK(SOURCE_SYNC_VOICE_OUTPUT_PORT)
    }
};

static const source_sync_sink_group_t sink_group_config_voice[] =
{
    {
        .meta_data_required = TRUE,
        .rate_match = FALSE,
        .channel_mask = CHANNEL_TO_MASK(SOURCE_SYNC_VOICE_INPUT_PORT)
    }
};

static const source_sync_route_t routes_config_voice[] =
{
    {SOURCE_SYNC_VOICE_INPUT_PORT, SOURCE_SYNC_VOICE_OUTPUT_PORT, ROUTE_TRANSITION_INSTANT, 0, GAIN_UNITY}
};

/* Configuration of low power routes */

static const source_sync_route_t routes_config_low_power_stereo[] =
{
    {SOURCE_SYNC_MUSIC_INPUT_LEFT_PORT,  SOURCE_SYNC_MUSIC_OUTPUT_LEFT_PORT,  ROUTE_TRANSITION_INSTANT, 0, GAIN_UNITY},
    {SOURCE_SYNC_MUSIC_INPUT_RIGHT_PORT, SOURCE_SYNC_MUSIC_OUTPUT_RIGHT_PORT, ROUTE_TRANSITION_INSTANT, 0, GAIN_UNITY}
};

static const source_sync_route_t routes_config_low_power_left[] =
{
    {SOURCE_SYNC_MUSIC_INPUT_LEFT_PORT, SOURCE_SYNC_MUSIC_OUTPUT_LEFT_PORT,  ROUTE_TRANSITION_INSTANT, 0, GAIN_UNITY},
    {SOURCE_SYNC_MUSIC_INPUT_LEFT_PORT, SOURCE_SYNC_MUSIC_OUTPUT_RIGHT_PORT, ROUTE_TRANSITION_INSTANT, 0, GAIN_UNITY}
};

static const source_sync_route_t routes_config_low_power_right[] =
{
    {SOURCE_SYNC_MUSIC_INPUT_RIGHT_PORT, SOURCE_SYNC_MUSIC_OUTPUT_LEFT_PORT,  ROUTE_TRANSITION_INSTANT, 0, GAIN_UNITY},
    {SOURCE_SYNC_MUSIC_INPUT_RIGHT_PORT, SOURCE_SYNC_MUSIC_OUTPUT_RIGHT_PORT, ROUTE_TRANSITION_INSTANT, 0, GAIN_UNITY}
};

/******************************************************************************/
static bool useLowPowerCore(const connection_type_t connection_type)
{
    if(audioMixerConnectionIsLowPowerMusic(connection_type) || audioMixerConnectionIsPrompt(connection_type))
        return TRUE;
    
    return FALSE;
}

/******************************************************************************/
const chain_config_t* mixerCoreGetChainConfig(connection_type_t connection_type)
{
    if(audioMixerConnectionIsVoiceOnly(connection_type))
        return &mixer_config_voice;

    if(useLowPowerCore(connection_type))
        return &mixer_config_low_power;
    
    return &mixer_config;
}

/******************************************************************************/
const source_sync_sink_group_t* mixerCoreGetSourceSyncSinkGroup(connection_type_t connection_type, uint16* num_groups)
{
    if (audioMixerConnectionOfConcurrentVoiceAndMusicEnabled())
    {
        *num_groups = ARRAY_DIM(sink_group_config);
        return sink_group_config;
    }
    else
    {
        if(audioMixerConnectionIsVoice(connection_type))
        {
            *num_groups = ARRAY_DIM(sink_group_config_voice);
            return sink_group_config_voice;
        }
        else
        {
            *num_groups = ARRAY_DIM(sink_group_config_music);
            return sink_group_config_music;
        }
    }
}

/******************************************************************************/
const source_sync_source_group_t* mixerCoreGetSourceSyncSourceGroup(connection_type_t connection_type, uint16* num_groups)
{
    if (audioMixerConnectionOfConcurrentVoiceAndMusicEnabled())
    {
        *num_groups = ARRAY_DIM(source_group_config);
        return source_group_config;
    }
    else
    {
        if(audioMixerConnectionIsVoice(connection_type))
        {
            *num_groups = ARRAY_DIM(source_group_config_voice);
            return source_group_config_voice;
        }
        else
        {
            *num_groups = ARRAY_DIM(source_group_config_music);
            return source_group_config_music;
        }
    }
}

/****************************************************************************
DESCRIPTION
    Updates source/sync routes with the sample_rate
*/
static void mixerCoreUpdateRoutesSampleRates(source_sync_route_t* routes, const uint16 num_routes, const uint32 sample_rate)
{
    int i;
    for(i=0; i < num_routes; i++)
        routes[i].sample_rate = sample_rate;
}

/******************************************************************************/
#define NUM_ROUTES(config) ARRAY_DIM(config)

/******************************************************************************/
static source_sync_route_t* createRoutes(size_t size_config, const source_sync_route_t* config)
{
    source_sync_route_t* routes = PanicUnlessMalloc(size_config);
    memcpy(routes, config, size_config);
    return routes;
}

/******************************************************************************/
#define CREATE_ROUTES(config) createRoutes(sizeof(config), config)

/******************************************************************************/
#define SETUP_ROUTES(routes, num_routes, config) \
{ \
    *num_routes = NUM_ROUTES(config); \
    routes = CREATE_ROUTES(config); \
}

/******************************************************************************/
static source_sync_route_t* mixerCoreGetHighQualityRoutesConfig(AUDIO_MUSIC_CHANNEL_MODE_T channel_mode, uint16* num_routes)
{
    source_sync_route_t* routes = NULL;
    switch (channel_mode)
    {
        case CHANNEL_MODE_STEREO:
            SETUP_ROUTES(routes, num_routes, routes_config);
            break;

        case CHANNEL_MODE_LEFT:
            SETUP_ROUTES(routes, num_routes, routes_left_channel_mode_config);
            break;

        case CHANNEL_MODE_RIGHT:
            SETUP_ROUTES(routes, num_routes, routes_right_channel_mode_config);
            break;

        case CHANNEL_MODE_DOWNMIX:
            SETUP_ROUTES(routes, num_routes, routes_downmix_channel_mode_config);
            break;

        default:
            Panic();
            break;
    }
    return routes;
}

/******************************************************************************/
static source_sync_route_t* mixerCoreGetLowPowerRoutesConfig(AUDIO_MUSIC_CHANNEL_MODE_T channel_mode, uint16* num_routes)
{
    source_sync_route_t* routes = NULL;
    switch (channel_mode)
    {
        case CHANNEL_MODE_STEREO:
        case CHANNEL_MODE_DOWNMIX:
            SETUP_ROUTES(routes, num_routes, routes_config_low_power_stereo);
            break;

        case CHANNEL_MODE_LEFT:
            SETUP_ROUTES(routes, num_routes, routes_config_low_power_left);
            break;

        case CHANNEL_MODE_RIGHT:
            SETUP_ROUTES(routes, num_routes, routes_config_low_power_right);
            break;

        default:
            Panic();
            break;
    }
    return routes;
}

/******************************************************************************/
static source_sync_route_t* mixerCoreGetRoutesConfig(connection_type_t connection_type, AUDIO_MUSIC_CHANNEL_MODE_T channel_mode, uint16* num_routes)
{
    if(useLowPowerCore(connection_type))
        return mixerCoreGetLowPowerRoutesConfig(channel_mode, num_routes);
    
    return mixerCoreGetHighQualityRoutesConfig(channel_mode, num_routes);
}

/******************************************************************************/
source_sync_route_t* mixerCoreGetSourceSyncChannelModeRoutes(AUDIO_MUSIC_CHANNEL_MODE_T channel_mode, uint16* num_routes, uint32 sample_rate)
{
    source_sync_route_t* routes = mixerCoreGetRoutesConfig(CONNECTION_TYPE_MUSIC, channel_mode, num_routes);
    mixerCoreUpdateRoutesSampleRates(routes, *num_routes, sample_rate);
    return routes;
}

/******************************************************************************/
source_sync_route_t* mixerCoreGetSourceSyncRoutes(connection_type_t connection_type, AUDIO_MUSIC_CHANNEL_MODE_T channel_mode, uint16* num_routes, uint32 sample_rate)
{
    source_sync_route_t* routes = NULL;

    if(audioMixerConnectionIsVoiceOnly(connection_type))
    {
        SETUP_ROUTES(routes, num_routes, routes_config_voice);
    }
    else
    {
        routes = mixerCoreGetRoutesConfig(connection_type, channel_mode, num_routes);
    }
    
    mixerCoreUpdateRoutesSampleRates(routes, *num_routes, sample_rate);
    
    return routes;
}

/******************************************************************************/
unsigned mixerCoreGetNumberOfMusicChannels(connection_type_t connection_type)
{
    return audioMixerConnectionIsVoiceOnly(connection_type) ? 1 : 2;
}

/******************************************************************************/
unsigned mixerCoreGetNumberOfDownmixChannels(connection_type_t connection_type)
{
    return audioMixerConnectionIsVoiceOnly(connection_type) ? 0 : 2;
}
