/*!
\copyright  Copyright (c) 2017 Qualcomm Technologies International, Ltd.
            All Rights Reserved.
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    Part of 6.2
\file       av_headset_kymera.c
\brief      Kymera Manager
*/

#include <a2dp.h>
#include <panic.h>
#include <stream.h>
#include <sink.h>
#include <source.h>
#include <chain.h>

#ifdef __QCC3400_APP__
#include <audio_clock.h>
#include <audio_power.h>
#endif

#include "av_headset.h"
#include "av_headset_latency.h"
#include "av_headset_log.h"
#include "av_headset_chain_roles.h"

#include "chains/chain_aptx_mono_no_autosync_decoder.h"
#include "chains/chain_sbc_mono_no_autosync_decoder.h"
#include "chains/chain_output_volume.h"
#include "chains/chain_forwarding_input_sbc_left.h"
#include "chains/chain_forwarding_input_aptx_left.h"
#include "chains/chain_forwarding_input_aac_left.h"
#include "chains/chain_forwarding_input_sbc_right.h"
#include "chains/chain_forwarding_input_aptx_right.h"
#include "chains/chain_forwarding_input_aac_right.h"
#ifndef HFP_USE_2MIC
#include "chains/chain_sco_nb.h"
#include "chains/chain_sco_wb.h"
#else
#include "chains/chain_sco_nb_2mic.h"
#include "chains/chain_sco_wb_2mic.h"
#endif /* HFP_USE_2MIC */
#include "chains/chain_tone_gen.h"

/*! Macro for creating messages */
#define MAKE_KYMERA_MESSAGE(TYPE) \
    TYPE##_T *message = PanicUnlessNew(TYPE##_T);

/*! Helper macro to get size of fixed arrays to populate structures */
#define DIMENSION_AND_ADDR_OF(ARRAY) ARRAY_DIM((ARRAY)), (ARRAY)

/*! Convert a channel ID to a bit mask */
#define CHANNEL_TO_MASK(channel) ((uint32)1 << channel)

/*!@{ \name Port numbers for the Source Sync operator */
#define KYMERA_SOURCE_SYNC_INPUT_PORT (0)
#define KYMERA_SOURCE_SYNC_OUTPUT_PORT (0)
/*!@} */

/*! Kymera requires gain specified in unit of 1/60th db */
#define KYMERA_DB_SCALE (60)

/*! Kymera ringtone generator has a fixed sample rate of 8 kHz */
#define KYMERA_TONE_GEN_RATE (8000)

/*!@{ \name Defines used in setting up kymera messages
    Kymera operator messages are 3 words long, with the ID in the 2nd word */
#define KYMERA_OP_MSG_LEN (3)
#define KYMERA_OP_MSG_WORD_MSG_ID (1)
#define KYMERA_OP_MSG_ID_TONE_END (0x0001) /*!< Kymera ringtone generator TONE_END message */
/*!@}*/

/*@{ \name System kick periods, in microseconds */
#define KICK_PERIOD_FAST (2000)
#define KICK_PERIOD_SLOW (7500)

#define KICK_PERIOD_SLAVE_SBC (KICK_PERIOD_SLOW)
#define KICK_PERIOD_SLAVE_APTX (KICK_PERIOD_SLOW)
#define KICK_PERIOD_MASTER_SBC (KICK_PERIOD_SLOW)
#define KICK_PERIOD_MASTER_AAC (KICK_PERIOD_SLOW)
#define KICK_PERIOD_MASTER_APTX (KICK_PERIOD_SLOW)
#define KICK_PERIOD_TONES (KICK_PERIOD_SLOW)
#define KICK_PERIOD_VOICE (KICK_PERIOD_FAST) /*!< Use low latency for voice */
/*@} */

/*! Maximum sample rate supported by this application */
#define MAX_SAMPLE_RATE (48000)

/*! Maximum codec rate expected by this application */
#define MAX_CODEC_RATE_KBPS (384) /* Stereo aptX */

/*!:{ \name Macros to calculate buffer sizes required to hold a specific (timed) amount of audio */
#define CODEC_BITS_PER_MEMORY_WORD (16)
#define MS_TO_BUFFER_SIZE_MONO_PCM(time_ms, sample_rate) (((time_ms) * (sample_rate)) / MS_PER_SEC)
#define US_TO_BUFFER_SIZE_MONO_PCM(time_us, sample_rate) (((time_us) * (sample_rate)) / US_PER_SEC)
#define MS_TO_BUFFER_SIZE_CODEC(time_ms, codec_rate_kbps) (((time_ms) * (codec_rate_kbps)) / CODEC_BITS_PER_MEMORY_WORD)
/*!@}*/

/*!@{ \name Buffer sizes required to hold enough audio to achieve the TTP latency */
#define PRE_DECODER_BUFFER_SIZE (MS_TO_BUFFER_SIZE_CODEC(PRE_DECODER_BUFFER_MS, MAX_CODEC_RATE_KBPS))
#define PCM_LATENCY_BUFFER_SIZE (MS_TO_BUFFER_SIZE_MONO_PCM(PCM_LATENCY_BUFFER_MS, MAX_SAMPLE_RATE))
/*!@}*/

static const chain_join_roles_t slave_inter_chain_connections[] =
{
    { .source_role = EPR_SOURCE_DECODED_PCM, .sink_role = EPR_SINK_MIXER_MAIN_IN }
};

static const chain_join_roles_t tone_music_inter_chain_connections[] =
{
    { .source_role = RESAMPLER_OUT, .sink_role = EPR_SINK_MIXER_TONE_IN }
};

static const chain_join_roles_t tone_voice_inter_chain_connections[] =
{
    { .source_role = RESAMPLER_OUT, .sink_role = EPR_SCO_VOLUME_AUX }
};

/* Configuration of source sync groups and routes */
static const source_sync_sink_group_t sink_groups[] =
{
    {
        .meta_data_required = TRUE,
        .rate_match = FALSE,
        .channel_mask = CHANNEL_TO_MASK(KYMERA_SOURCE_SYNC_INPUT_PORT)
    }
};

static const source_sync_source_group_t source_groups[] =
{
    {
        .meta_data_required = TRUE,
        .ttp_required = TRUE,
        .channel_mask = CHANNEL_TO_MASK(KYMERA_SOURCE_SYNC_OUTPUT_PORT)
    }
};

static source_sync_route_t routes[] =
{
    {
        .input_terminal = KYMERA_SOURCE_SYNC_INPUT_PORT,
        .output_terminal = KYMERA_SOURCE_SYNC_OUTPUT_PORT,
        .transition_samples = 0,
        .sample_rate = 0, /* Overridden later */
        .gain = 0
    }
};

static const capability_bundle_t capability_bundle[] = {
    {
        "download_switched_passthrough_consumer.edkcs",
        capability_bundle_available_p0
    },
    {
        "download_aptx_demux.edkcs",
        capability_bundle_available_p0
    },
};

static const capability_bundle_config_t bundle_config = {capability_bundle, ARRAY_DIM(capability_bundle)};

static void appKymeraToneStop(void);


/*! @brief Convert volume on 0..127 scale to MIN_VOLUME_DB..MAX_VOLUME_DB and return the KYMERA_DB_SCALE value.
           A input of 0 results in muted output.
 */
static int32 volTo60thDbGain(uint16 volume)
{
    int32 gain = -90;
    if (volume)
    {
        int32 minv = appConfigMinVolumedB();
        int32 maxv = appConfigMaxVolumedB();
        gain = volume * (maxv - minv);
        gain /= 127;
        gain += minv;
    }
    return gain * KYMERA_DB_SCALE;
}

