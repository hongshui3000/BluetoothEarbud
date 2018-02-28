/* Copyright (c) 2016 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */
/**
 * \file
 * VM library module that implements USB Audio 1.0 class device support
 */
#ifndef USB_DEVICE_CLASS_REMOVE_AUDIO

#include <csrtypes.h>
#include <message.h>
#include <panic.h>
#include <sink.h>
#include <source.h>
#include <string.h>
#include <stream.h>
#include <usb.h>
/*#include <print.h>*/


#include "usb_device_class/usb_device_audio.h"
#include "usb_device_class/usb_device_class_private.h"

#include "assert.h"

static uint32 cur_speaker_sample_freq = SAMPLE_RATE_48K;
static uint32 cur_mic_sample_freq = SAMPLE_RATE_48K;

/** Default volume levels for speaker and mic */
static usb_device_class_audio_volume_config volume_def = {
        VOL_MIN,
        VOL_MAX,
        VOL_RES,
        VOL_MIN,
        VOL_MAX,
        VOL_RES,
        MIC_RES,
        MIC_DEFAULT
};

/** User-supplied volume levels for speaker and mic */
static usb_device_class_audio_volume_config volume_user;

/** Currently used volume levels for speaker and mic */
static usb_device_class_audio_volume_config* volume =
                                                      &volume_def;

/** Compares supplied volume levels to the default ones */
static bool usb_device_class_is_audio_volume_default(
        const usb_device_class_audio_volume_config* vol_config)
{
    return memcmp(vol_config, &volume_def, sizeof(volume_def)) == 0;
}   

/** UsbCodes values for USB control interface class */
static const UsbCodes usb_codes_ac = {
        B_INTERFACE_CLASS_AUDIO, /* bInterfaceClass */
        B_INTERFACE_SUB_CLASS_AUDIOCONTROL, /* bInterfaceSubClass */
        PR_PROTOCOL_UNDEFINED, /* bInterfaceProtocol */
        I_INTERFACE_INDEX /* iInterface */
};

/** UsbCodes values for USB streaming interface class */
static const UsbCodes usb_codes_as = {
        B_INTERFACE_CLASS_AUDIO, /* bInterfaceClass */
        B_INTERFACE_SUB_CLASS_AUDIOSTREAMING, /* bInterfaceSubClass */
        PR_PROTOCOL_UNDEFINED, /* bInterfaceProtocol */
        I_INTERFACE_INDEX /* iInterface */
};

/**
 * Default USB Audio Descriptors
 *
 * Default descriptors declare the dongle as a microphone and speaker
 * that support 16 bit mono PCM at 8khz. This is just an example,
 * Application can supply it's own descriptors with UsbDeviceClassConfigure.
 *
 * Terminal Architecture
 *
 * Microphone(4) -> Feature Unit(5) -> USB Output Terminal(6)
 *
 * USB Input Terminal(1) -> Feature Unit(2) -> Speaker(3)
 * USB Streaming:
 * Isochronous, mono, 16 bit, PCM input and output.
 *
 * Format of PCM stream:
 * | Mono LSB | Mono MSB |
 */

/** Default USB control interface descriptors for speaker + mic */
static const uint8 interface_descriptor_control_mic_and_speaker[] =
{
    /* Class Specific Header */
    0x0A,         /* bLength */
    0x24,         /* bDescriptorType = CS_INTERFACE */
    0x01,         /* bDescriptorSubType = HEADER */
    0x00, 0x01,   /* bcdADC = Audio Device Class v1.00 */
    0x0A + 0x0c + 0x0b + 0x09 + 0x0c + 0x0d + 0x09, /* wTotalLength LSB */
    0x00,         /* wTotalLength MSB */
    0x02,         /* bInCollection = 2 AudioStreaming interfaces */
    0x01,         /* baInterfaceNr(1) - AS#1 id */
    0x02,         /* baInterfaceNr(2) - AS#2 id */

    /* Microphone IT */
    0x0c,         /* bLength */
    0x24,         /* bDescriptorType = CS_INTERFACE */
    0x02,         /* bDescriptorSubType = INPUT_TERMINAL */
    MIC_IT,       /* bTerminalID */
    0x03, 0x02,   /* wTerminalType = Personal Microphone */
    0x00,         /* bAssocTerminal = none */
    0x01,         /* bNrChannels = 1 */
    0x00, 0x00,   /* wChannelConfig = mono */
    0x00,         /* iChannelName = no string */
    0x00,         /* iTerminal = same as USB product string */
    
    /* Microphone Features */
    0x0b,           /*bLength*/
    0x24,           /*bDescriptorType = CS_INTERFACE */
    0x06,           /*bDescriptorSubType = FEATURE_UNIT*/
    MIC_FU,         /*bUnitId*/
    MIC_IT,         /*bSourceId - Microphone IT*/
    0x02,           /*bControlSize = 2 bytes per control*/
    0x01, 0x00,     /*bmaControls[0] = 0001 (Mute on Master Channel)*/
    0x00, 0x00,     /*bmaControls[1] = 0000 (No gain control)*/
    0x00,           /*iFeature = same as USB product string*/

    /* Microphone OT */
    0x09,         /* bLength */
    0x24,         /* bDescriptorType = CS_INTERFACE */
    0x03,         /* bDescriptorSubType = OUTPUT_TERMINAL */
    MIC_OT,       /* bTerminalID */
    0x01, 0x01,   /* wTerminalType = USB streaming */
    0x00,         /* bAssocTerminal = none */
    MIC_FU,       /* bSourceID - Microphone Features */
    0x00,         /* iTerminal = same as USB product string */

    /* Speaker IT */
    0x0c,         /* bLength */
    0x24,         /* bDescriptorType = CS_INTERFACE */
    0x02,         /* bDescriptorSubType = INPUT_TERMINAL */
    SPEAKER_IT,   /* bTerminalID */
    0x01, 0x01,   /* wTerminalType = USB streaming */
    0x00,         /* bAssocTerminal = none */
    0x02,         /* bNrChannels = 2 */
    0x03, 0x00,   /* wChannelConfig = left front and right front */
    0x00,         /* iChannelName = no string */
    0x00,         /* iTerminal = same as USB product string */
    
    /* Speaker Features */
    0x0d,           /*bLength*/
    0x24,           /*bDescriptorType = CS_INTERFACE */
    0x06,           /*bDescriptorSubType = FEATURE_UNIT*/
    SPEAKER_FU,     /*bUnitId*/
    SPEAKER_IT,     /*bSourceId - Speaker IT*/
    0x02,           /*bControlSize = 2 bytes per control*/
    0x01, 0x00,     /*bmaControls[0] = 0001 (Mute on Master Channel)*/
    0x02, 0x00,     /*bmaControls[1] = 0002 (Vol on Left Front)*/
    0x02, 0x00,     /*bmaControls[1] = 0002 (Vol on Right Front)*/
    0x00,           /*iFeature = same as USB product string*/

    /* Speaker OT */
    0x09,         /* bLength */
    0x24,         /* bDescriptorType = CS_INTERFACE */
    0x03,         /* bDescriptorSubType = OUTPUT_TERMINAL */
    SPEAKER_OT,   /* bTerminalID */
    0x01, 0x03,   /* wTerminalType = Speaker */
    0x00,         /* bAssocTerminal = none */
    SPEAKER_FU,   /* bSourceID - Speaker Features*/
    0x00,         /* iTerminal = same as USB product string */
};

