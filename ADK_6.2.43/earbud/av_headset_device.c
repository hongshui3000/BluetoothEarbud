/*!
\copyright  Copyright (c) 2015 - 2017 Qualcomm Technologies International, Ltd.
            All Rights Reserved.
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    Part of 6.2
\file       av_headset_device.c
\brief	    Device Management.
*/

#include <panic.h>
#include <connection.h>
#include <ps.h>
#include <string.h>
#include <region.h>
#include <service.h>

#include "av_headset.h"
#include "av_headset_log.h"
#include "av_headset_con_manager.h"


/*! \brief Macro for simplifying creating messages */
#define MAKE_DEVICE_MESSAGE(TYPE) \
    TYPE##_T *message = PanicUnlessNew(TYPE##_T);
/*! \brief Macro for simplying copying message content */
#define COPY_DEVICE_MESSAGE(src, dst) *(dst) = *(src);


/*! \brief Update the RAM cache of a device attributes. */
static void appDeviceUpdateCache(deviceTaskData *theDevice, appDeviceAttributes *attributes, const bdaddr *bd_addr)
{
    if (attributes->type == DEVICE_TYPE_EARBUD && !BdaddrIsZero(bd_addr))
    {
        DEBUG_LOGF("appDeviceUpdateCache, update peer, bdaddr %04x,%02x,%06lx, version %u.%02u",
                     bd_addr->nap, bd_addr->uap, bd_addr->lap,
                     attributes->tws_version >> 8, attributes->tws_version & 0xFF);
        theDevice->peer_bd_addr = *bd_addr;
        theDevice->peer_tws_version = attributes->tws_version;
        theDevice->peer_connected = appConManagerIsConnected(bd_addr);
    }
    else if (attributes->type == DEVICE_TYPE_HANDSET && !BdaddrIsZero(bd_addr))
    {
        DEBUG_LOGF("appDeviceUpdateCache, update handset, bdaddr %04x,%02x,%06lx, version %u.%02u",
                     bd_addr->nap, bd_addr->uap, bd_addr->lap,
                     attributes->tws_version >> 8, attributes->tws_version & 0xFF);
        theDevice->handset_bd_addr = *bd_addr;
        theDevice->handset_tws_version = attributes->tws_version;
        theDevice->handset_connected = appConManagerIsConnected(bd_addr);
    }
}

bool appDeviceFindBdAddrAttributes(const bdaddr *bd_addr, appDeviceAttributes *attributes)
{
    return ConnectionSmGetAttributeNowReq(0, TYPED_BDADDR_PUBLIC, bd_addr, attributes ? sizeof(*attributes) : 0, (uint8 *)attributes);
}

/*! @brief Get attributes for device type starting search at specified index.
 */
static bool appDeviceGetAttributes(bdaddr *bd_addr, deviceType type, appDeviceAttributes *attributes,
                                   int *index)
{
    typed_bdaddr taddr;
    int iter;

    /* NULL attributes pointer not allowed, as we need to read attributes to get device type */
    PanicNull(attributes);

    /* setup iterator start value, permit caller to specify NULL and just get first
     * instance found */
    if (index == NULL)
        iter = 0;
    else
        iter = *index;

    for (; iter < appConfigMaxPairedDevices(); iter++)
    {
        if (ConnectionSmGetIndexedAttributeNowReq(0, iter,
                                                  sizeof(*attributes), (uint8 *)attributes,
                                                  &taddr))
        {
            if (attributes->type == type)
            {
                if (bd_addr)
                    *bd_addr = taddr.addr;
                if (index)
                    *index = iter + 1;
                return TRUE;
            }
        }
    }

    return FALSE;
}

bool appDeviceGetPeerAttributes(bdaddr *bd_addr, appDeviceAttributes *attributes)
{
    return appDeviceGetAttributes(bd_addr, DEVICE_TYPE_EARBUD, attributes, NULL);
}

bool appDeviceGetPeerBdAddr(bdaddr *bd_addr)
{
    appDeviceAttributes attributes;
    return appDeviceGetAttributes(bd_addr, DEVICE_TYPE_EARBUD, &attributes, NULL);
}

