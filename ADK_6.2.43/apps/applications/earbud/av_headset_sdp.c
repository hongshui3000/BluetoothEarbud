/*!
\copyright  Copyright (c) 2005 - 2017 Qualcomm Technologies International, Ltd.\n
            All Rights Reserved.\n
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    Part of 6.2
\file       av_headset_sdp.c
\brief	    SDP definitions and help functions
*/

#include <region.h>
#include <service.h>
#include <bdaddr.h>
#include <panic.h>

#include "av_headset.h"
#include "av_headset_sdp.h"

/*! Offset of the peer Bluetooth address in a sink service record */
#define PEER_BDADDR_OFFSET (55)

static const uint8 tws_sink_service_record[] =
{
   /* Offset */ /* ServiceClassIDList(0x0001), Data Element Sequence */
    /*  0 */    SDP_ATTR_ID(UUID_SERVICE_CLASS_ID_LIST),
    /*  3 */        SDP_DATA_EL_SEQ(17),

                    /*  UUID Qualcomm TWS+ Sink (760055b5-e4eb-4933-ae68-7569043e309d) */
    /*  5 */        SDP_DATA_EL_UUID128(0x76, 0x00, 0x55, 0xb5, 0xe4, 0xeb, 0x49, 0x33, 0xae, 0x68, 0x75, 0x69, 0x04, 0x3e, 0x30, 0x9d),

                /*  BluetoothProfileDescriptorList (0x0009) */
    /* 22 */    SDP_ATTR_ID(UUID_BT_PROFILE_DESCRIPTOR_LIST),
    /* 25 */    SDP_DATA_EL_SEQ(22),
    /* 27 */        SDP_DATA_EL_SEQ(20),

                        /* UUID Qualcomm TWS+ Profile (ca9a00f7-677f-4435-9827-785a2589c368) */
    /* 29 */            SDP_DATA_EL_UUID128(0xca,0x9a,0x00,0xf7,0x67,0x7f,0x44,0x35,0x98,0x27,0x78,0x5a,0x25,0x89,0xc3,0x68),

                        /* Version */
    /* 46 */            SDP_DATA_EL_UINT16(DEVICE_TWS_VERSION),

                /*  Bluetooth Address(0x0200) */
    /* 49 */    SDP_ATTR_ID(UUID_PEER_BDADDR),
    /* 52 */    SDP_DATA_EL_SEQ(10),

                    /* NAP */
    /* 54 */        SDP_DATA_EL_UINT16(0xA5A5),

                    /* UAP */
    /* 57 */        SDP_DATA_EL_UINT8(0x00),

                    /* LAP */
    /* 59 */        SDP_DATA_EL_UINT32(0x000000),

};

const uint8 *appSdpGetTwsSinkServiceRecord(void)
{
    return tws_sink_service_record;
}

uint16 appSdpGetTwsSinkServiceRecordSize(void)
{
    return sizeof(tws_sink_service_record);
}


static const uint8 tws_sink_service_search_request[] =
{
    SDP_DATA_EL_SEQ(17),                     /* type = DataElSeq, 17 bytes in DataElSeq */
        SDP_DATA_EL_UUID128(0x76, 0x00, 0x55, 0xb5, 0xe4, 0xeb, 0x49, 0x33, 0xae, 0x68, 0x75, 0x69, 0x04, 0x3e, 0x30, 0x9d)
};

const uint8 *appSdpGetTwsSinkServiceSearchRequest(void)
{
    return tws_sink_service_search_request;
}

uint16 appSdpGetTwsSinkServiceSearchRequestSize(void)
{
    return sizeof(tws_sink_service_search_request);
}

/* TWS+ Sink attribute search request */
static const uint8 tws_sink_attribute_list[] =
{
     SDP_DATA_EL_SEQ(3),                               /* Data Element Sequence of 3 */
        SDP_ATTR_ID(UUID_BT_PROFILE_DESCRIPTOR_LIST),  /* Current Version Attribute ID */
};

const uint8 *appSdpGetTwsSinkAttributeSearchRequest(void)
{
    return tws_sink_attribute_list;
}

uint16 appSdpGetTwsSinkAttributeSearchRequestSize(void)
{
    return sizeof(tws_sink_attribute_list);
}



/* TWS+ Source UUID search request */
static const uint8 tws_source_service_search_request[] =
{
    SDP_DATA_EL_SEQ(17),                     /* type = DataElSeq, 17 bytes in DataElSeq */
        0x1C, 0x49, 0x30, 0x5a, 0x31, 0xa8, 0xce, 0x45, 0xa3, 0x8c, 0x88, 0x36, 0x57, 0xf1, 0xda, 0xd3, 0xff
};

const uint8 *appSdpGetTwsSourceServiceSearchRequest(void)
{
    return tws_source_service_search_request;
}

uint16 appSdpGetTwsSourceServiceSearchRequestSize(void)
{
    return sizeof(tws_source_service_search_request);
}



/* TWS+ Source attribute search request */
static const uint8 tws_source_attribute_list[] =
{
    SDP_DATA_EL_SEQ(3),                                /* Data Element Sequence of 3 */
        SDP_ATTR_ID(UUID_BT_PROFILE_DESCRIPTOR_LIST),  /* Current Version Attribute ID */
};

const uint8 *appSdpGetTwsSourceAttributeSearchRequest(void)
{
    return tws_source_attribute_list;
}

uint16 appSdpGetTwsSourceAttributeSearchRequestSize(void)
{
    return sizeof(tws_source_attribute_list);
}



bool appSdpFindTwsVersion(const uint8 *ptr, const uint8 *end, uint16 *tws_version)
{
    static const uint8 tws_profile_uuid[] = {0xca,0x9a,0x00,0xf7,0x67,0x7f,0x44,0x35,0x98,0x27,0x78,0x5a,0x25,0x89,0xc3,0x68};
    ServiceDataType type;
    Region record, protocols, protocol;
    Region value;

    PanicNull(tws_version);

    record.begin = ptr;
    record.end   = end;

    while (ServiceFindAttribute(&record, UUID_BT_PROFILE_DESCRIPTOR_LIST, &type, &protocols))
    {
        if(type == sdtSequence)
        {
            while (ServiceGetValue(&protocols, &type, &protocol))
            {
                if (type == sdtSequence)
                {
                    if (ServiceGetValue(&protocol, &type, &value))
                    {
                        if (type == sdtUUID)
                        {
                            if (RegionMatchesUUID128(&value, tws_profile_uuid))
                            {
                                if (ServiceGetValue(&protocol, &type, &value))
                                {
                                    if (type == sdtUnsignedInteger)
                                    {
                                        *tws_version = (uint16)RegionReadUnsigned(&value);
                                        return TRUE;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return FALSE; /* Failed */
}


void appSdpSetTwsSinkServiceRecordPeerBdAddr(uint8 *record, const bdaddr *bd_addr)
{
    record[PEER_BDADDR_OFFSET + 0] = (bd_addr->nap >> 8) & 0xFF;
    record[PEER_BDADDR_OFFSET + 1] = (bd_addr->nap >> 0) & 0xFF;
    record[PEER_BDADDR_OFFSET + 3] = (bd_addr->uap) & 0xFF;
    record[PEER_BDADDR_OFFSET + 5] = 0x00;
    record[PEER_BDADDR_OFFSET + 6] = (bd_addr->lap >> 16) & 0xFF;
    record[PEER_BDADDR_OFFSET + 7] = (bd_addr->lap >> 8) & 0xFF;
    record[PEER_BDADDR_OFFSET + 8] = (bd_addr->lap >> 0) & 0xFF;
}