/** Default USB control interface descriptors for mic */
static const uint8 interface_descriptor_control_mic[] =
{
    /* Class Specific Header */
    0x09,         /* bLength */
    0x24,         /* bDescriptorType = CS_INTERFACE */
    0x01,         /* bDescriptorSubType = HEADER */
    0x00, 0x01, /* bcdADC = Audio Device Class v1.00 */
    0x09 + 0x0c + 0x0b + 0x09, /* wTotalLength LSB */
    0x00,         /* wTotalLength MSB */
    0x01,         /* bInCollection = 1 AudioStreaming interface */
    0x01,         /* baInterfaceNr(1) - AS#1 id */

    /* Microphone IT */
    0x0c,         /* bLength */
    0x24,         /* bDescriptorType = CS_INTERFACE */
    0x02,         /* bDescriptorSubType = INPUT_TERMINAL */
    MIC_IT,       /* bTerminalID */
    0x03, 0x02,   /* wTerminalType = Personal Microphone */
    0x00,         /* bAssocTerminal = none */
    0x01,         /* bNrChannels = 1 */
    0x00, 0x00,   /* wChannelConfig = mono */
    0x00,         /* iChannelName = no string */
    0x00,         /* iTerminal = same as USB product string */
    
    /* Microphone Features */
    0x0b,           /*bLength*/
    0x24,           /*bDescriptorType = CS_INTERFACE */
    0x06,           /*bDescriptorSubType = FEATURE_UNIT*/
    MIC_FU,         /*bUnitId*/
    MIC_IT,         /*bSourceId - Microphone IT*/
    0x02,           /*bControlSize = 2 bytes per control*/
    0x01, 0x00,     /*bmaControls[0] = 0001 (Mute on Master Channel)*/
    0x00, 0x00,     /*bmaControls[1] = 0000 (No gain control)*/
    0x00,           /*iFeature = same as USB product string*/

    /* Microphone OT */
    0x09,         /* bLength */
    0x24,         /* bDescriptorType = CS_INTERFACE */
    0x03,         /* bDescriptorSubType = OUTPUT_TERMINAL */
    MIC_OT,       /* bTerminalID */
    0x01, 0x01,   /* wTerminalType = USB streaming */
    0x00,         /* bAssocTerminal = none */
    MIC_FU,       /* bSourceID - Microphone Features */
    0x00,         /* iTerminal = same as USB product string */
};

/** Default USB control interface descriptors for speaker */
static const uint8 interface_descriptor_control_speaker[] =
{
    /* Class Specific Header */
    0x09,         /* bLength */
    0x24,         /* bDescriptorType = CS_INTERFACE */
    0x01,         /* bDescriptorSubType = HEADER */
    0x00, 0x01, /* bcdADC = Audio Device Class v1.00 */
    0x09 + 0x0c + 0x0d + 0x09, /* wTotalLength LSB */
    0x00,         /* wTotalLength MSB */
    0x01,         /* bInCollection = 1 AudioStreaming interface */
    0x01,         /* baInterfaceNr(1) - AS#1 id */
    
    /* Speaker IT */
    0x0c,         /* bLength */
    0x24,         /* bDescriptorType = CS_INTERFACE */
    0x02,         /* bDescriptorSubType = INPUT_TERMINAL */
    SPEAKER_IT,   /* bTerminalID */
    0x01, 0x01,   /* wTerminalType = USB streaming */
    0x00,         /* bAssocTerminal = none */
    0x02,         /* bNrChannels = 2 */
    0x03, 0x00,   /* wChannelConfig = left front and right front */
    0x00,         /* iChannelName = no string */
    0x00,         /* iTerminal = same as USB product string */
    
    /* Speaker Features */
    0x0d,           /*bLength*/
    0x24,           /*bDescriptorType = CS_INTERFACE */
    0x06,           /*bDescriptorSubType = FEATURE_UNIT*/
    SPEAKER_FU,     /*bUnitId*/
    SPEAKER_IT,     /*bSourceId - Speaker IT*/
    0x02,           /*bControlSize = 2 bytes per control*/
    0x01, 0x00,     /*bmaControls[0] = 0001 (Mute on Master Channel)*/
    0x02, 0x00,     /*bmaControls[1] = 0002 (Vol on Left Front)*/
    0x02, 0x00,     /*bmaControls[1] = 0002 (Vol on Right Front)*/
    0x00,           /*iFeature = same as USB product string*/

    /* Speaker OT */
    0x09,         /* bLength */
    0x24,         /* bDescriptorType = CS_INTERFACE */
    0x03,         /* bDescriptorSubType = OUTPUT_TERMINAL */
    SPEAKER_OT,   /* bTerminalID */
    0x01, 0x03,   /* wTerminalType = Speaker */
    0x00,         /* bAssocTerminal = none */
    SPEAKER_FU,   /* bSourceID - Speaker Features*/
    0x00,         /* iTerminal = same as USB product string */
};