bool appDeviceGetHandsetAttributes(bdaddr *bd_addr, appDeviceAttributes *attributes,
                                   int *index)
{
    return appDeviceGetAttributes(bd_addr, DEVICE_TYPE_HANDSET, attributes, index);
}

bool appDeviceGetHandsetBdAddr(bdaddr *bd_addr)
{
    appDeviceAttributes attributes;
    return appDeviceGetAttributes(bd_addr, DEVICE_TYPE_HANDSET, &attributes, NULL);
}

bool appDeviceDelete(const bdaddr *bd_addr)
{
    if (!appConManagerIsConnected(bd_addr))
    {
        deviceTaskData *theDevice = appGetDevice();

        ConnectionAuthSetPriorityDevice(bd_addr, FALSE);
        ConnectionSmDeleteAuthDevice(bd_addr);

        if (BdaddrIsSame(&theDevice->handset_bd_addr, bd_addr))
            BdaddrSetZero(&theDevice->handset_bd_addr);

        if (BdaddrIsSame(&theDevice->peer_bd_addr, bd_addr))
            BdaddrSetZero(&theDevice->peer_bd_addr);

        return TRUE;
    }
    else
    {
        DEBUG_LOG("appDeviceDelete, Failed to delete device as connected");
        return FALSE;
    }
}

/*! @brief Update connectivity state for a device. */
static void appDeviceHandleConManagerConnectionInd(CON_MANAGER_CONNECTION_IND_T* ind)
{
    deviceTaskData *theDevice = appGetDevice();

    if (appDeviceIsHandset(&ind->bd_addr))
    {
        DEBUG_LOGF("appDeviceHandleConManagerConnectionInd HANDSET CONN:%d Status:%d",
                                                    ind->connected, ind->reason);
        theDevice->handset_connected = ind->connected;
    }
    else if (appDeviceIsPeer(&ind->bd_addr))
    {
        DEBUG_LOGF("appDeviceHandleConManagerConnectionInd PEER CONN:%d Status:%d",
                                                    ind->connected, ind->reason);
        theDevice->peer_connected = ind->connected;
    }
    else
    {
        DEBUG_LOGF("appDeviceHandleConManagerConnectionInd UNKNOWN %lx CONN:%d Status:%d", 
                                    ind->bd_addr.lap, ind->connected, ind->reason); 
    }

    /* If we've disconnected, clear just paired flag */
    appDeviceAttributes attr;
    if (!ind->connected && appDeviceFindBdAddrAttributes(&ind->bd_addr, &attr))
    {
        if (attr.flags & DEVICE_FLAGS_JUST_PAIRED)
        {
            attr.flags &= ~DEVICE_FLAGS_JUST_PAIRED;
            appDeviceSetAttributes(&ind->bd_addr, &attr);
            DEBUG_LOG("appDeviceHandleConManagerConnectionInd, clearing just paired flag");
        }
    }
}

/*! @brief Peer signalling task message handler.
 */
static void appDeviceHandleMessage(Task task, MessageId id, Message message)
{
    UNUSED(task);

    switch (id)
    {
        case CON_MANAGER_CONNECTION_IND:
            appDeviceHandleConManagerConnectionInd((CON_MANAGER_CONNECTION_IND_T*)message);
            break;
        default:
            break;
    }
}

void appDeviceInit(void)
{
    deviceTaskData *theDevice = appGetDevice();

    DEBUG_LOG("appDeviceInit");

    theDevice->task.handler = appDeviceHandleMessage;

    BdaddrSetZero(&theDevice->handset_bd_addr);
    BdaddrSetZero(&theDevice->peer_bd_addr);

    /* Scan TDL for peer and handset devices */
    for (int index = 0; index < appConfigMaxPairedDevices(); index++)
    {
        appDeviceAttributes attributes;
        typed_bdaddr t_bd_addr;

        /* Retrieve attributes from the specified device index */
        if (ConnectionSmGetIndexedAttributeNowReq(0, index,
                                                  sizeof(attributes), (uint8 *)&attributes,
                                                  &t_bd_addr))
        {
            /* Update cache */
            appDeviceUpdateCache(theDevice, &attributes, &t_bd_addr.addr);
        }
        else
            break;
    }

    /* register to receive notifications of connections */
    appConManagerRegisterConnectionsClient(&theDevice->task);
}

