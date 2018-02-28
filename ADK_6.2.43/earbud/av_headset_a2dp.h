/*!
\copyright  Copyright (c) 2008 - 2017 Qualcomm Technologies International, Ltd.\n
            All Rights Reserved.\n
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    Part of 6.2
\file       av_headset_a2dp.h
\brief      Header file for A2DP State Machine
*/

#ifndef _AV_HEADSET_A2DP_H_
#define _AV_HEADSET_A2DP_H_

#include <a2dp.h>
#include "av_headset.h"

struct appDeviceAttributes;



/*! \brief Check if SEID is for non-TWS CODEC */
#define appA2dpIsSeidNonTwsSink(seid) \
    (((seid) >= AV_SEID_SBC_SNK) && \
     ((seid) <= AV_SEID_APTX_SNK))

/*! \brief Check if SEID is for TWS Sink CODEC */
#define appA2dpIsSeidTwsSink(seid) \
    (((seid) >= AV_SEID_APTX_MONO_TWS_SNK) && \
     ((seid) <= AV_SEID_SBC_MONO_TWS_SNK))

/*! \brief Check if SEID is for TWS Source CODEC */
#define appA2dpIsSeidTwsSource(seid) \
    (((seid) >= AV_SEID_APTX_MONO_TWS_SRC) && \
     ((seid) <= AV_SEID_SBC_MONO_TWS_SRC))
     
/*! \brief Check if SEID is for TWS CODEC */
#define appA2dpIsSeidTws(seid) \
    (((seid) >= AV_SEID_APTX_MONO_TWS_SNK) && \
     ((seid) <= AV_SEID_SBC_MONO_TWS_SRC))

/*! \brief Check if SEID is for Sink */
#define appA2dpIsSeidSink(seid) \
    (((seid) >= AV_SEID_SBC_SNK) && \
     ((seid) <= AV_SEID_SBC_MONO_TWS_SNK))

/*! \brief Check if SEID is for Source */
#define appA2dpIsSeidSource(seid) \
    (((seid) >= AV_SEID_APTX_MONO_TWS_SRC) && \
     ((seid) <= AV_SEID_SBC_MONO_TWS_SRC))

/*! \brief Check this instance is a non-TWS Sink */
#define appA2dpIsSinkNonTwsCodec(theInst) \
    (appA2dpIsSeidNonTwsSink(theInst->a2dp_current_seid))

/*! \brief Check this instance is a TWS Sink */
#define appA2dpIsSinkTwsCodec(theInst) \
    (appA2dpIsSeidTwsSink(theInst->a2dp_current_seid))

/*! \brief Check this instance is a Sink */
#define appA2dpIsSinkCodec(theInst) \
    (appA2dpIsSeidSink(theInst->a2dp_current_seid))

/*! \brief Check this instance is a Source */
#define appA2dpIsSourceCodec(theInst) \
    (appA2dpIsSeidSource(theInst->a2dp_current_seid))

/*! \brief Check this instance is a TWS Source or Sink */
#define appA2dpIsTwsCodec(theInst) \
    (appA2dpIsSeidTws(theInst->a2dp_current_seid))
    
/*! \brief Get A2DP lock */
#define appA2dpGetLock(theInst) ((theInst)->a2dp_lock)

/*! \brief Set A2DP lock */
#define appA2dpSetLock(theInst, lock) \
    do { \
        (theInst)->a2dp_lock = (lock); \
    } while (0)
//        AV_PRINT(("appA2dpSetLock %p %d\n", theInst, lock)); \
//    } while (0)


/*!@{ \name Masks to use to check for the current state meeting some conditions */
#define A2DP_STATE_MASK_CONNECTED_SIGNALLING            (A2DP_STATE_CONNECTED_SIGNALLING)
#define A2DP_STATE_MASK_CONNECTED_MEDIA                 (A2DP_STATE_CONNECTED_MEDIA)
#define A2DP_STATE_MASK_CONNECTED_MEDIA_STREAMING       (A2DP_STATE_CONNECTED_MEDIA | 0x0F)
#define A2DP_STATE_MASK_CONNECTED_MEDIA_STARTING_LOCAL  (A2DP_STATE_CONNECTED_MEDIA_STARTING_LOCAL)
/*!@} */


/*! \brief Is A2DP state 'connected signalling' */
#define appA2dpIsStateConnectedSignalling(a2dp_state) \
    (((a2dp_state) & A2DP_STATE_MASK_CONNECTED_SIGNALLING) == A2DP_STATE_CONNECTED_SIGNALLING)
	  
/*! \brief Is A2DP state 'connected media' */
#define appA2dpIsStateConnectedMedia(a2dp_state) \
    (((a2dp_state) & A2DP_STATE_MASK_CONNECTED_MEDIA) == A2DP_STATE_CONNECTED_MEDIA)

/*! \brief Is A2DP state 'connected media streaming' */
#define appA2dpIsStateConnectedMediaStreaming(a2dp_state) \
    ((a2dp_state) == A2DP_STATE_CONNECTED_MEDIA_STREAMING)

/*! \brief Is A2DP state 'connected media starting local' */
#define appA2dpIsStateConnectedMediaStartingLocal(a2dp_state) \
    (((a2dp_state) & A2DP_STATE_MASK_CONNECTED_MEDIA_STARTING_LOCAL) == A2DP_STATE_CONNECTED_MEDIA_STARTING_LOCAL)


/*! \brief Is A2DP instance media channel connected */
#define appA2dpIsConnectedMedia(theInst) \
    appA2dpIsStateConnectedMedia((theInst)->a2dp_state)

/*! \brief Is A2DP instance streaming */
#define appA2dpIsStreaming(theInst) \
    appA2dpIsStateConnectedMediaStreaming((theInst)->a2dp_state)

/*! \brief Is A2DP instance disconnected */
#define appA2dpIsDisconnected(theInst) \
    ((theInst)->a2dp_state == A2DP_STATE_DISCONNECTED)

/*! \brief Quick check for whether a2dp is connected on the specified AV instance */
#define appA2dpIsConnected(theInst) \
    (((theInst)->a2dp_state & A2DP_STATE_MASK_CONNECTED_SIGNALLING) == A2DP_STATE_CONNECTED_SIGNALLING)

extern void appA2dpInstanceInit(avInstanceTaskData *theAv, uint8 suspend_state);
extern void appA2dpVolumeSet(avInstanceTaskData *theAv, uint16 volume);
extern void appA2dpSetDefaultAttributes(struct appDeviceAttributes *attributes);
extern avA2dpState appA2dpGetState(avInstanceTaskData *theAv);
extern void appA2dpInstanceHandleMessage(avInstanceTaskData *theInst, MessageId id, Message message);

#endif
