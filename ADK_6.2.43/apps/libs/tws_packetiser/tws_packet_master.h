/****************************************************************************
Copyright (c) 2016 Qualcomm Technologies International, Ltd.
Part of 6.2

FILE NAME
    tws_packet_master.h

DESCRIPTION
    The tws packet master's state.
*/

#ifndef TWS_PACKET_MASTER_H_
#define TWS_PACKET_MASTER_H_

/*! TWS master packet state */
typedef struct __tws_packet_master
{
    /*! Write ptr in buffer. */
    uint8 *ptr;

    /*! Length of the buffer. */
    uint32 len;

    /*! Points to start of buffer. */
    uint8 *buffer;

    /*! Number of frames written to the packet */
    uint32 frames;

    /*! One frame per packet */
    bool one_frame_per_packet;

    /*! Store the current read offset into the source data buffer. 
        Used to allow audio frames to be transmitted in fragments in multiple packets. */
    uint32 src_read_offset;

    /*! Latched TRUE when a fragment of a audio frame is written to a packet */
    bool has_fragment_latched;

    /*! Fragmentation is only allowed when configured for particular codecs */
    bool fragmentation_allowed;

} tws_packet_master_t;

#endif