void appDeviceInitAttributes(appDeviceAttributes *attributes)
{
    attributes->a2dp_num_seids = 0;
    attributes->dev_info_version = 1;
    attributes->hfp_profile = 0;
    attributes->supported_profiles = 0;
    attributes->connected_profiles = 0;
    attributes->tws_version = 0;
    attributes->type = DEVICE_TYPE_UNKNOWN;
    attributes->link_mode = DEVICE_LINK_MODE_UNKNOWN;
    attributes->flags = 0x00;
#ifdef INCLUDE_AV
    appA2dpSetDefaultAttributes(attributes);
#endif
#ifdef INCLUDE_HFP
    appHfpSetDefaultAttributes(attributes);
#endif
}

void appDeviceSetAttributes(const bdaddr *bd_addr, appDeviceAttributes *attributes)
{
    deviceTaskData *theDevice = appGetDevice();

    /* Write attributes */
    ConnectionSmPutAttribute(0, bd_addr, sizeof(*attributes), (uint8 *)attributes);

    /* Update cache */
    appDeviceUpdateCache(theDevice, attributes, bd_addr);
}

uint16 appDeviceTwsVersion(const bdaddr *bd_addr)
{
    deviceTaskData *theDevice = appGetDevice();

    if (BdaddrIsSame(bd_addr, &theDevice->handset_bd_addr))
        return theDevice->handset_tws_version;
    else if (BdaddrIsSame(bd_addr, &theDevice->peer_bd_addr))
        return theDevice->peer_tws_version;

    return DEVICE_TYPE_UNKNOWN;
}

void appDeviceSetTwsVersion(const bdaddr *bd_addr, uint16 tws_version)
{
    appDeviceAttributes attributes;
    uint16 events = 0;

    /* Retrieve attributes for device with specified address */
    if (appDeviceFindBdAddrAttributes(bd_addr, &attributes))
    {
        /* if TWS version has changed, set the flag to indicate
         * cope with learning a device now supports TWS+ or that it was TWS+ but
         * has downgraded to standard. */
        if (   (attributes.tws_version == DEVICE_TWS_UNKNOWN && tws_version == DEVICE_TWS_VERSION)
            || (attributes.tws_version == DEVICE_TWS_STANDARD && tws_version == DEVICE_TWS_VERSION))
        {
            DEBUG_LOG("appDeviceSetTwsVersion, setting link key transmit flag");
            attributes.flags |= DEVICE_FLAGS_HANDSET_LINK_KEY_TX_REQD;
            events |= RULE_EVENT_PEER_UPDATE_LINKKEYS;
        }

        if (   (attributes.tws_version == DEVICE_TWS_UNKNOWN && tws_version == DEVICE_TWS_STANDARD)
            || (attributes.tws_version == DEVICE_TWS_VERSION && tws_version == DEVICE_TWS_STANDARD))
        {
            /* Don't set forward required flag  and generate event if the peer has told us to
             * pair with this handset */
            if (~attributes.flags & DEVICE_FLAGS_PRE_PAIRED_HANDSET)
            {
                DEBUG_LOG("appDeviceSetTwsVersion, setting handset address forward flag");
                attributes.flags |= DEVICE_FLAGS_HANDSET_ADDRESS_FORWARD_REQD;
                events |= RULE_EVENT_PEER_UPDATE_LINKKEYS;
            }
        }

        /* Match found, update attributes */
        attributes.tws_version = tws_version;
        appDeviceSetAttributes(bd_addr, &attributes);

        /* Set event if link-key or address needs to be forward to peer */
        if (events)
            appConnRulesSetEvent(appGetSmTask(), events);
    }
}

bool appDeviceIsPeer(const bdaddr *bd_addr)
{
    deviceTaskData *theDevice = appGetDevice();
    appDeviceAttributes attributes;

    /* Check cache first */
    if (BdaddrIsSame(bd_addr, &theDevice->peer_bd_addr))
        return TRUE;

    /* Check cache first */
    if (BdaddrIsSame(bd_addr, &theDevice->handset_bd_addr))
        return FALSE;

    /* Retrieve attributes for device with specified address */
    if (appDeviceFindBdAddrAttributes(bd_addr, &attributes))
    {
        /* Match found, so update cache and return indication device
           is handset or not */
        appDeviceUpdateCache(theDevice, &attributes, bd_addr);
        return attributes.type == DEVICE_TYPE_EARBUD;
    }

    return FALSE;
}