static void appKymeraGetA2dpCodecSettingsCore(a2dp_codec_settings *codec_settings,
                                              uint32 *rate, bool *cp_header_enabled, uint16 *mtu)
{
    if (rate)
        *rate = codec_settings->rate;
    if (cp_header_enabled)
        *cp_header_enabled = !!(codec_settings->codecData.content_protection);
    if (mtu)
        *mtu = codec_settings->codecData.packet_size;

    DEBUG_LOGF("appKymeraGetA2dpCodecSettingsCore, rate %u, cp %u, mtu %u",
                codec_settings->rate, codec_settings->codecData.content_protection,
                codec_settings->codecData.packet_size);
}

static void appKymeraGetA2dpCodecSettingsSBC(a2dp_codec_settings *codec_settings,
                                             sbc_encoder_params_t *sbc_encoder_params)
{
    uint8 sbc_format = codec_settings->codecData.format;
    sbc_encoder_params->channel_mode = codec_settings->channel_mode;
    sbc_encoder_params->bitpool_size = codec_settings->codecData.bitpool;
    sbc_encoder_params->sample_rate = codec_settings->rate;
    sbc_encoder_params->number_of_subbands = (sbc_format & 1) ? 8 : 4;
    sbc_encoder_params->number_of_blocks = (((sbc_format >> 4) & 3) + 1) * 4;
    sbc_encoder_params->allocation_method =  ((sbc_format >> 2) & 1);
}

/*! \brief Configure power mode and clock frequencies of the DSP for the lowest
 *         power consumption possible based on the current state / codec.
 *
 * Note that calling this function with chains already started will likely
 * cause audible glitches if using I2S output. DAC output should be ok.
 */
static void appKymeraConfigureDspPowerMode(bool tone_playing)
{
#if defined(__QCC3400_APP__)
    kymeraTaskData *theKymera = appGetKymera();

    /* Assume we are switching to the low power slow clock unless one of the
     * special cases below applies */
    audio_dsp_clock_configuration cconfig = {
        .active_mode = AUDIO_DSP_SLOW_CLOCK,
        .low_power_mode = AUDIO_DSP_CLOCK_NO_CHANGE,
        .trigger_mode = AUDIO_DSP_CLOCK_NO_CHANGE
    };
    audio_dsp_clock kclocks;
    audio_power_save_mode mode = AUDIO_POWER_SAVE_MODE_3;

    if (theKymera->state == KYMERA_STATE_SCO_ACTIVE)
    {
        /* Never switch clocks at all for voice */
        return;
    }

    if (tone_playing)
    {
        /* Always jump up to normal clock for tones - for most codecs there is
         * not enough MIPs when running on a slow clock to also play a tone */
        cconfig.active_mode = AUDIO_DSP_BASE_CLOCK;
        mode = AUDIO_POWER_SAVE_MODE_1;
    }
    else
    {
        /* Either setting up for the first time or returning from a tone, in
         * either case return to the default clock rate for the codec in use */
        switch (theKymera->a2dp_seid)
        {
            case AV_SEID_APTX_SNK:
                /* Not enough MIPs to run aptX master (TWS standard) on slow clock */
                cconfig.active_mode = AUDIO_DSP_BASE_CLOCK;
                mode = AUDIO_POWER_SAVE_MODE_1;
                break;

            default:
                break;
        }
    }

    PanicFalse(AudioDspClockConfigure(&cconfig));
    PanicFalse(AudioPowerSaveModeSet(mode));

    PanicFalse(AudioDspGetClock(&kclocks));
    mode = AudioPowerSaveModeGet();
    DEBUG_LOGF("appKymeraConfigureDspPowerMode, kymera clocks %d %d %d, mode %d", kclocks.active_mode, kclocks.low_power_mode, kclocks.trigger_mode, mode);
#endif
}

/*! \brief Configure PIO required for controlling external amplifier. */
static void appKymeraExternalAmpSetup(void)
{
    if (appConfigExternalAmpControlRequired())
    {
        int pio_mask = (1 << (appConfigExternalAmpControlPio() % 32));
        int pio_bank = appConfigExternalAmpControlPio() / 32;

        /* map in PIO */
        PioSetMapPins32Bank(pio_bank, pio_mask, pio_mask);
        /* set as output */
        PioSetDir32Bank(pio_bank, pio_mask, pio_mask);
        /* start disabled */
        PioSet32Bank(pio_bank, pio_mask,
                     appConfigExternalAmpControlDisableMask());
    }
}

/*! \brief Enable or disable external amplifier. */
static void appKymeraExternalAmpControl(bool enable)
{
    if (appConfigExternalAmpControlRequired())
    {
        int pio_mask = (1 << (appConfigExternalAmpControlPio() % 32));
        int pio_bank = appConfigExternalAmpControlPio() / 32;

        PioSet32Bank(pio_bank, pio_mask,
                     enable ? appConfigExternalAmpControlEnableMask() :
                              appConfigExternalAmpControlDisableMask());
    }
}

static void appKymeraConfigureRtpDecoder(Operator op, rtp_codec_type_t codec_type, uint32 rate, bool cp_header_enabled)
{
    rtp_working_mode_t mode = (codec_type == rtp_codec_type_aptx && !cp_header_enabled) ?
                                    rtp_ttp_only : rtp_decode;
    OperatorsRtpSetCodecType(op, codec_type);
    OperatorsRtpSetWorkingMode(op, mode);
    OperatorsStandardSetTimeToPlayLatency(op, TWS_STANDARD_LATENCY_US);
    OperatorsStandardSetBufferSize(op, PRE_DECODER_BUFFER_SIZE);
    OperatorsRtpSetContentProtection(op, cp_header_enabled);
    OperatorsStandardSetSampleRate(op, rate);
}

/* Set the SPC mode, consumer if is_consumer else passthrough */
static void appKymeraConfigureSpcMode(Operator op, bool is_consumer)
{
    uint16 msg[2];
    msg[0] = 1; /* MSG ID to set SPC mode transition */
    msg[1] = is_consumer;
    PanicFalse(OperatorMessage(op, msg, 2, NULL, 0));
}

/* Set the SPC data format, PCM if is_pcm else encoded-data */
static void appKymeraConfigureSpcDataFormat(Operator op, bool is_pcm)
{
    uint16 msg[2];
    msg[0] = 2; /* MSG ID to set SPC data type */
    msg[1] = is_pcm;
    PanicFalse(OperatorMessage(op, msg, 2, NULL, 0));
}

static void appKymeraConfigureSourceSync(Operator op, uint32 rate, unsigned kick_period_us)
{
    /* Override sample rate in routes config */
    routes[0].sample_rate = rate;

    /* Send operator configuration messages */
    OperatorsStandardSetSampleRate(op, rate);
    OperatorsSourceSyncSetSinkGroups(op, DIMENSION_AND_ADDR_OF(sink_groups));
    OperatorsSourceSyncSetSourceGroups(op, DIMENSION_AND_ADDR_OF(source_groups));
    OperatorsSourceSyncSetRoutes(op, DIMENSION_AND_ADDR_OF(routes));

    /* Output buffer needs to be able to hold at least SS_MAX_PERIOD worth
     * of audio (default = 2 * Kp), but be less than SS_MAX_LATENCY (5 * Kp).
     * The recommendation is 2 Kp more than SS_MAX_PERIOD, so 4 * Kp. */
    OperatorsStandardSetBufferSize(op, US_TO_BUFFER_SIZE_MONO_PCM(4 * kick_period_us, rate));
}

static void appKymeraSetMainVolume(kymera_chain_handle_t chain, uint16 volume)
{
    Operator volop = ChainGetOperatorByRole(chain, OPR_VOLUME_CONTROL);
    if (volop != 0)
    {
        OperatorsVolumeSetMainGain(volop, volTo60thDbGain(volume));
        OperatorsVolumeMute(volop, (volume == 0));
    }
}

static void appKymeraSetVolume(kymera_chain_handle_t chain, uint16 volume)
{
    Operator volop = ChainGetOperatorByRole(chain, OPR_VOLUME_CONTROL);
    if (volop != 0)
    {
        OperatorsVolumeSetMainAndAuxGain(volop, volTo60thDbGain(volume));
        OperatorsVolumeMute(volop, (volume == 0));
    }
}

