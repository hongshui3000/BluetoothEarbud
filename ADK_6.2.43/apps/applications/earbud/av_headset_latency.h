/*!
\copyright  Copyright (c) 2017 Qualcomm Technologies International, Ltd.\n
            All Rights Reserved.\n
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    Part of 6.2
\file       av_headset_latency.h
\brief      Definitions of latency.

For TWS standard, latencies are measured from arrival of audio in the
audio subsystem to the synchronised rendering by TWS master and TWS
slave.
*/

#ifndef AV_HEADSET_LATENCY_H
#define AV_HEADSET_LATENCY_H

#include <rtime.h>
#include <hydra_macros.h>

/*! The TWS standard target latency in milli-seconds. */
#define TWS_STANDARD_LATENCY_MS 300
/*! The TWS standard target latency in micro-seconds. */
#define TWS_STANDARD_LATENCY_US (US_PER_MS * TWS_STANDARD_LATENCY_MS)

/*! Buffering in the TWS standard graph is split between the (pre) source sync and
 *  pre-decoder buffers. The buffer after source sync is treated as "headroom",
 *  since it is comparatively small (4 * kick period). So for SBC/AAC, the entire
 *  TTP latency is split over these two buffers. AptX will have some extra buffer
 *  space since the decoding is split across two operators - there will be an
 *  extra buffer between the demux operator and aptX mono decoder. */
#define PCM_LATENCY_BUFFER_MS (250)
/*! The difference between the total latency and the PCM buffer latency is the
    amount of buffering required pre-decoder. */
#define PRE_DECODER_BUFFER_MS (TWS_STANDARD_LATENCY_MS - PCM_LATENCY_BUFFER_MS)

#endif // AV_HEADSET_LATENCY_H