bool appDeviceIsHandset(const bdaddr *bd_addr)
{
    deviceTaskData *theDevice = appGetDevice();
    appDeviceAttributes attributes;

    /* Check cache first */
    if (BdaddrIsSame(bd_addr, &theDevice->handset_bd_addr))
        return TRUE;

    /* Check cache first */
    if (BdaddrIsSame(bd_addr, &theDevice->peer_bd_addr))
        return FALSE;

    /* Retrieve attributes for device with specified address */
    if (appDeviceFindBdAddrAttributes(bd_addr, &attributes))
    {
        /* Match found, so update cache and return indication device
           is handset or not */
        appDeviceUpdateCache(theDevice, &attributes, bd_addr);
        return attributes.type == DEVICE_TYPE_HANDSET;
    }

    return FALSE;
}

bool appDeviceIsA2dpSupported(const bdaddr *bd_addr)
{
    appDeviceAttributes attributes;

    /* Retrieve attributes for device with specified address */
    if (appDeviceFindBdAddrAttributes(bd_addr, &attributes))
        return attributes.supported_profiles & DEVICE_PROFILE_A2DP;
    else
        return FALSE;
}

bool appDeviceIsAvrcpSupported(const bdaddr *bd_addr)
{
    appDeviceAttributes attributes;

    /* Retrieve attributes for device with specified address */
    if (appDeviceFindBdAddrAttributes(bd_addr, &attributes))
        return attributes.supported_profiles & DEVICE_PROFILE_AVRCP;
    else
        return FALSE;
}

bool appDeviceIsHfpSupported(const bdaddr *bd_addr)
{
    appDeviceAttributes attributes;

    /* Retrieve attributes for device with specified address */
    if (appDeviceFindBdAddrAttributes(bd_addr, &attributes))
        return attributes.supported_profiles & DEVICE_PROFILE_HFP;
    else
        return FALSE;
}

bool appDeviceWasConnected(const bdaddr *bd_addr)
{
    appDeviceAttributes attributes;

    /* Retrieve attributes for device with specified address */
    if (appDeviceFindBdAddrAttributes(bd_addr, &attributes))
    {
        DEBUG_LOGF("appDeviceWasConnected, connected %02x", attributes.connected_profiles);
        return attributes.connected_profiles;
    }
    else
        return FALSE;
}

void appDeviceSetHfpWasConnected(const bdaddr *bd_addr, bool connected)
{
    appDeviceAttributes attributes;

    /* Retrieve attributes for device with specified address */
    if (appDeviceFindBdAddrAttributes(bd_addr, &attributes))
    {
        DEBUG_LOGF("appDeviceSetHfpWasConnected, connected %02x", attributes.connected_profiles);
        attributes.connected_profiles &= ~DEVICE_PROFILE_HFP;
        if (connected)
            attributes.connected_profiles |= DEVICE_PROFILE_HFP;
        appDeviceSetAttributes(bd_addr, &attributes);
        DEBUG_LOGF("appDeviceSetHfpWasConnected, connected %02x", attributes.connected_profiles);
    }
}

void appDeviceSetA2dpWasConnected(const bdaddr *bd_addr, bool connected)
{
    appDeviceAttributes attributes;

    /* Retrieve attributes for device with specified address */
    if (appDeviceFindBdAddrAttributes(bd_addr, &attributes))
    {
        DEBUG_LOGF("appDeviceSetA2dpWasConnected, connected %02x", attributes.connected_profiles);
        attributes.connected_profiles &= ~DEVICE_PROFILE_A2DP;
        if (connected)
            attributes.connected_profiles |= DEVICE_PROFILE_A2DP;
        appDeviceSetAttributes(bd_addr, &attributes);
        DEBUG_LOGF("appDeviceSetA2dpWasConnected, connected %02x", attributes.connected_profiles);
    }
}