static void appKymeraMuteVolume(kymera_chain_handle_t chain)
{
    appKymeraSetVolume(chain, 0);
}

/* These SBC encoder parameters are used when forwarding is disabled to save power */
static void appKymeraSetLowPowerSBCParams(kymera_chain_handle_t chain, uint32 rate)
{
    Operator op = ChainGetOperatorByRole(chain, OPR_SBC_ENCODER);
    if (op != INVALID_OPERATOR)
    {
        sbc_encoder_params_t sbc_encoder_params_low_power_default = {
            .number_of_subbands = 4,
            .number_of_blocks = 16,
            .bitpool_size = 1,
            .sample_rate = rate,
            .channel_mode = sbc_encoder_channel_mode_mono,
            .allocation_method = sbc_encoder_allocation_method_loudness
        };
        OperatorsSbcEncoderSetEncodingParams(op, &sbc_encoder_params_low_power_default);
    }
}


/*  Configure the source sync and volume operators in the supplied chain.

    Other than sample rate and kick period, the gain in the volume operator
    is set to level (0db gain) and then muted.
    
    Requires the chain to use standard roles for the operators, namely
    #OPR_SOURCE_SYNC and #OPR_VOLUME_CONTROL.
 */
static void appKymeraConfigureOutputChainOperators(kymera_chain_handle_t chain,
                                                   uint32 sample_rate, unsigned kick_period)
{
    Operator sync_op;
    Operator volume_op;

    /* Configure operators */
    sync_op = (Operator)PanicFalse(ChainGetOperatorByRole(chain, OPR_SOURCE_SYNC));
    appKymeraConfigureSourceSync(sync_op, sample_rate, kick_period);

    volume_op = ChainGetOperatorByRole(chain, OPR_VOLUME_CONTROL);
    OperatorsStandardSetSampleRate(volume_op, sample_rate);
    appKymeraMuteVolume(chain);
}

static void appKymeraCreateOutputChain(uint32 rate, unsigned kick_period)
{
    kymeraTaskData *theKymera = appGetKymera();
    Sink dac;
    kymera_chain_handle_t chain;

    /* Create chain */
    chain = ChainCreate(&chain_output_volume_config);
    theKymera->chain_output_vol_handle = chain;

    appKymeraConfigureOutputChainOperators(chain, rate, kick_period);

    /* Configure the DAC channel */
    dac = StreamAudioSink(AUDIO_HARDWARE_CODEC, AUDIO_INSTANCE_0, appConfigLeftAudioChannel());
    PanicFalse(SinkConfigure(dac, STREAM_CODEC_OUTPUT_RATE, rate));

    ChainConnect(chain);
    ChainConnectOutput(chain, dac, EPR_SOURCE_MIXER_OUT);
}

static void appKymeraCreateToneChain(const ringtone_note *tone, uint32 out_rate)
{
    kymeraTaskData *theKymera = appGetKymera();
    Operator op;

    /* Create chain with resampler */
    theKymera->chain_tone_handle = ChainCreate(&chain_tone_gen_config);

    /* Configure resampler */
    op = ChainGetOperatorByRole(theKymera->chain_tone_handle, RESAMPLER);
    OperatorsResamplerSetConversionRate(op, KYMERA_TONE_GEN_RATE, out_rate);

    /* Configure ringtone generator */
    op = ChainGetOperatorByRole(theKymera->chain_tone_handle, TONE_GEN);
    OperatorsStandardSetSampleRate(op, KYMERA_TONE_GEN_RATE);
    OperatorsConfigureToneGenerator(op, tone, &theKymera->task);

    ChainConnect(theKymera->chain_tone_handle);
}

static void appKymeraA2dpStartMaster(kymeraTaskData *theKymera, Source media_source, uint32 rate, bool cp_header_enabled)
{
    Operator op;
    kymera_chain_handle_t chain_handle = NULL;
    bool is_left = appConfigIsLeft();
    unsigned kick_period;

    /* Create input chain */
    switch (theKymera->a2dp_seid)
    {
        case AV_SEID_SBC_SNK:
        {
            DEBUG_LOG("appKymeraA2dpStartMaster, standard TWS SBC");
            chain_handle = ChainCreate(is_left ?
                                &chain_forwarding_input_sbc_left_config :
                                &chain_forwarding_input_sbc_right_config);
            op = ChainGetOperatorByRole(chain_handle, OPR_RTP_DECODER);
            appKymeraConfigureRtpDecoder(op, rtp_codec_type_sbc, rate, cp_header_enabled);
            op = ChainGetOperatorByRole(chain_handle, OPR_SWITCHED_PASSTHROUGH_CONSUMER);
            appKymeraConfigureSpcDataFormat(op, FALSE);
            kick_period = KICK_PERIOD_MASTER_SBC;
        }
        break;
        case AV_SEID_AAC_SNK:
        {
            Operator op_rtp_decoder;
            DEBUG_LOG("appKymeraA2dpStartMaster, standard TWS AAC");
            chain_handle = ChainCreate(is_left ?
                                &chain_forwarding_input_aac_left_config :
                                &chain_forwarding_input_aac_right_config);
            op_rtp_decoder = ChainGetOperatorByRole(chain_handle, OPR_RTP_DECODER);
            appKymeraConfigureRtpDecoder(op_rtp_decoder, rtp_codec_type_aac, rate, cp_header_enabled);
            op = ChainGetOperatorByRole(chain_handle, OPR_SWITCHED_PASSTHROUGH_CONSUMER);
            appKymeraConfigureSpcDataFormat(op, FALSE);
            op = ChainGetOperatorByRole(chain_handle, OPR_AAC_DECODER);
            OperatorsRtpSetAacCodec(op_rtp_decoder, op);
            kick_period = KICK_PERIOD_MASTER_AAC;
        }
        break;
        case AV_SEID_APTX_SNK:
            DEBUG_LOG("appKymeraA2dpStartMaster, standard TWS aptX");
            chain_handle = ChainCreate(is_left ?
                                &chain_forwarding_input_aptx_left_config :
                                &chain_forwarding_input_aptx_right_config);
            op = ChainGetOperatorByRole(chain_handle, OPR_RTP_DECODER);
            appKymeraConfigureRtpDecoder(op, rtp_codec_type_aptx, rate, cp_header_enabled);
            op = ChainGetOperatorByRole(chain_handle, OPR_SWITCHED_PASSTHROUGH_CONSUMER);
            appKymeraConfigureSpcDataFormat(op, FALSE);
            op = ChainGetOperatorByRole(chain_handle, OPR_APTX_DEMUX);
            OperatorsStandardSetSampleRate(op, rate);
            kick_period = KICK_PERIOD_MASTER_APTX;
        break;
        default:
            /* Control should never reach here */
            Panic();
        return;
    }

    appKymeraSetLowPowerSBCParams(chain_handle, rate);

    OperatorsFrameworkSetKickPeriod(kick_period);
    theKymera->chain_input_handle = chain_handle;
    ChainConnect(theKymera->chain_input_handle);

    /* Configure DSP for low power */
    appKymeraConfigureDspPowerMode(FALSE);

    /* Create output chain */
    appKymeraCreateOutputChain(rate, kick_period);
    op = ChainGetOperatorByRole(theKymera->chain_output_vol_handle, OPR_LATENCY_BUFFER);
    OperatorsStandardSetBufferSize(op, PCM_LATENCY_BUFFER_SIZE);

    /* Connect input and output chains together */
    ChainConnectInput(theKymera->chain_output_vol_handle,
                      ChainGetOutput(theKymera->chain_input_handle, EPR_SOURCE_DECODED_PCM),
                      EPR_SINK_MIXER_MAIN_IN);

    /* Connect media source */
    StreamDisconnect(media_source, 0);
    if (ChainConnectInput(theKymera->chain_input_handle, media_source, EPR_SINK_MEDIA))
    {
        /* Enable external amplifier if required */
        appKymeraExternalAmpControl(TRUE);

        /* Start chains */
        ChainStart(theKymera->chain_output_vol_handle);
        ChainStart(theKymera->chain_input_handle);
    }
}


