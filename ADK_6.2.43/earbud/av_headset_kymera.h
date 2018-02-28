/*!
\copyright  Copyright (c) 2017 Qualcomm Technologies International, Ltd.\n
            All Rights Reserved.\n
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    Part of 6.2
\file       av_headset_kymera.h
\brief      Header file for the Kymera Manager

*/

#ifndef AV_HEADSET_KYMERA_H
#define AV_HEADSET_KYMERA_H

#include <chain.h>
#include <transform.h>

#include "av_headset.h"

/*! \brief The kymera module states. */
typedef enum app_kymera_states
{
    /*! Kymera is idle. */
    KYMERA_STATE_IDLE,
    /*! Kymera is streaming A2DP locally. */
    KYMERA_STATE_A2DP_STREAMING,
    /*! Kymera is streaming A2DP locally and forwarding to the slave. */
    KYMERA_STATE_A2DP_STREAMING_WITH_FORWARDING,
    /*! Kymera is streaming SCO locally. */
    KYMERA_STATE_SCO_ACTIVE,
    /*! Kymera is playing a tone. */
    KYMERA_STATE_TONE_PLAYING,
} appKymeraState;


/*! \brief Kymera instance structure.

    This structure contains all the information for Kymera audio chains.
*/
typedef struct
{
    /*! The kymera module's task. */
    TaskData          task;
    /*! The current state. */
    appKymeraState    state;

    /*! The input chain is used in TWS master and slave roles for A2DP streaming
        and is typified by containing a decoder. */
    kymera_chain_handle_t chain_input_handle;
    /*! The tone chain is used when a tone is played. */
    kymera_chain_handle_t chain_tone_handle;
    /*! The volume/output chain is used in TWS master and slave roles for A2DP
        streaming. */
    kymera_chain_handle_t chain_output_vol_handle;
    /*! The SCO chain is used for SCO audio. */
    kymera_chain_handle_t chain_sco_handle;

    /*! The TWS master packetiser transform instance. The transform packs
        compressed audio frames (e.g. SBC, aptX) from the audio subsystem into
        TWS packets for transmission over the air to the TWS slave. */
    Transform packetiser_master;
    /*! The TWS slave packetiser transform instance. The transform receives TWS
        packets over the air from the TWS master. It unpacks compressed audio
        frames (e.g. SBC, aptX) and writes them to the audio subsystem. */
    Transform packetiser_slave;

    /*! The current output sample rate. */
    uint32 output_rate;
    /*! A lock set when a tone is playing. Internal messages are typically sent
        conditionally this lock meaning events are queued until the tone is
        complete. */
    uint16 wait_for_tone;
    /*! The current A2DP stream endpoint identifier. */
    uint8  a2dp_seid;

} kymeraTaskData;

/*! \brief Internal message IDs */
enum app_kymera_internal_message_ids
{
    /*! Internal A2DP start message. */
    KYMERA_INTERNAL_A2DP_START,
    /*! Internal A2DP start forwarding message. */
    KYMERA_INTERNAL_A2DP_START_FORWARDING,
    /*! Internal A2DP stop message. */
    KYMERA_INTERNAL_A2DP_STOP,
    /*! Internal A2DP set volume message. */
    KYMERA_INTERNAL_A2DP_SET_VOL,
    /*! Internal SCO start message. */
    KYMERA_INTERNAL_SCO_START,
    /*! Internal SCO set volume message. */
    KYMERA_INTERNAL_SCO_SET_VOL,
    /*! Internal SCO stop message. */
    KYMERA_INTERNAL_SCO_STOP,
    /*! Internal SCO microphone mute message. */
    KYMERA_INTERNAL_SCO_MIC_MUTE,
    /*! Internal tone play message. */
    KYMERA_INTERNAL_TONE_PLAY,
};

/*! \brief The #KYMERA_INTERNAL_A2DP_START message content. */
typedef struct
{
    /*! The A2DP identifier of the remote device. */
    uint8 device_id;
    /*! The A2DP identifier of the media stream/channel. */
    uint8 stream_id;
    /*! The A2DP identifier of the local stream endpoint. */
    uint8 seid;
    /*! The A2DP media sink (source of media). */
    Sink media_sink;
} KYMERA_INTERNAL_A2DP_START_T;

/*! \brief The #KYMERA_INTERNAL_A2DP_START_FORWARDING message content. */
typedef struct
{
    /*! The A2DP identifier of the remote device. */
    uint8 device_id;
    /*! The A2DP identifier of the media stream/channel. */
    uint8 stream_id;
    /*! The A2DP identifier of the local stream endpoint. */
    uint8 seid;
    /*! The A2DP media sink. */
    Sink forwarding_sink;
} KYMERA_INTERNAL_A2DP_START_FORWARDING_T;