void appDeviceSetA2dpIsSupported(const bdaddr *bd_addr)
{
    appDeviceAttributes attributes;

    /* Retrieve attributes for device with specified address */
    if (appDeviceFindBdAddrAttributes(bd_addr, &attributes))
    {
        attributes.supported_profiles |= DEVICE_PROFILE_A2DP;
        appDeviceSetAttributes(bd_addr, &attributes);
    }
}

void appDeviceSetAvrcpIsSupported(const bdaddr *bd_addr)
{
    appDeviceAttributes attributes;

    /* Retrieve attributes for device with specified address */
    if (appDeviceFindBdAddrAttributes(bd_addr, &attributes))
    {
        attributes.supported_profiles |= DEVICE_PROFILE_AVRCP;
        appDeviceSetAttributes(bd_addr, &attributes);
    }
}

void appDeviceSetHfpIsSupported(const bdaddr *bd_addr, hfp_profile profile)
{
    appDeviceAttributes attributes;

    /* Retrieve attributes for device with specified address */
    if (appDeviceFindBdAddrAttributes(bd_addr, &attributes))
    {
        attributes.supported_profiles |= DEVICE_PROFILE_HFP;
        attributes.hfp_profile = profile;
        appDeviceSetAttributes(bd_addr, &attributes);
    }
}

void appDeviceSetLinkMode(const bdaddr *bd_addr, deviceLinkMode link_mode)
{
    appDeviceAttributes attributes;

    /* Retrieve attributes for device with specific address */
    if(appDeviceFindBdAddrAttributes(bd_addr, &attributes))
    {
        attributes.link_mode = link_mode;
        appDeviceSetAttributes(bd_addr, &attributes);
    }
}

bool appDeviceIsSecureConnection(const bdaddr *bd_addr)
{
    appDeviceAttributes attributes;

    /* Retrieve attributes for device with specific address */
    if(appDeviceFindBdAddrAttributes(bd_addr, &attributes))
        return (attributes.link_mode == DEVICE_LINK_MODE_SECURE_CONNECTION);

    return FALSE;
}

bool appDeviceSetHandsetLinkKeyTxReqd(bdaddr *handset_bd_addr, bool reqd)
{
    appDeviceAttributes attr;

    if (appDeviceGetHandsetAttributes(handset_bd_addr, &attr, NULL))
    {
        if (reqd)
            attr.flags |= DEVICE_FLAGS_HANDSET_LINK_KEY_TX_REQD;
        else
            attr.flags &= ~DEVICE_FLAGS_HANDSET_LINK_KEY_TX_REQD;

        appDeviceSetAttributes(handset_bd_addr, &attr);

        return TRUE;
    }

    return FALSE;
}

bool appDeviceHandsetAttrIsLinkKeyTxReqd(appDeviceAttributes* attr)
{
    return attr->flags & DEVICE_FLAGS_HANDSET_LINK_KEY_TX_REQD;
}

bool appDeviceGetHandsetLinkKeyTxReqd(bdaddr *handset_bd_addr, bool *reqd)
{
    appDeviceAttributes attr;
    UNUSED(reqd);

    if (appDeviceGetHandsetAttributes(handset_bd_addr, &attr, NULL))
    {
        return appDeviceHandsetAttrIsLinkKeyTxReqd(&attr);
    }

    return FALSE;
}

bool appDeviceIsHandsetConnected(void)
{
    deviceTaskData *theDevice = appGetDevice();
    return theDevice->handset_connected;
}

bool appDeviceIsHandsetA2dpConnected(void)
{
    bdaddr peer_addr;
    if (appDeviceGetHandsetBdAddr(&peer_addr))
    {
        avInstanceTaskData *inst = appAvInstanceFindFromBdAddr(&peer_addr);
        if (inst)
        {
            if (appA2dpIsConnected(inst))
                return TRUE;
        }
    }
    return FALSE;
}

bool appDeviceIsHandsetA2dpStreaming(void)
{
    bdaddr peer_addr;
    if (appDeviceGetHandsetBdAddr(&peer_addr))
    {
        avInstanceTaskData* inst = appAvInstanceFindFromBdAddr(&peer_addr);
        if (inst)
        {
            if (appA2dpIsStreaming(inst))
                return TRUE;
        }
    }
    return FALSE;
}