static void appKymeraA2dpStartSlave(kymeraTaskData *theKymera, Source media_source, bool cp_header_enabled, uint32 rate, bool is_aptx)
{
    unsigned kick_period;
    vm_transform_packetise_codec p0_codec;
    Operator op;

    /* Create input chain */
    if (is_aptx)
    {
        DEBUG_LOG("appKymeraA2dpStartSlave, TWS+ aptX");
        theKymera->chain_input_handle = ChainCreate(&chain_aptx_mono_no_autosync_decoder_config);
        kick_period = KICK_PERIOD_SLAVE_APTX;
        p0_codec = VM_TRANSFORM_PACKETISE_CODEC_APTX;
    }
    else
    {
        DEBUG_LOG("appKymeraA2dpStartSlave, TWS+ SBC");
        theKymera->chain_input_handle = ChainCreate(&chain_sbc_mono_no_autosync_decoder_config);
        kick_period = KICK_PERIOD_SLAVE_SBC;
        p0_codec = VM_TRANSFORM_PACKETISE_CODEC_SBC;
    }
    op = ChainGetOperatorByRole(theKymera->chain_input_handle, OPR_LATENCY_BUFFER);
    OperatorsStandardSetBufferSize(op, 0x1000);
    appKymeraConfigureSpcDataFormat(op, FALSE);

    OperatorsFrameworkSetKickPeriod(kick_period);
    ChainConnect(theKymera->chain_input_handle);

    /* Configure DSP for low power */
    appKymeraConfigureDspPowerMode(FALSE);

    /* Create output chain */
    appKymeraCreateOutputChain(rate, kick_period);

    /* Connect chains together */
    ChainJoin(theKymera->chain_input_handle, theKymera->chain_output_vol_handle,
              DIMENSION_AND_ADDR_OF(slave_inter_chain_connections));

    /* Disconnect A2DP from dispose sink */
    StreamDisconnect(media_source, 0);

    theKymera->packetiser_slave = TransformPacketise(media_source, ChainGetInput(theKymera->chain_input_handle, EPR_SINK_MEDIA));
    TransformConfigure(theKymera->packetiser_slave, VM_TRANSFORM_PACKETISE_CODEC, p0_codec);
    TransformConfigure(theKymera->packetiser_slave, VM_TRANSFORM_PACKETISE_MODE, VM_TRANSFORM_PACKETISE_MODE_TWSPLUS);
    TransformConfigure(theKymera->packetiser_slave, VM_TRANSFORM_PACKETISE_SAMPLE_RATE, (uint16)(rate & 0xffff));
    TransformConfigure(theKymera->packetiser_slave, VM_TRANSFORM_PACKETISE_CPENABLE, cp_header_enabled);
    TransformStart(theKymera->packetiser_slave);

    /* Enable external amplifier if required */
    appKymeraExternalAmpControl(TRUE);

    /* Switch to passthrough now the operator is fully connected */
    appKymeraConfigureSpcMode(op, FALSE);

    /* Start chains */
    ChainStart(theKymera->chain_input_handle);
    ChainStart(theKymera->chain_output_vol_handle);
}

static void appKymeraHandleInternalA2dpStart(uint8 device_id, uint8 stream_id, uint8 seid, Sink media_sink)
{
    kymeraTaskData *theKymera = appGetKymera();
    uint32 rate;
    bool cp_header_enabled;
    Source media_source;

    DEBUG_LOGF("appKymeraHandleInternalA2dpStart, sink %p, seid %d, state %u", media_sink, seid, theKymera->state);

    PanicNotZero(theKymera->wait_for_tone);

    if (theKymera->state == KYMERA_STATE_TONE_PLAYING)
    {
        /* If there is a tone still playing at this point,
         * it must be an interruptable tone, so cut it off */
        appKymeraToneStop();
    }

    /* Can only start streaming if we're currently idle */
    PanicFalse(theKymera->state == KYMERA_STATE_IDLE);

    /* Ensure there are no audio chains already */
    PanicNotNull(theKymera->chain_input_handle);
    PanicNotNull(theKymera->chain_output_vol_handle);

    /* We need the source from the sink to connect to RTP decoder */
    media_source = StreamSourceFromSink(media_sink);

    /* Get the settings we need from the configured CODEC settings */
    a2dp_codec_settings *codec_settings = A2dpCodecGetSettings(device_id, stream_id);
    if (SourceIsValid(media_source) && (codec_settings != NULL))
    {
        appKymeraGetA2dpCodecSettingsCore(codec_settings, &rate, &cp_header_enabled, NULL);
        free(codec_settings);

        DEBUG_LOGF("appKymeraHandleInternalA2dpStart, media source %p, rate %u, cp %u", media_source, rate, cp_header_enabled);

        /* Set state to 'a2dp streaming' */
        theKymera->state = KYMERA_STATE_A2DP_STREAMING;
        theKymera->output_rate = rate;
        theKymera->a2dp_seid = seid;

        switch (seid)
        {
            /* Standard forwarding TWS sinks */
            case AV_SEID_SBC_SNK:
            case AV_SEID_AAC_SNK:
            case AV_SEID_APTX_SNK:
                appKymeraA2dpStartMaster(theKymera, media_source, rate, cp_header_enabled);
                break;

            /* TWS+ slave */
            case AV_SEID_APTX_MONO_TWS_SNK:
                appKymeraA2dpStartSlave(theKymera, media_source, cp_header_enabled, rate, TRUE);
                break;

            /* Standard (timestamped SBC) slave */
            case AV_SEID_SBC_MONO_TWS_SNK:
                appKymeraA2dpStartSlave(theKymera, media_source, cp_header_enabled, rate, FALSE);
                break;

            default:
                /* Unsupported SEID, control should never reach here */
                Panic();
                break;
        }
    }
    else
    {
        DEBUG_LOGF("appKymeraHandleInternalA2dpStart, invalid source (%p) not starting", media_source);
    }
}

static void appKymeraHandleInternalA2dpStop(Sink media_sink)
{
    kymeraTaskData *theKymera = appGetKymera();

    switch (theKymera->state)
    {
        case KYMERA_STATE_IDLE:
            /* This can happen if A2DP is started but by the time the internal handler
               attempts to start kymera the A2DP media_sink is invalid and kymera is not started.*/
            DEBUG_LOGF("appKymeraHandleInternalA2dpStop, already idle, sink %p", media_sink);
            break;

        case KYMERA_STATE_A2DP_STREAMING_WITH_FORWARDING:
            /* Stop forwarding first */
            PanicFalse(appKymeraA2dpStopForwarding());
            /* Fallthrough */

        case KYMERA_STATE_A2DP_STREAMING:
            DEBUG_LOGF("appKymeraHandleInternalA2dpStop, sink %p, state %u", media_sink, theKymera->state);
            PanicNotZero(theKymera->wait_for_tone);
            PanicNull(theKymera->chain_input_handle);
            PanicNull(theKymera->chain_output_vol_handle);

            /* If there is a tone still playing at this point, it must be interruptable,
            so cut it off. */
            appKymeraMuteVolume(theKymera->chain_output_vol_handle);

            /* Stop chains before disconnecting */
            ChainStop(theKymera->chain_input_handle);
            ChainStop(theKymera->chain_output_vol_handle);
            if (theKymera->chain_tone_handle)
            {
                ChainStop(theKymera->chain_tone_handle);
            }

            /* Disable external amplifier if required */
            appKymeraExternalAmpControl(FALSE);

            /* Disconnect A2DP source from the RTP operator */
            StreamDisconnect(StreamSourceFromSink(media_sink), 0);

            /* Dispose of any incoming data on media channel */
            StreamConnectDispose(StreamSourceFromSink(media_sink));

            /* Destroy chains now that input has been disconnected */
            ChainDestroy(theKymera->chain_input_handle);
            theKymera->chain_input_handle = NULL;
            ChainDestroy(theKymera->chain_output_vol_handle);
            theKymera->chain_output_vol_handle = NULL;
            if (theKymera->chain_tone_handle)
            {
                ChainDestroy(theKymera->chain_tone_handle);
                theKymera->chain_tone_handle = NULL;
            }

            /* Destroy slave packetiser */
            if (theKymera->packetiser_slave)
            {
                TransformStop(theKymera->packetiser_slave);
                theKymera->packetiser_slave = NULL;
            }

            /* There should be no master packetiser */
            PanicFalse(theKymera->packetiser_master == NULL);

            /* Move back to idle state */
            theKymera->state = KYMERA_STATE_IDLE;
            theKymera->output_rate = 0;
            theKymera->a2dp_seid = AV_SEID_INVALID;
            break;

        default:
            DEBUG_LOGF("appKymeraHandleInternalA2dpStop sink %p, invalid state %u",
                        media_sink, theKymera->state);
            Panic();
            break;
    }
}