/** Default USB streaming interface descriptors for speaker */
static const uint8 interface_descriptor_streaming_speaker[] =
{
    /* Class Specific AS interface descriptor */
    0x07,         /* bLength */
    0x24,         /* bDescriptorType = CS_INTERFACE */
    0x01,         /* bDescriptorSubType = AS_GENERAL */
    SPEAKER_IT,   /* bTerminalLink = Speaker IT */
    0x00,         /* bDelay */
    0x01, 0x00, /* wFormatTag = PCM */

    /* Type 1 format type descriptor */
    0x08 + 0x12,  /* bLength 8+((number of sampling frequencies)*3) */
    0x24,         /* bDescriptorType = CS_INTERFACE */
    0x02,         /* bDescriptorSubType = FORMAT_TYPE */
    0x01,         /* bFormatType = FORMAT_TYPE_I */
    0x02,         /* bNumberOfChannels */
    0x02,         /* bSubframeSize = 2 bytes */
    0x10,         /* bBitsResolution */
    0x06,         /* bSampleFreqType = 6 discrete sampling frequencies */
    SAMPLE_RATE_48K & 0xff, /* tSampleFreq = 48000*/
    (SAMPLE_RATE_48K >> 8) & 0xff,
    /*lint -e{572}
     * we don't care that SAMPLE_RATE_{48K,44K1,32K,22K05,16K,8K} >> 16 == 0 */
    (SAMPLE_RATE_48K >> 16) & 0xff,
    SAMPLE_RATE_44K1 & 0xff, /* tSampleFreq = 44100*/
    (SAMPLE_RATE_44K1 >> 8) & 0xff,
    (SAMPLE_RATE_44K1 >> 16) & 0xff,
    SAMPLE_RATE_32K & 0xff, /* tSampleFreq = 32000 */
    (SAMPLE_RATE_32K >> 8 ) & 0xff,
    (SAMPLE_RATE_32K >> 16) & 0xff,
    SAMPLE_RATE_22K05 & 0xff, /* tSampleFreq = 22050 */
    (SAMPLE_RATE_22K05 >> 8 ) & 0xff,
    (SAMPLE_RATE_22K05 >> 16) & 0xff,
    SAMPLE_RATE_16K & 0xff, /* tSampleFreq = 16000 */
    (SAMPLE_RATE_16K >> 8 ) & 0xff,
    (SAMPLE_RATE_16K >> 16) & 0xff,
    SAMPLE_RATE_8K & 0xff, /* tSampleFreq = 8000 */
    (SAMPLE_RATE_8K >> 8) & 0xff,
    (SAMPLE_RATE_8K >> 16) & 0xff,
    

    /* Class specific AS isochronous audio data endpoint descriptor */
    0x07,         /* bLength */
    0x25,         /* bDescriptorType = CS_ENDPOINT */
    0x01,         /* bDescriptorSubType = AS_GENERAL */
    0x81,         /* bmAttributes = MaxPacketsOnly and
                     SamplingFrequency control */
    0x02,         /* bLockDelayUnits = Decoded PCM samples */
    0x00, 0x00     /* wLockDelay */
};

/** Default USB streaming interface descriptors for mic */
static const uint8 interface_descriptor_streaming_mic[] =
{
    /* Class Specific AS interface descriptor */
    0x07,         /* bLength */
    0x24,         /* bDescriptorType = CS_INTERFACE */
    0x01,         /* bDescriptorSubType = AS_GENERAL */
    MIC_OT,       /* bTerminalLink = Microphone OT */
    0x00,         /* bDelay */
    0x01, 0x00, /* wFormatTag = PCM */

    /* Type 1 format type descriptor */
    0x08 + 0x03,/* bLength */
    0x24,        /* bDescriptorType = CS_INTERFACE */
    0x02,         /* bDescriptorSubType = FORMAT_TYPE */
    0x01,         /* bFormatType = FORMAT_TYPE_I */
    0x01,         /* bNumberOfChannels */
    0x02,         /* bSubframeSize = 2 bytes */
    0x10,         /* bBitsResolution */
    0x01,         /* bSampleFreqType = 1 discrete sampling freq */
    SAMPLE_RATE & 0xff, /* tSampleFreq */
    (SAMPLE_RATE >> 8) & 0xff,
    /*lint -e(572)
     * we don't care that SAMPLE_RATE >> 16 == 0 */
    (SAMPLE_RATE >> 16) & 0xff,

    /* Class specific AS isochronous audio data endpoint descriptor */
    0x07,         /* bLength */
    0x25,         /* bDescriptorType = CS_ENDPOINT */
    0x01,         /* bDescriptorSubType = AS_GENERAL */
    0x00,         /* bmAttributes = none */
    0x02,         /* bLockDelayUnits = Decoded PCM samples */
    0x00, 0x00     /* wLockDelay */
};