/*! \brief The #KYMERA_INTERNAL_A2DP_SET_VOL message content. */
typedef struct
{
    /*! The volume to set. */
    uint16 volume;
} KYMERA_INTERNAL_A2DP_SET_VOL_T;

/*! \brief The #KYMERA_INTERNAL_A2DP_STOP message content. */
typedef struct
{
    /*! The source of media to stop streaming. */
    Sink media_sink;
} KYMERA_INTERNAL_A2DP_STOP_T;

/*! \brief The #KYMERA_INTERNAL_SCO_START message content. */
typedef struct
{
    /*! The SCO audio sink. */
    Sink audio_sink;
    /*! WB-Speech codec bit masks. */
    hfp_wbs_codec_mask codec;
    /*! The link Wesco. */
    uint8 wesco;
} KYMERA_INTERNAL_SCO_START_T;

/*! \brief The #KYMERA_INTERNAL_SCO_SET_VOL message content. */
typedef struct
{
    /*! The volume to set. */
    uint8 volume;
} KYMERA_INTERNAL_SCO_SET_VOL_T;

/*! \brief The #KYMERA_INTERNAL_SCO_MIC_MUTE message content. */
typedef struct
{
    /*! TRUE to enable mute, FALSE to disable mute. */
    bool mute;
} KYMERA_INTERNAL_SCO_MIC_MUTE_T;

/*! \brief #KYMERA_INTERNAL_TONE_PLAY message content */
typedef struct
{
    /*! Pointer to he ringtone structure to play. */
    const ringtone_note *tone;
    /*! If TRUE, the tone may be interrupted by another event before it is
        completed. If FALSE, the tone may not be interrupted by another event
        and will play to completion. */
    bool interruptible;
} KYMERA_INTERNAL_TONE_PLAY_T;

/*! \brief Start streaming A2DP audio.
    \param device_id The A2DP identifier of the remote device.
    \param stream_id The A2DP identifier of the media stream/channel.
    \param seid The A2DP identifier of the local stream endpoint.
    \param media_sink The A2DP media sink (source of media).
    \return TRUE on success, otherwise FALSE.
*/
bool appKymeraA2dpStart(uint8 device_id, uint8 stream_id, uint8 seid, Sink media_sink);

/*! \brief Stop streaming A2DP audio.
    \param media_sink The A2DP media sink (source of media).
    \return TRUE on success, otherwise FALSE.
*/
bool appKymeraA2dpStop(Sink media_sink);

/*! \brief Set the A2DP streaming volume.
    \param volume The desired volume in the range 0 (mute) to 127 (max).
*/
void appKymeraA2dpSetVolume(uint16 volume);

/*! \brief Start forwarding A2DP audio to a TWS slave.
    \param device_id The A2DP identifier of the remote device.
    \param stream_id The A2DP identifier of the media stream/channel.
    \param seid The A2DP identifier of the local stream endpoint.
    \param forwarding_sink The A2DP forwarding sink.
    \return TRUE on success, otherwise FALSE.
*/
bool appKymeraA2dpStartForwarding(uint8 device_id, uint8 stream_id, uint8 seid, Sink forwarding_sink);

/*! \brief Stop forwarding A2DP audio to a TWS slave.
    \return TRUE on success, otherwise FALSE.
*/
bool appKymeraA2dpStopForwarding(void);

/*! \brief Start SCO audio.
    \param audio_sink The SCO audio sink.
    \param codec WB-Speech codec bit masks.
    \param wesco The link Wesco.
*/
void appKymeraScoStart(Sink audio_sink, hfp_wbs_codec_mask codec, uint8 wesco);

/*! \brief Stop SCO audio.
*/
void appKymeraScoStop(void);

/*! \brief Set SCO volume.
    \param volume [IN] HFP volume in the range 0 (mute) to 15 (max).
 */
void appKymeraScoSetVolume(uint8 volume);

/*! \brief Enable or disable MIC muting.
    \param mute [IN] TRUE to mute MIC, FALSE to unmute MIC.
 */
void appKymeraScoMicMute(bool mute);

/*! \brief Play a tone.
    \param tone The address of the tone to play.
    \param interruptible If TRUE, the tone may be interrupted by another event
           before it is completed. If FALSE, the tone may not be interrupted by
           another event and will play to completion.
*/
void appKymeraTonePlay(const ringtone_note *tone, bool interruptible);

/*! \brief Initialise the kymera module. */
void appKymeraInit(void);

#endif // AV_HEADSET_KYMERA_H