static void appKymeraHandleInternalA2dpStartForwarding(uint8 device_id, uint8 stream_id, uint8 seid, Sink forwarding_sink)
{
    kymeraTaskData *theKymera = appGetKymera();
    sbc_encoder_params_t sbc_encoder_params;
    uint16 mtu;
    bool cp_header_enabled;
    Operator spc_op;
    vm_transform_packetise_codec p0_codec = VM_TRANSFORM_PACKETISE_CODEC_APTX;

    Source audio_source = ChainGetOutput(theKymera->chain_input_handle, EPR_SOURCE_FORWARDING_MEDIA);
    DEBUG_LOGF("appKymeraHandleInternalA2dpStartForwarding, sink %p, source %p, seid %d, state %u",
                forwarding_sink, audio_source, seid, theKymera->state);

    PanicNotZero(theKymera->wait_for_tone);

    /* Can only start forwarding if already streaming */
    PanicFalse(theKymera->state == KYMERA_STATE_A2DP_STREAMING);

    a2dp_codec_settings *codec_settings = A2dpCodecGetSettings(device_id, stream_id);
    appKymeraGetA2dpCodecSettingsCore(codec_settings, NULL, &cp_header_enabled, &mtu);
    appKymeraGetA2dpCodecSettingsSBC(codec_settings, &sbc_encoder_params);
    free(codec_settings);

    switch (seid)
    {
        case AV_SEID_APTX_MONO_TWS_SRC:
            p0_codec = VM_TRANSFORM_PACKETISE_CODEC_APTX;
            break;

        case AV_SEID_SBC_MONO_TWS_SRC:
        {
            p0_codec = VM_TRANSFORM_PACKETISE_CODEC_SBC;
            Operator sbc_encoder= ChainGetOperatorByRole(theKymera->chain_input_handle, OPR_SBC_ENCODER);
            OperatorsSbcEncoderSetEncodingParams(sbc_encoder, &sbc_encoder_params);
            break;
        }

        default:
            /* Unsupported SEID, control should never reach here */
            Panic();
            break;
    }

    theKymera->packetiser_master = TransformPacketise(audio_source, forwarding_sink);
    TransformConfigure(theKymera->packetiser_master, VM_TRANSFORM_PACKETISE_CODEC, p0_codec);
    TransformConfigure(theKymera->packetiser_master, VM_TRANSFORM_PACKETISE_MODE, VM_TRANSFORM_PACKETISE_MODE_TWSPLUS);
    TransformConfigure(theKymera->packetiser_master, VM_TRANSFORM_PACKETISE_MTU, mtu);
    TransformConfigure(theKymera->packetiser_master, VM_TRANSFORM_PACKETISE_TIME_BEFORE_TTP, appConfigTwsTimeBeforeTx() / 1000);
    TransformConfigure(theKymera->packetiser_master, VM_TRANSFORM_PACKETISE_LATEST_TIME_BEFORE_TTP, appConfigTwsDeadline() / 1000);
    TransformConfigure(theKymera->packetiser_master, VM_TRANSFORM_PACKETISE_CPENABLE, cp_header_enabled);
    TransformStart(theKymera->packetiser_master);

    spc_op = ChainGetOperatorByRole(theKymera->chain_input_handle, OPR_SWITCHED_PASSTHROUGH_CONSUMER);
    appKymeraConfigureSpcMode(spc_op, FALSE);
    theKymera->state = KYMERA_STATE_A2DP_STREAMING_WITH_FORWARDING;
}

static bool appKymeraHandleInternalA2dpStopForwarding(void)
{
    Operator spc_op;
    kymeraTaskData *theKymera = appGetKymera();

    DEBUG_LOGF("appKymeraHandleInternalA2dpStopForwarding, state %u", theKymera->state);

    if (theKymera->state != KYMERA_STATE_A2DP_STREAMING_WITH_FORWARDING)
        return FALSE;

    spc_op = ChainGetOperatorByRole(theKymera->chain_input_handle, OPR_SWITCHED_PASSTHROUGH_CONSUMER);
    appKymeraConfigureSpcMode(spc_op, TRUE);

    if (theKymera->packetiser_master)
    {
        TransformStop(theKymera->packetiser_master);
        theKymera->packetiser_master = NULL;
    }

    appKymeraSetLowPowerSBCParams(theKymera->chain_input_handle, theKymera->output_rate);

    theKymera->state = KYMERA_STATE_A2DP_STREAMING;
    return TRUE;
}

static void appKymeraHandleInternalA2dpSetVolume(uint16 volume)
{
    kymeraTaskData *theKymera = appGetKymera();

    DEBUG_LOGF("appKymeraHandleInternalA2dpSetVolume, vol %u", volume);

    switch (theKymera->state)
    {
        case KYMERA_STATE_A2DP_STREAMING:
        case KYMERA_STATE_A2DP_STREAMING_WITH_FORWARDING:
            appKymeraSetMainVolume(theKymera->chain_output_vol_handle, volume);
            break;

        default:
            break;
    }
}


/* SCO chain selection is wrapped in this function.
   
   This code relies on the NB and WB chains being "the same" other than
   operators.
 */
static void appKymeraGetScoChainConfig(const chain_config_t **chosen,hfp_wbs_codec_mask codec)
{
#ifndef HFP_USE_2MIC
        /* Select narrowband or wideband chain depending on CODEC */
        *chosen = (codec == hfp_wbs_codec_mask_msbc) ? &chain_sco_wb_config :
                                                       &chain_sco_nb_config;
#else
        /* Select narrowband or wideband chain depending on CODEC */
        *chosen = (codec == hfp_wbs_codec_mask_msbc) ? &chain_sco_wb_2mic_config :
                                                       &chain_sco_nb_2mic_config;
#endif
}