/** Audio endpoint data, provides bRefresh and bSyncAddress */
static const uint8 audio_endpoint_user_data[] =
{
    0, /* bRefresh */
    0  /* bSyncAddress */
};


/**  Streaming Isochronous Endpoint,
 * maximum packet size 192 (stereo at 48khz) */
static const EndPointInfo epinfo_streaming_speaker[] =
{
    {
        end_point_iso_in, /** address */
        end_point_attr_iso, /** attributes */
        192, /** max packet size */
        1, /** poll_interval */
        audio_endpoint_user_data, /** data to be appended */
        sizeof(audio_endpoint_user_data) /** length of data appended */
    }
};


/** Streaming Isochronous Endpoint,
 * maximum packet size 96 (mono at 48khz) */
static const EndPointInfo epinfo_streaming_mic[] =
{
    {
        end_point_iso_out, /** address */
        end_point_attr_iso, /** attributes */
        96, /** max packet size */
        1, /** poll_interval */
        audio_endpoint_user_data, /** data to be appended */
        sizeof(audio_endpoint_user_data), /** length of data appended */
    }
};

/** Default device configuration with interface and endpoint descriptors
 * for both speaker and mic */
static const usb_device_class_audio_config usb_audio_config_mic_and_speaker =
{
    {
            interface_descriptor_control_mic_and_speaker,
            sizeof(interface_descriptor_control_mic_and_speaker),
            NULL
    },
    {
            interface_descriptor_streaming_mic,
            sizeof(interface_descriptor_streaming_mic),
            epinfo_streaming_mic
    },
    {
            interface_descriptor_streaming_speaker,
            sizeof(interface_descriptor_streaming_speaker),
            epinfo_streaming_speaker
    }
};

/** Default device configuration with interface and endpoint descriptors
 * for just mic */
static const usb_device_class_audio_config usb_audio_config_mic =
{
    {
            interface_descriptor_control_mic,
            sizeof(interface_descriptor_control_mic),
            NULL
    },
    {
            interface_descriptor_streaming_mic,
            sizeof(interface_descriptor_streaming_mic),
            epinfo_streaming_mic
    },
    {
            NULL, 0, NULL
    }
};

/** Default device configuration with interface and endpoint descriptors
 * for just speaker */
static const usb_device_class_audio_config usb_audio_config_speaker =
{
    {
            interface_descriptor_control_speaker,
            sizeof(interface_descriptor_control_speaker),
            NULL
    },
    {
            NULL, 0, NULL
    },
    {
            interface_descriptor_streaming_speaker,
            sizeof(interface_descriptor_streaming_speaker),
            epinfo_streaming_speaker
    }
};

/** Stores device configuration with interface and endpoint descriptors */
static const usb_device_class_audio_config* usb_audio_config = NULL;

/** Parses supplied descriptor and populates the list of supported rates */
static uint8 usbAudioGetRate(usb_device_class_descriptor descriptor,
                             uint32 *supported_rates)
{
    const uint8* ptr = descriptor.descriptor;
    const uint8* end = ptr + descriptor.size_descriptor;

    while(ptr < end)
    {
        const usb_format_descriptor* format;
        format = (const usb_format_descriptor*) ptr;

        if ((format->header.type == B_DESCRIPTOR_TYPE_CS_INTERFACE) &&
            (format->header.sub_type == B_DESCRIPTOR_SUB_TYPE_FORMAT) &&
            (format->type == B_DESCRIPTOR_FORMAT_TYPE_I))
        {
            uint8 i;
            for (i = 0;
                    (i < format->num_freq) && (i < SUPPORTED_FREQUENCIES); i++)
            {
                supported_rates[i] = (format->freq[i*3]) |
                             ((uint16)format->freq[(i*3)+1] << 8) |
                             ((uint32)format->freq[(i*3)+2] << 16);
            }
            return i;
        }
        ptr += format->header.length;
    }
    return 0;
}

/** Get pointer to the location with the requested control object value
 *
 * The following control requests are supported:
 * VOLUME: current, min, max, res values for left, right and master channels
 *         for mic and speaker
 * MUTE: current master channel value for mic and speaker
 * */
static uint16* audioGetPtr(uint8 control, uint8 code,
                           uint8 unit_id, uint8 channel)
{
    switch (control)
    {
    case VOLUME_CONTROL:
        /* Can get any of CUR, MIN, MAX, RES for vol */
        switch (unit_id)
        {
        case SPEAKER_FU:
            L3_DBG_MSG2("SPEAKER VOLUME CODE - 0x%X CHANNEL - 0x%X\n",
                        code, channel);
            switch (code)
            {
            case REQ_CUR:
                /* Get pointer to vol for this channel */
                switch (channel)
                {
                case CHANNEL_LEFT:
                case CHANNEL_MASTER:
                    return (uint16*)&device->audio.out_l_vol;
                case CHANNEL_RIGHT:
                    return (uint16*)&device->audio.out_r_vol;
                default:
                    break;
                }
                break;
            case REQ_MIN:
                return &volume->speaker_min;
            case REQ_MAX:
                return &volume->speaker_max;
            case REQ_RES:
                return &volume->speaker_res;
            default:
                break;
            }
            break;

        case MIC_FU:
            L3_DBG_MSG2("MIC VOLUME CODE - 0x%X CHANNEL - 0x%X\n",
                        code, channel);
            switch (code)
            {
            case REQ_CUR:
                /* Get pointer to vol for this channel */
                if((channel == CHANNEL_LEFT) ||
                   (channel == CHANNEL_MASTER))
                {
                    return (uint16*)&device->audio.in_vol;
                }
                break;
            case REQ_MIN:
                return &volume->mic_min;
            case REQ_MAX:
                return &volume->mic_max;
            case REQ_RES:
                return &volume->mic_res;
            default:
                break;
            }
            break;
        default:
            break;
        }
        break;

    case MUTE_CONTROL:
        if (code == REQ_CUR && channel == CHANNEL_MASTER)
        {
            switch (unit_id)
            {
            case MIC_FU:
                L3_DBG_MSG2("MIC MUTE CODE - 0x%X CHANNEL - 0x%X\n",
                            code, channel);
                return &device->audio.in_mute;
            case SPEAKER_FU:
                L3_DBG_MSG2("SPEAKER MUTE CODE - 0x%X CHANNEL - 0x%X\n",
                            code, channel);
                return &device->audio.out_mute;
            default:
                break;
            }
        }
        break;
    default:
        break;
    }
    /* Unsupported request */
    return NULL;
}