bool appDeviceIsHandsetAvrcpConnected(void)
{
    bdaddr peer_addr;
    if (appDeviceGetHandsetBdAddr(&peer_addr))
    {
        avInstanceTaskData* inst = appAvInstanceFindFromBdAddr(&peer_addr);
        if (inst)
        {
            if (appAvrcpIsConnected(inst))
                return TRUE;
        }
    }
    return FALSE;
}

bool appDeviceIsHandsetHfpConnected(void)
{
    bdaddr peer_addr;
    if (appDeviceGetHandsetBdAddr(&peer_addr))
        return (appHfpIsConnected() & BdaddrIsSame(&peer_addr, appHfpGetAgBdAddr()));
    else
        return FALSE;
}

bool appDeviceIsPeerConnected(void)
{
    deviceTaskData *theDevice = appGetDevice();
    return theDevice->peer_connected;
}

bool appDeviceIsPeerA2dpConnected(void)
{
    bdaddr peer_addr;
    if (appDeviceGetPeerBdAddr(&peer_addr))
    {
        avInstanceTaskData* inst = appAvInstanceFindFromBdAddr(&peer_addr);
        if (inst)
        {
            if (!appA2dpIsDisconnected(inst))
                return TRUE;
        }
    }
    return FALSE;
}

bool appDeviceIsPeerAvrcpConnected(void)
{
    bdaddr peer_addr;
    if (appDeviceGetPeerBdAddr(&peer_addr))
    {
        avInstanceTaskData* inst = appAvInstanceFindFromBdAddr(&peer_addr);
        if (inst)
        {
            if (!appAvrcpIsDisconnected(inst))
                return TRUE;
        }
    }
    return FALSE;
}

bool appDeviceIsPeerAvrcpConnectedForAv(void)
{
    bdaddr peer_addr;
    if (appDeviceGetPeerBdAddr(&peer_addr))
    {
        avInstanceTaskData* inst = appAvInstanceFindFromBdAddr(&peer_addr);
        if (inst)
        {
            return appAvIsAvrcpConnected(inst);
        }
    }
    return FALSE;
}

bool appDeviceSetHandsetAddressForwardReq(bdaddr *handset_bd_addr, bool reqd)
{
    appDeviceAttributes attr;

    if (appDeviceGetHandsetAttributes(handset_bd_addr, &attr, NULL))
    {
        if (reqd)
            attr.flags |= DEVICE_FLAGS_HANDSET_ADDRESS_FORWARD_REQD;
        else
            attr.flags &= ~DEVICE_FLAGS_HANDSET_ADDRESS_FORWARD_REQD;

        appDeviceSetAttributes(handset_bd_addr, &attr);

        return TRUE;
    }

    return FALSE;
}

bool appDeviceHandsetAttrIsAddressForwardReqd(appDeviceAttributes* attr)
{
    return attr->flags & DEVICE_FLAGS_HANDSET_ADDRESS_FORWARD_REQD;
}

bool appDeviceGetHandsetAddressForwardReqd(bdaddr *handset_bd_addr, bool *reqd)
{
    appDeviceAttributes attr;
    UNUSED(reqd);

    if (appDeviceGetHandsetAttributes(handset_bd_addr, &attr, NULL))
    {
        return appDeviceHandsetAttrIsAddressForwardReqd(&attr);
    }

    return FALSE;
}

bool appDeviceIsTwsPlusHandset(const bdaddr *handset_bd_addr)
{
    if (appDeviceIsHandset(handset_bd_addr))
    {
        if (appDeviceTwsVersion(handset_bd_addr) == DEVICE_TWS_VERSION)
            return TRUE;
    }

    return FALSE;
}

bool appDeviceHasJustPaired(const bdaddr *bd_addr)
{
    appDeviceAttributes attr;

    if (appDeviceFindBdAddrAttributes(bd_addr, &attr))
        return attr.flags & DEVICE_FLAGS_JUST_PAIRED;
    else
        return FALSE;
}

bool appDeviceIsHandsetAnyProfileConnected(void)
{
    return appDeviceIsHandsetHfpConnected() &&
           appDeviceIsHandsetA2dpConnected() &&
           appDeviceIsHandsetAvrcpConnected();
}