static void appKymeraHandleInternalScoStart(Sink audio_sink, hfp_wbs_codec_mask codec, uint8 wesco)
{
    UNUSED(wesco);
    
    kymeraTaskData *theKymera = appGetKymera();
    const uint32_t rate = (codec == hfp_wbs_codec_mask_msbc) ? 16000 : 8000;
    const chain_config_t *chain_config;
    kymera_chain_handle_t chain;

    DEBUG_LOGF("appKymeraHandleInternalScoStart, sink 0x%x, rate %u, wesco %u, state %u", audio_sink, rate, wesco, theKymera->state);

    PanicNotZero(theKymera->wait_for_tone);

    if (theKymera->state == KYMERA_STATE_TONE_PLAYING)
    {
        /* If there is a tone still playing at this point,
         * it must be an interruptible tone, so cut it off */
        appKymeraToneStop();
    }

    /* Can't start voice chain if we're not idle */
    PanicFalse(theKymera->state == KYMERA_STATE_IDLE);

    /* SCO chain must be destroyed if we get here */
    PanicNotNull(theKymera->chain_sco_handle);

    appKymeraGetScoChainConfig(&chain_config,codec);

    /* Create chain */
    chain = ChainCreate(chain_config);
    theKymera->chain_sco_handle = chain;

    /* Get sources and sinks for chain */
    Source sco_src = ChainGetOutput(chain, EPR_SCO_TO_AIR);
    Sink sco_sink = ChainGetInput(chain, EPR_SCO_FROM_AIR);
    Source audio_source = StreamSourceFromSink(audio_sink);
    Source mic_src1 = StreamAudioSource(AUDIO_HARDWARE_CODEC, appConfigMicAudioInstance(), AUDIO_CHANNEL_A);
    Sink mic_sink1 = ChainGetInput(chain, EPR_SCO_MIC1);
#ifdef HFP_USE_2MIC
    Source mic_src1b = StreamAudioSource(AUDIO_HARDWARE_CODEC, appConfigMicAudioInstance(), AUDIO_CHANNEL_B);
    Sink mic_sink1b = ChainGetInput(chain, EPR_SCO_MIC2);
#endif
    Source speaker_src = ChainGetOutput(chain, EPR_SCO_SPEAKER);
    Sink speaker_snk = StreamAudioSink(AUDIO_HARDWARE_CODEC, AUDIO_INSTANCE_0, appConfigLeftAudioChannel());

    /* Set AEC REF sample rate */
    Operator aec_op = ChainGetOperatorByRole(chain, OPR_SCO_AEC);
    OperatorsAecSetSampleRate(aec_op, rate, rate);

    /* Enable Time To Play if supported */
    if (appConfigScoChainTTP(wesco))
    {
        Operator sco_rcv_op = ChainGetOperatorByRole(chain, OPR_SCO_RECEIVE);

        OperatorsStandardSetTimeToPlayLatency(sco_rcv_op, appConfigScoChainTTP(wesco));
        OperatorsAecEnableTtpGate(aec_op, TRUE, 50, TRUE);
    }

    appKymeraConfigureOutputChainOperators(chain, rate, KICK_PERIOD_VOICE);

    /* Set DAC and ADC sample rate */
    SourceConfigure(mic_src1, STREAM_CODEC_INPUT_RATE, rate);

#ifdef HFP_USE_2MIC
    SourceConfigure(mic_src1b, STREAM_CODEC_INPUT_RATE, rate);
    SourceSynchronise(mic_src1,mic_src1b);
#endif
    SinkConfigure(speaker_snk, STREAM_CODEC_OUTPUT_RATE, rate);

    /* Conect it all together */
    StreamConnect(sco_src, audio_sink);
    StreamConnect(audio_source, sco_sink);
    StreamConnect(mic_src1, mic_sink1);
#ifdef HFP_USE_2MIC
    StreamConnect(mic_src1b, mic_sink1b);
#endif
    StreamConnect(speaker_src, speaker_snk);
    ChainConnect(chain);

    /* Enable external amplifier if required */
    appKymeraExternalAmpControl(TRUE);

    /* Turn on MIC bias */
    MicbiasConfigure(MIC_BIAS_0, MIC_BIAS_ENABLE, MIC_BIAS_FORCE_ON);

    /* Start chain */
    ChainStart(chain);

    /* Move to SCO active state */
    theKymera->state = KYMERA_STATE_SCO_ACTIVE;
    theKymera->output_rate = rate;
}

static void appKymeraHandleInternalScoStop(void)
{
    kymeraTaskData *theKymera = appGetKymera();

    DEBUG_LOGF("appKymeraHandleInternalScoStop, state %u", theKymera->state);

    PanicNotZero(theKymera->wait_for_tone);
    PanicFalse(theKymera->state == KYMERA_STATE_SCO_ACTIVE);
    PanicNull(theKymera->chain_sco_handle);

    Source send_src = ChainGetOutput(theKymera->chain_sco_handle, EPR_SCO_TO_AIR);
    Sink rcv_sink = ChainGetInput(theKymera->chain_sco_handle, EPR_SCO_FROM_AIR);
    Sink send_sink1 = ChainGetInput(theKymera->chain_sco_handle, EPR_SCO_MIC1);
#ifdef HFP_USE_2MIC
    Sink send_sink1b = ChainGetInput(theKymera->chain_sco_handle, EPR_SCO_MIC2);
#endif
    Source rcv_src = ChainGetOutput(theKymera->chain_sco_handle, EPR_SCO_SPEAKER);

    /* Mute first. If there is a tone still playing at this
     * point, it must be an interruptible tone, so cut it off. */
    appKymeraMuteVolume(theKymera->chain_sco_handle);

    /* Stop chains */
    ChainStop(theKymera->chain_sco_handle);
    if (theKymera->chain_tone_handle)
    {
        ChainStop(theKymera->chain_tone_handle);
    }

    StreamDisconnect(send_src, send_sink1);
#ifdef HFP_USE_2MIC
    StreamDisconnect((Source)NULL, send_sink1b);
#endif
    StreamDisconnect(rcv_src, rcv_sink);

    /* Destroy chains */
    ChainDestroy(theKymera->chain_sco_handle);
    theKymera->chain_sco_handle = NULL;
    if (theKymera->chain_tone_handle)
    {
        ChainDestroy(theKymera->chain_tone_handle);
        theKymera->chain_tone_handle = NULL;
    }

    /* Disable external amplifier if required */
    appKymeraExternalAmpControl(FALSE);

    /* Turn off MIC bias */
    /*! \todo Check if MIC_BIAS different for 2 mic */
    MicbiasConfigure(MIC_BIAS_0, MIC_BIAS_ENABLE, MIC_BIAS_OFF);

    /* Update state variables */
    theKymera->state = KYMERA_STATE_IDLE;
    theKymera->output_rate = 0;
}

static void appKymeraHandleInternalScoSetVolume(uint8 volume)
{
    kymeraTaskData *theKymera = appGetKymera();

    DEBUG_LOGF("appKymeraHandleInternalScoSetVolume, vol %u", volume);

    switch (theKymera->state)
    {
        case KYMERA_STATE_SCO_ACTIVE:
        {
            uint16 volume_scaled = ((uint16)volume * 127) / 15;
            appKymeraSetMainVolume(theKymera->chain_sco_handle, volume_scaled);
        }
        break;

        default:
            break;
    }
}

static void appKymeraHandleInternalScoMicMute(bool mute)
{
    kymeraTaskData *theKymera = appGetKymera();

    DEBUG_LOGF("appKymeraHandleInternalScoMicMute, mute %u", mute);

    switch (theKymera->state)
    {
        case KYMERA_STATE_SCO_ACTIVE:
        {
            Operator aec_op = ChainGetOperatorByRole(theKymera->chain_sco_handle, OPR_SCO_AEC);
            if (aec_op)
                OperatorsAecMuteMicOutput(aec_op, mute);
        }
        break;

        default:
            break;
    }
}