/** Get requested audio level and push the value into the sink */
static uint16 audioGetLevel(Sink sink, uint8 control, uint8 code,
                            uint8 unit_id, uint8 channel)
{
    uint8*  p_snk;

    /* Get pointer to requested value */
    uint16* pv = audioGetPtr(control, code, unit_id, channel);

    if(pv)
    {
        L3_DBG_MSG3("VAL - 0x%X Control - %d Code - %d\n",
                    *pv, control, code);
        if(control == MUTE_CONTROL)
        {
            /* Mute is 1 bytes */
            *claimSink(sink, 1) = (uint8)*pv;
            return 1;
        }
        else if(control == VOLUME_CONTROL)
        {
            /* Volume is 2 bytes */
            p_snk    = claimSink(sink, 2);
            p_snk[0] = (uint8)((*pv) & 0xFF);
            p_snk[1] = (uint8)((*pv) >> 8);
            return 2;
        }
    }
    /* Unsupported request */
    return 0;
}

/** Round received volume level */
static void audioGainRound(uint8 hi, uint8 lo, int16* res)
{
    /* Convert hi byte to signed int */
    int16 temp = (uint16)((hi & 0x80) ? (hi - 0xFF) : hi);

    /* Round up if lo byte >= 0.5 (where 1 = 1/256) */
    if(lo >= 0x80)
    {
        temp ++;
    }

    /* Round to nearest 3dB */
    for((*res) = -44; (*res) < 1; (*res)+= 3)
    {
        if(temp <= (*res))
        {
            break;
        }
    }
    (*res)--;
}

/** Read the level value from the source, store it in the volume control
 * and notify Application with the USB_DEVICE_CLASS_MSG_AUDIO_LEVELS_IND
 * message. */