static void appKymeraHandleInternalTonePlay(const ringtone_note *tone, bool interruptible)
{
    kymeraTaskData *theKymera = appGetKymera();
    Operator op;
    kymera_chain_handle_t output_chain;
    const chain_join_roles_t *connections;
    unsigned num_connections;

    DEBUG_LOGF("appKymeraHandleInternalTonePlay, tone %p, int %u", tone, interruptible);

    PanicNotZero(theKymera->wait_for_tone);

    /* Check if there is already a tone playing (chain exists) */
    if (theKymera->chain_tone_handle != NULL)
    {
        /* If there is a tone still playing at this point,
         * it must be an interruptible tone, so cut it off */
        appKymeraToneStop();
    }

    op = ChainGetOperatorByRole(theKymera->chain_output_vol_handle, OPR_VOLUME_CONTROL);
    output_chain = theKymera->chain_output_vol_handle;
    connections = tone_music_inter_chain_connections;
    num_connections = ARRAY_DIM(tone_music_inter_chain_connections);

    switch (theKymera->state)
    {
        case KYMERA_STATE_TONE_PLAYING:
            /* Tone already playing, control should never reach here */
            DEBUG_LOGF("appKymeraHandleInternalTonePlay, already playing, state %u", theKymera->state);
            Panic();
            break;

        case KYMERA_STATE_SCO_ACTIVE:
            op = ChainGetOperatorByRole(theKymera->chain_sco_handle, OPR_VOLUME_CONTROL);
            output_chain = theKymera->chain_sco_handle;
            connections = tone_voice_inter_chain_connections;
            num_connections = ARRAY_DIM(tone_voice_inter_chain_connections);
            /* Fall through */
        case KYMERA_STATE_A2DP_STREAMING:
        case KYMERA_STATE_A2DP_STREAMING_WITH_FORWARDING:
            /* Already playing audio, can just mix tone in at output vol AUX_IN */
            appKymeraCreateToneChain(tone, theKymera->output_rate);
            /* Mute aux in port first */
            OperatorsVolumeSetAuxGain(op, volTo60thDbGain(0));
            /* Connect tone chain to output */
            ChainJoin(theKymera->chain_tone_handle, output_chain, num_connections, connections);
            /* Unmute */
            OperatorsVolumeSetAuxGain(op, APP_UI_TONE_VOLUME * KYMERA_DB_SCALE);
            /* May need to exit low power mode to play tone simultaneously */
            appKymeraConfigureDspPowerMode(TRUE);
            /* Start tone */
            ChainStart(theKymera->chain_tone_handle);
            theKymera->wait_for_tone = !interruptible;
            break;

        case KYMERA_STATE_IDLE:
            /* Need to set up audio output chain to play tone from scratch */
            appKymeraCreateOutputChain(KYMERA_TONE_GEN_RATE, KICK_PERIOD_TONES);
            appKymeraCreateToneChain(tone, KYMERA_TONE_GEN_RATE);
            /* Connect chains */
            output_chain = theKymera->chain_output_vol_handle;
            ChainJoin(theKymera->chain_tone_handle, output_chain, num_connections, connections);
            /* Unmute */
            op = ChainGetOperatorByRole(theKymera->chain_output_vol_handle, OPR_VOLUME_CONTROL);
            OperatorsVolumeMute(op, FALSE);
            OperatorsVolumeSetAuxGain(op, APP_UI_TONE_VOLUME * KYMERA_DB_SCALE);
            /* Enable external amplifier if required */
            appKymeraExternalAmpControl(TRUE);
            /* Start tone, Source Sync will drive main vol input with silence */
            ChainStart(theKymera->chain_output_vol_handle);
            ChainStart(theKymera->chain_tone_handle);
            /* Update state variables */
            theKymera->state = KYMERA_STATE_TONE_PLAYING;
            theKymera->output_rate = KYMERA_TONE_GEN_RATE;
            theKymera->wait_for_tone = !interruptible;
            break;

        default:
            /* Unknown state / not supported */
            DEBUG_LOGF("appKymeraHandleInternalTonePlay, unsupported state %u", theKymera->state);
            Panic();
            break;
    }
}

void appKymeraTonePlay(const ringtone_note *tone, bool interruptible)
{
    kymeraTaskData *theKymera = appGetKymera();

    DEBUG_LOGF("appKymeraTonePlay, queue tone %p, int %u", tone, interruptible);

    MAKE_KYMERA_MESSAGE(KYMERA_INTERNAL_TONE_PLAY);
    message->tone = tone;
    message->interruptible = interruptible;

    MessageSendConditionally(&theKymera->task, KYMERA_INTERNAL_TONE_PLAY, message, &theKymera->wait_for_tone);
}

static void appKymeraToneStop(void)
{
    kymeraTaskData *theKymera = appGetKymera();
    Operator op;

    DEBUG_LOGF("appKymeraToneStop, state %u", theKymera->state);

    /* Exit if there isn't a tone playing */
    if (theKymera->chain_tone_handle == NULL)
        return;

    op = ChainGetOperatorByRole(theKymera->chain_output_vol_handle, OPR_VOLUME_CONTROL);

    switch (theKymera->state)
    {
        case KYMERA_STATE_IDLE:
            /* No tone playing, control should never reach here */
            DEBUG_LOGF("appKymeraToneStop, already stopped, state %u", theKymera->state);
            Panic();
            break;

        case KYMERA_STATE_SCO_ACTIVE:
            op = ChainGetOperatorByRole(theKymera->chain_sco_handle, OPR_VOLUME_CONTROL);
            /* Fall through */
        case KYMERA_STATE_A2DP_STREAMING:
        case KYMERA_STATE_A2DP_STREAMING_WITH_FORWARDING:
            /* Just stop tone, leave audio playing */
            OperatorsVolumeSetAuxGain(op, volTo60thDbGain(0));
            ChainStop(theKymera->chain_tone_handle);
            ChainDestroy(theKymera->chain_tone_handle);
            theKymera->chain_tone_handle = NULL;
            /* Return to low power mode (if applicable) */
            appKymeraConfigureDspPowerMode(FALSE);
            break;

        case KYMERA_STATE_TONE_PLAYING:
            /* Mute and stop all chains */
            OperatorsVolumeSetMainAndAuxGain(op, volTo60thDbGain(0));
            OperatorsVolumeMute(op, TRUE);
            ChainStop(theKymera->chain_tone_handle);
            ChainStop(theKymera->chain_output_vol_handle);
            /* Disable external amplifier if required */
            appKymeraExternalAmpControl(FALSE);
            /* Destroy all chains */
            ChainDestroy(theKymera->chain_tone_handle);
            ChainDestroy(theKymera->chain_output_vol_handle);
            theKymera->chain_tone_handle = NULL;
            theKymera->chain_output_vol_handle = NULL;
            /* Move back to idle state */
            theKymera->state = KYMERA_STATE_IDLE;
            theKymera->output_rate = 0;
            break;

        default:
            /* Unknown state / not supported */
            DEBUG_LOGF("appKymeraToneStop, unsupported state %u", theKymera->state);
            Panic();
            break;
    }

    theKymera->wait_for_tone = FALSE;
}


bool appKymeraA2dpStart(uint8 device_id, uint8 stream_id, uint8 seid, Sink media_sink)
{
    kymeraTaskData *theKymera = appGetKymera();

    DEBUG_LOGF("appKymeraA2dpStart, queue sink %p", media_sink);

    /* Check the seid is supported */
    switch (seid)
    {
        case AV_SEID_SBC_SNK:
        case AV_SEID_AAC_SNK:
        case AV_SEID_APTX_SNK:
        case AV_SEID_APTX_MONO_TWS_SNK:
        case AV_SEID_SBC_MONO_TWS_SNK:
            break;

        default:
            /* Unsupported SEID */
            DEBUG_LOGF("appKymeraA2dpStart, unsupported seid %d", seid);
            return FALSE;
    }

    MAKE_KYMERA_MESSAGE(KYMERA_INTERNAL_A2DP_START);
    message->device_id  = device_id;
    message->stream_id  = stream_id;
    message->seid       = seid;
    message->media_sink = media_sink;

    MessageSendConditionally(&theKymera->task, KYMERA_INTERNAL_A2DP_START, message, &theKymera->wait_for_tone);

    return TRUE;
}

bool appKymeraA2dpStop(Sink media_sink)
{
    kymeraTaskData *theKymera = appGetKymera();

    DEBUG_LOGF("appKymeraA2dpStop msg, sink %p", media_sink);


    /* Make sure we have a valid sink */
    PanicNull(media_sink);

    if (!MessageCancelAll(&theKymera->task, KYMERA_INTERNAL_A2DP_START))
    {
        MAKE_KYMERA_MESSAGE(KYMERA_INTERNAL_A2DP_STOP);
        message->media_sink = media_sink;

        MessageSendConditionally(&theKymera->task, KYMERA_INTERNAL_A2DP_STOP, message, &theKymera->wait_for_tone);
    }

    return TRUE;
}