static bool audioSetLevel(Source source, uint8 control, uint8 code,
                          uint8 unit_id, uint8 channel)
{
    int16 val = 0;
    const uint8* p_src = SourceMap(source);
    uint16* pv = audioGetPtr(control, code, unit_id, channel);

    /* Only allow setting current levels */
    if (!pv || (code != REQ_CUR))
    {
        return FALSE;
    }

    if (control == MUTE_CONTROL)
    {
        if (SourceSize(source) >= 1)
        {
            val = (int16)p_src[0];
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        if (SourceSize(source) >= 2)
        {
            if (device->audio.user_audio_levels)
            {
                /* if user defined levels then just store raw value */
                val = (int16)(p_src[0] | (p_src[1] << 8));
            }
            else
            {
                /* round audio level */
                audioGainRound(p_src[1], p_src[0], &val);
            }
        }
        else
        {
            return FALSE;
        }
    }

    if ((int16)(*pv) != val)
    {
        *pv = (uint16)val;
        /* Level has changed, notify application */
        (void)MessageCancelFirst(device->app_task,
                                 USB_DEVICE_CLASS_MSG_AUDIO_LEVELS_IND);
        MessageSendLater(device->app_task,
                         USB_DEVICE_CLASS_MSG_AUDIO_LEVELS_IND, NULL, 10);
    }
    L3_DBG_MSG1("VAL - 0x%X\n", *pv);
    /* Respond with success */
    return TRUE;
}

/** Handle streaming interface class control requests */
static void handleAudioStreamingClassRequest(usb_interface_type interface_type,
                                             Source req)
{
    uint16 packet_size;
    Sink sink = StreamSinkFromSource(req);

    /* Check for outstanding Class requests */
    while ((packet_size = SourceBoundary(req)) != 0)
    {
        /*
            Build the response. It must contain the original request,
            so copy from the source header.
        */
        UsbResponse usbresp;
        uint16f header_size;

        header_size = SourceSizeHeader(req) * sizeof(uint16);
        assert(header_size == sizeof(UsbResponse));

        memcpy(&usbresp.original_request, SourceMapHeader(req),
               sizeof(UsbRequest));

        /* Set the response fields to default values
         * to make the code below simpler */
        usbresp.success = FALSE;
        usbresp.data_length = 0;

        /* Endpoint only allows SET_/GET_ of sampling frequency */
        if ((REQ_CS(usbresp.original_request) == SAMPLING_FREQ_CONTROL) &&
            (usbresp.original_request.wLength == 3))
        {
            L3_DBG_MSG1("Sampling Freq Request %d\n",
                        usbresp.original_request.wIndex);
            /* Check this is for a valid audio end point */
            if(usbresp.original_request.wIndex == end_point_iso_in ||
                    usbresp.original_request.wIndex == end_point_iso_out)
            {
                /* Get the supported rate */
                uint32 supported_rates[SUPPORTED_FREQUENCIES];
                uint8 num_freq;
                if(usbresp.original_request.wIndex == end_point_iso_in)
                {
                    num_freq = usbAudioGetRate(
                            usb_audio_config->streaming_speaker,
                            supported_rates);
                }
                else
                {
                    num_freq = usbAudioGetRate(
                            usb_audio_config->streaming_mic,
                            supported_rates);
                }

                if (usbresp.original_request.bRequest == SET_CUR)
                {
                    uint8 i;
                    const uint8* rate = SourceMap(req);
                    uint32 new_rate  = (uint32)rate[0] |
                                      ((uint32)rate[1] << 8) |
                                      ((uint32)rate[2] << 16);

                    /* Reject bad value */
                    for (i=0; i < num_freq; i++)
                    {
                        if (supported_rates[i] == new_rate)
                        {
                            MAKE_USB_DEVICE_CLASS_MESSAGE(
                                    USB_DEVICE_CLASS_SAMPLE_RATE);
                            message->sample_rate = new_rate;
                            usbresp.success = TRUE;
                            /* store sample rate in static variable */
                            if(interface_type ==
                                    usb_interface_audio_streaming_speaker)
                            {
                                cur_speaker_sample_freq = new_rate;
                                L3_DBG_MSG2("Set Speaker Rate 0x%x - %d\n",
                                            new_rate, usbresp.success);
                                /* notify VM application
                                 * of a change in sample rate */
                                MessageSend(device->app_task,
                                   USB_DEVICE_CLASS_MSG_SPEAKER_SAMPLE_RATE_IND,
                                   message);
                            }
                            else
                            {
                                cur_mic_sample_freq = new_rate;
                                L3_DBG_MSG2("Set MicRate 0x%x - %d\n",
                                            new_rate, usbresp.success);
                                /* notify VM application
                                 * of a change in sample rate */
                                MessageSend(device->app_task,
                                       USB_DEVICE_CLASS_MSG_MIC_SAMPLE_RATE_IND,
                                       message);
                            }
                            break;
                        }
                    }
                }
                else if (usbresp.original_request.bRequest == GET_CUR)
                {
                    /* Return current value */
                    uint8 *ptr;
                    if ((ptr = claimSink(sink, 3)) != 0)
                    {
                        /* store sample rate in static variable */
                        if(interface_type ==
                                usb_interface_audio_streaming_speaker)
                        {
                            ptr[0] = (uint8)(cur_speaker_sample_freq & 0xff);
                            ptr[1] = (uint8)(cur_speaker_sample_freq >> 8);
                            ptr[2] = (uint8)(cur_speaker_sample_freq >> 16);
                            L3_DBG_MSG1("Get Speaker Rate 0x%x\n",
                                        cur_speaker_sample_freq);
                        }
                        else
                        {
                            ptr[0] = (uint8)(cur_mic_sample_freq & 0xff);
                            ptr[1] = (uint8)(cur_mic_sample_freq >> 8);
                            ptr[2] = (uint8)(cur_mic_sample_freq >> 16);
                            L3_DBG_MSG1("Get Mic Rate 0x%x\n",
                                        cur_mic_sample_freq);
                        }
                        usbresp.data_length = 3;
                        usbresp.success = TRUE;
                    }
                }
            }
        }

        if (usbresp.data_length)
        {
            (void)SinkFlushHeader(sink, usbresp.data_length,
                                  (void *)&usbresp, sizeof(UsbResponse));
        }
        else
        {
            /* Sink packets can never be zero-length, so flush a dummy byte */
            (void)SinkClaim(sink, 1);
            (void)SinkFlushHeader(sink, 1, (void *)&usbresp,
                                  sizeof(UsbResponse));
        }
        /* Discard the original request */
        SourceDrop(req, packet_size);

#if defined(HOSTIO_USB_VM_STUB) && defined(DEBUG_STREAM_USB_CLASS)
        L5_DBG_MSG4("handleAudioStreamingClassRequest: id=0x%x "
                    "bmRequestType=0x%x bRequest=0x%x wValue=0x%x",
                   usbresp.original_request.id,
                   usbresp.original_request.bmRequestType,
                   usbresp.original_request.bRequest,
                   usbresp.original_request.wValue);
        L5_DBG_MSG4("handleAudioStreamingClassRequest: "
                    "wIndex=0x%x wLength=0x%x success=0x%x length=0x%x",
                   usbresp.original_request.wIndex,
                   usbresp.original_request.wLength,
                   usbresp.success,
                   usbresp.data_length);
#endif
    }
}

/** Task handler for streaming endpoint source and sink streams */
static void audioStreamingHandler(Task task, MessageId id, Message message)
{
    UNUSED(task);
    UNUSED(message);

    if (id == MESSAGE_MORE_DATA)
    {
        L3_DBG_MSG("USB: MESSAGE_MORE_DATA audio streaming\n");
        if (device->usb_classes & USB_DEVICE_CLASS_TYPE_AUDIO_SPEAKER)
        {
            handleAudioStreamingClassRequest(
                    usb_interface_audio_streaming_speaker,
                    StreamUsbClassSource(SPEAKER_INTF));
        }
        if (device->usb_classes & USB_DEVICE_CLASS_TYPE_AUDIO_MICROPHONE)
        {
            handleAudioStreamingClassRequest(
                    usb_interface_audio_streaming_mic,
                    StreamUsbClassSource(MIC_INTF));
        }
    }
}

/** Enumerate streaming mic
 *
 * Add USB interface, endpoints, enable messages, register task handler and
 * set the Sink task */
static bool usbEnumerateAudioMic(usb_device_class_descriptor descriptor)
{
    UsbInterface intf;

    MIC_INTF = usb_interface_error;

    /* Add the microphone Audio Streaming Interface */
    intf = (UsbInterface)UsbAddInterface(&usb_codes_as,
                                         B_DESCRIPTOR_TYPE_CS_INTERFACE,
                                         descriptor.descriptor,
                                         descriptor.size_descriptor);
    
    if (intf == usb_interface_error)
    {
        return FALSE;
    }

    /* Add the microphone endpoint */
    if (UsbAddEndPoints(intf, 1, descriptor.end_point_info) == FALSE)
    {
        return FALSE;
    }

    STREAM_TASK.handler = audioStreamingHandler;
    (void)MessageStreamTaskFromSink(StreamUsbClassSink(intf), &STREAM_TASK);

#ifdef HOSTIO_USB_VM_STUB
    (void)SourceConfigure(StreamUsbClassSource(intf),
                          VM_SOURCE_MESSAGES, VM_MESSAGES_ALL);
    (void)SinkConfigure(StreamUsbClassSink(intf),
                        VM_SINK_MESSAGES, VM_MESSAGES_ALL);
#endif

    /* set mic default volume */
    device->audio.in_vol = (int16)volume->mic_default;

    MIC_INTF = intf;

    return TRUE;
}

/** Enumerate streaming speaker
 *
 * Add USB interface, endpoints, enable messages, register task handler and
 * set the Source task */
static bool usbEnumerateAudioSpeaker(usb_device_class_descriptor descriptor)
{
    UsbInterface intf;

    SPEAKER_INTF = usb_interface_error;

    /* Add the speaker Audio Streaming Interface */
    intf = (UsbInterface)UsbAddInterface(&usb_codes_as,
                                         B_DESCRIPTOR_TYPE_CS_INTERFACE,
                                         descriptor.descriptor,
                                         descriptor.size_descriptor);
    
    if (intf == usb_interface_error)
    {
        return FALSE;
    }

    /* Add the speaker endpoint */
    if (UsbAddEndPoints(intf, 1, descriptor.end_point_info) == FALSE)
    {
        return FALSE;
    }

    STREAM_TASK.handler = audioStreamingHandler;
    (void) MessageStreamTaskFromSink(StreamUsbClassSink(intf), &STREAM_TASK);

#ifdef HOSTIO_USB_VM_STUB
    (void)SourceConfigure(StreamUsbClassSource(intf),
                          VM_SOURCE_MESSAGES, VM_MESSAGES_ALL);
    (void)SinkConfigure(StreamUsbClassSink(intf),
                        VM_SINK_MESSAGES, VM_MESSAGES_ALL);
#endif

    /* set speaker default volume */
    device->audio.out_l_vol = (int16)volume->speaker_default;
    device->audio.out_r_vol = device->audio.out_l_vol;

    SPEAKER_INTF = intf;
            
    return TRUE;
}

/** Handle control interface class control requests */
static void handleAudioControlClassRequest(Source req)
{
    uint16 packet_size;
    Sink sink = StreamSinkFromSource(req);

    uint8 channel;
    uint8 unit_id;
    uint8 code;
    uint8 control;

    /* Check for outstanding Class requests */
    while ((packet_size = SourceBoundary(req)) != 0)
    {
        /*
            Build the response. It must contain the original request,
            so copy from the source header.
        */
        UsbResponse usbresp;
        uint16f header_size;

        header_size = SourceSizeHeader(req) * sizeof(uint16);
        assert(header_size == sizeof(UsbResponse));

        memcpy(&usbresp.original_request, SourceMapHeader(req),
               sizeof(UsbRequest));

        /* Reject by default */
        usbresp.data_length = 0;
        usbresp.success = FALSE;

        /* Get audio specific request info */
        channel = REQ_CN(usbresp.original_request);
        control = REQ_CS(usbresp.original_request);
        code    = REQ_CODE(usbresp.original_request);
        unit_id = REQ_UNIT(usbresp.original_request);

        if(REQ_IS_GET(usbresp.original_request))
        {
            /* Return value to USB host */
            L3_DBG_MSG("GET\n");
            usbresp.data_length = audioGetLevel(sink, control, code,
                                                unit_id, channel);
            usbresp.success = (usbresp.data_length != 0);
        }
        else
        {
            /* Update value sent by USB host */
            L3_DBG_MSG("SET\n");
            usbresp.success = audioSetLevel(req, control, code,
                                            unit_id, channel);
        }

        L3_DBG_MSG1("SUCCESS 0x%X\n", usbresp.success);

        if (usbresp.data_length)
        {
            (void)SinkFlushHeader(sink, usbresp.data_length,
                                  (void *)&usbresp, sizeof(UsbResponse));
        }
        else
        {
            /* Sink packets can never be zero-length, so flush a dummy byte */
            (void)SinkClaim(sink, 1);
            (void)SinkFlushHeader(sink, 1, (void *)&usbresp,
                                  sizeof(UsbResponse));
        }
        /* Discard the original request */
        SourceDrop(req, packet_size);
    }
}

/** Task handler for device class interface requests */
static void audioControlHandler(Task task, MessageId id, Message message)
{
    UNUSED(task);
    UNUSED(message);

    if (id == MESSAGE_MORE_DATA)
    {
        L3_DBG_MSG("USB: MESSAGE_MORE_DATA audio control\n");
        handleAudioControlClassRequest(StreamUsbClassSource(CTRL_INTF));
    }
}

/** Enumerate control interface
 *
 * Add USB interface, enable messages, register task handler and
 * set the Sink task */
static bool usbEnumerateAudioControl(usb_device_class_descriptor descriptor)
{
    UsbInterface intf;

    /* Add an Audio Control Interface */
    CTRL_INTF = usb_interface_error;

    intf = (UsbInterface) UsbAddInterface(&usb_codes_ac,
                                          B_DESCRIPTOR_TYPE_CS_INTERFACE,
                                          descriptor.descriptor,
                                          descriptor.size_descriptor);

    if (intf == usb_interface_error)
    {
        return FALSE;
    }

    CTRL_TASK.handler = audioControlHandler;
    (void)MessageStreamTaskFromSink(StreamUsbClassSink(intf), &CTRL_TASK);

#ifdef HOSTIO_USB_VM_STUB
    (void)SourceConfigure(StreamUsbClassSource(intf),
                          VM_SOURCE_MESSAGES, VM_MESSAGES_ALL);
    (void)SinkConfigure(StreamUsbClassSink(intf),
                        VM_SINK_MESSAGES, VM_MESSAGES_ALL);
#endif

    CTRL_INTF = intf;

    return TRUE;
}

/** Save user-supplied descriptors */
bool usbConfigureAudioDescriptors(usb_device_class_config config,
                                  const usb_device_class_audio_config* params)
{
    /* Sanity check request */
    if(!params ||
            (config != USB_DEVICE_CLASS_CONFIG_AUDIO_INTERFACE_DESCRIPTORS))
    {
        return FALSE;
    }

    /* Update audio config */
    usb_audio_config = params;
    return TRUE;
}

/** Save user-supplied volumes */
bool usbConfigureAudioVolume(usb_device_class_config config,
                             const usb_device_class_audio_volume_config* params)
{
   
    /* Sanity check request */
    if(!params || (config != USB_DEVICE_CLASS_CONFIG_AUDIO_VOLUMES))
    {
        return FALSE;
    }
    
    /* Update audio config */
    memcpy(&volume_user, params, sizeof(volume_user));
    volume = &volume_user;
  
    return TRUE;
}

/** Enumerate USB audio class device */
bool usbEnumerateAudio(uint16 usb_device_class)
{
    if(!usb_audio_config)
    {
        if ((usb_device_class & USB_DEVICE_CLASS_TYPE_AUDIO_MICROPHONE) &&
                (usb_device_class & USB_DEVICE_CLASS_TYPE_AUDIO_SPEAKER))
        {
            usb_audio_config = &usb_audio_config_mic_and_speaker;
        }
        else if (usb_device_class & USB_DEVICE_CLASS_TYPE_AUDIO_MICROPHONE)
        {
            usb_audio_config = &usb_audio_config_mic;
        }
        else if (usb_device_class & USB_DEVICE_CLASS_TYPE_AUDIO_SPEAKER)
        {
            usb_audio_config = &usb_audio_config_speaker;
        }
        else
        {
            assert(0);
        }
    }

    /* Attempt to register control interface if present */
    if(usb_audio_config->control.descriptor &&
            !usbEnumerateAudioControl(usb_audio_config->control))
    {
        return FALSE;
    }
    /* Attempt to register mic interface if present */
    if(usb_audio_config->streaming_mic.descriptor &&
            !usbEnumerateAudioMic(usb_audio_config->streaming_mic))
    {
        return FALSE;
    }
    /* Attempt to register speaker interface if present */
    if(usb_audio_config->streaming_speaker.descriptor &&
            !usbEnumerateAudioSpeaker(usb_audio_config->streaming_speaker))
    {
        return FALSE;
    }
    
    (void)StreamConfigure(VM_STREAM_USB_ALT_IF_MSG_ENABLED, 1);
    
    if (!usb_device_class_is_audio_volume_default(volume))
    {
        /* store that volumes are user defined */
        device->audio.user_audio_levels = TRUE;
    }
    
    return TRUE;
}

/** Return Sink for the mic */
Sink usbAudioMicSink(void)
{
    return StreamUsbEndPointSink(end_point_iso_out);
}

/** Return Source for the speaker */
Source usbAudioSpeakerSource(void)
{
    return StreamUsbEndPointSource(end_point_iso_in);
}

/** Return current levels */
void usbAudioGetLevels(uint16* levels)
{
    /* Copy global structure into response */
    usb_device_class_audio_levels* audio_levels;
    audio_levels = (usb_device_class_audio_levels*)levels;
    *audio_levels = device->audio;
    
    if (device->audio.user_audio_levels)
    {
        /* don't do any level conversion if user defined levels */       
    }
    else
    {
        /* Convert from dB to f/w setting */
        audio_levels->out_l_vol = VOL_SETTING(audio_levels->out_l_vol);
        audio_levels->out_r_vol = VOL_SETTING(audio_levels->out_r_vol);
    }
}

/** Return mic and speaker interface IDs */
UsbInterface usbAudioGetInterfaceId(usb_device_class_get_value interface)
{
    if(interface == USB_DEVICE_CLASS_GET_MIC_INTERFACE_ID)
    {
        if(device->usb_classes & USB_DEVICE_CLASS_TYPE_AUDIO_MICROPHONE)
            return device->usb_interface[usb_interface_audio_streaming_mic];
        else
            return usb_interface_error;
    }
    else
    {
        if(device->usb_classes & USB_DEVICE_CLASS_TYPE_AUDIO_SPEAKER)
            return device->usb_interface[usb_interface_audio_streaming_speaker];
        else
            return usb_interface_error;
    }
}

/** Access function to allow VM code to read sample frequency from lib */
uint32 usbAudioGetSpeakerSampleFreq(void)
{
    return cur_speaker_sample_freq;
}

/** Access function to allow VM code to read sample frequency from lib */
uint32 usbAudioGetMicSampleFreq(void)
{
    return cur_mic_sample_freq;
}

#else /* !USB_DEVICE_CLASS_REMOVE_AUDIO */
    static const int usb_device_class_audio_unused;
#endif /* USB_DEVICE_CLASS_REMOVE_AUDIO */
    