bool appKymeraA2dpStartForwarding(uint8 device_id, uint8 stream_id, uint8 seid, Sink forwarding_sink)
{
    kymeraTaskData *theKymera = appGetKymera();

    DEBUG_LOGF("appKymeraA2dpStartForwarding, queue sink %p, seid %d, state %u", forwarding_sink, seid, theKymera->state);

    /* Make sure we have a valid sink */
    PanicNull(forwarding_sink);

    /* Check the seid is supported */
    switch (seid)
    {
        case AV_SEID_APTX_MONO_TWS_SRC:
        case AV_SEID_SBC_MONO_TWS_SRC:
            break;

        default:
            /* Unsupported SEID */
            DEBUG_LOGF("appKymeraA2dpStartForwarding, unsupported sink %p, seid %d, state %u", forwarding_sink, seid, theKymera->state);
            return FALSE;
    }

    MAKE_KYMERA_MESSAGE(KYMERA_INTERNAL_A2DP_START_FORWARDING);
    message->device_id       = device_id;
    message->stream_id       = stream_id;
    message->seid            = seid;
    message->forwarding_sink = forwarding_sink;

    MessageSendConditionally(&theKymera->task, KYMERA_INTERNAL_A2DP_START_FORWARDING, message, &theKymera->wait_for_tone);

    return TRUE;
}

bool appKymeraA2dpStopForwarding(void)
{
    kymeraTaskData *theKymera = appGetKymera();

    if (!MessageCancelAll(&theKymera->task, KYMERA_INTERNAL_A2DP_START_FORWARDING))
    {
        return appKymeraHandleInternalA2dpStopForwarding();
    }
    else
        return TRUE;
}

void appKymeraA2dpSetVolume(uint16 volume)
{
    kymeraTaskData *theKymera = appGetKymera();

    DEBUG_LOGF("appKymeraA2dpSetVolume msg, vol %u", volume);

    MAKE_KYMERA_MESSAGE(KYMERA_INTERNAL_A2DP_SET_VOL);
    message->volume = volume;

    MessageSendConditionally(&theKymera->task, KYMERA_INTERNAL_A2DP_SET_VOL, message, &theKymera->wait_for_tone);
}

void appKymeraScoStart(Sink audio_sink, hfp_wbs_codec_mask codec, uint8 wesco)
{
    kymeraTaskData *theKymera = appGetKymera();

    DEBUG_LOGF("appKymeraScoStart, queue sink 0x%x", audio_sink);

    PanicNull(audio_sink);

    MAKE_KYMERA_MESSAGE(KYMERA_INTERNAL_SCO_START);
    message->audio_sink = audio_sink;
    message->codec      = codec;
    message->wesco      = wesco;

    MessageSendConditionally(&theKymera->task, KYMERA_INTERNAL_SCO_START, message, &theKymera->wait_for_tone);
}

void appKymeraScoStop(void)
{
    kymeraTaskData *theKymera = appGetKymera();

    DEBUG_LOG("appKymeraScoStop msg");

    MessageSendConditionally(&theKymera->task, KYMERA_INTERNAL_SCO_STOP, NULL, &theKymera->wait_for_tone);
}

void appKymeraScoSetVolume(uint8 volume)
{
    kymeraTaskData *theKymera = appGetKymera();

    DEBUG_LOGF("appKymeraScoSetVolume msg, vol %u", volume);

    MAKE_KYMERA_MESSAGE(KYMERA_INTERNAL_SCO_SET_VOL);
    message->volume = volume;

    MessageSendConditionally(&theKymera->task, KYMERA_INTERNAL_SCO_SET_VOL, message, &theKymera->wait_for_tone);
}

void appKymeraScoMicMute(bool mute)
{
    kymeraTaskData *theKymera = appGetKymera();

    DEBUG_LOGF("appKymeraScoMicMute msg, mute %u", mute);

    MAKE_KYMERA_MESSAGE(KYMERA_INTERNAL_SCO_MIC_MUTE);
    message->mute = mute;
    MessageSend(&theKymera->task, KYMERA_INTERNAL_SCO_MIC_MUTE, message);
}


static void kymera_dsp_msg_handler(MessageFromOperator *op_msg)
{
    PanicFalse(op_msg->len == KYMERA_OP_MSG_LEN);

    switch (op_msg->message[KYMERA_OP_MSG_WORD_MSG_ID])
    {
        case KYMERA_OP_MSG_ID_TONE_END:
            appKymeraToneStop();
        break;

        default:
        break;
    }
}

static void kymera_msg_handler(Task task, MessageId id, Message msg)
{
    UNUSED(task);
    switch (id)
    {
        case MESSAGE_FROM_OPERATOR:
            kymera_dsp_msg_handler((MessageFromOperator *)msg);
        break;

        case KYMERA_INTERNAL_A2DP_START:
        {
            KYMERA_INTERNAL_A2DP_START_T *m = (KYMERA_INTERNAL_A2DP_START_T *)msg;
            appKymeraHandleInternalA2dpStart(m->device_id, m->stream_id, m->seid, m->media_sink);
        }
        break;

        case KYMERA_INTERNAL_A2DP_START_FORWARDING:
        {
            KYMERA_INTERNAL_A2DP_START_FORWARDING_T *m = (KYMERA_INTERNAL_A2DP_START_FORWARDING_T *)msg;
            appKymeraHandleInternalA2dpStartForwarding(m->device_id, m->stream_id, m->seid, m->forwarding_sink);
        }
        break;

        case KYMERA_INTERNAL_A2DP_STOP:
        {
            KYMERA_INTERNAL_A2DP_STOP_T *m = (KYMERA_INTERNAL_A2DP_STOP_T *)msg;
            appKymeraHandleInternalA2dpStop(m->media_sink);
        }
        break;

        case KYMERA_INTERNAL_A2DP_SET_VOL:
        {
            KYMERA_INTERNAL_A2DP_SET_VOL_T *m = (KYMERA_INTERNAL_A2DP_SET_VOL_T *)msg;
            appKymeraHandleInternalA2dpSetVolume(m->volume);
        }
        break;

        case KYMERA_INTERNAL_SCO_START:
        {
            KYMERA_INTERNAL_SCO_START_T *m = (KYMERA_INTERNAL_SCO_START_T *)msg;
            appKymeraHandleInternalScoStart(m->audio_sink, m->codec, m->wesco);
        }
        break;

        case KYMERA_INTERNAL_SCO_SET_VOL:
        {
            KYMERA_INTERNAL_SCO_SET_VOL_T *m = (KYMERA_INTERNAL_SCO_SET_VOL_T *)msg;
            appKymeraHandleInternalScoSetVolume(m->volume);
        }
        break;

        case KYMERA_INTERNAL_SCO_MIC_MUTE:
        {
            KYMERA_INTERNAL_SCO_MIC_MUTE_T *m = (KYMERA_INTERNAL_SCO_MIC_MUTE_T *)msg;
            appKymeraHandleInternalScoMicMute(m->mute);
        }
        break;


        case KYMERA_INTERNAL_SCO_STOP:
        {
            appKymeraHandleInternalScoStop();
        }
        break;

        case KYMERA_INTERNAL_TONE_PLAY:
        {
            KYMERA_INTERNAL_TONE_PLAY_T *m = (KYMERA_INTERNAL_TONE_PLAY_T *)msg;
            appKymeraHandleInternalTonePlay(m->tone, m->interruptible);
        }
        break;

        default:
        break;
    }
}

void appKymeraInit(void)
{
    kymeraTaskData *theKymera = appGetKymera();
    theKymera->task.handler = kymera_msg_handler;
    theKymera->state = KYMERA_STATE_IDLE;
    theKymera->output_rate = 0;
    theKymera->wait_for_tone = FALSE;
    theKymera->a2dp_seid = AV_SEID_INVALID;
    appKymeraExternalAmpSetup();
    ChainSetDownloadableCapabilityBundleConfig(&bundle_config);
}
