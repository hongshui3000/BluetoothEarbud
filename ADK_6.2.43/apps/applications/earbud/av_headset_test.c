/*!
\copyright  Copyright (c) 2017 Qualcomm Technologies International, Ltd.
            All Rights Reserved.
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    Part of 6.2
\file       av_headset_test.c
\brief      Implementation of specifc application testing functions 
*/


#include "av_headset.h"
#include "av_headset_log.h"

#include <cryptovm.h>

uint16 appTestBatteryVoltage = 0;

/*! @brief Put Earbud into Handset Pairing mode 
*/
void appTestPairHandset(void)
{
    appSmPairHandset();
}

/*! @brief Delete all Handset pairing
*/
void appTestDeleteHandset(void)
{
    appSmDeleteHandsets();
}

/*! @brief Put Earbud into Peer Pairing mode 
*/
void appTestPairPeer(void)
{
    appPairingPeerPair(NULL, FALSE);
}

/*! @brief Delete Earbud peer pairing
*/
bool appTestDeletePeer(void)
{
    bdaddr bd_addr;

    /* Check if we have previously paired with an earbud */
    if (appDeviceGetPeerBdAddr(&bd_addr))
    {
        return appDeviceDelete(&bd_addr);
    }
    else
    {
        DEBUG_LOG("appTestDeletePeer: NO PEER TO DELETE");
        return FALSE;
    }
}


/*! @brief Return if Earbud is in a Pairing mode
*/
bool appTestIsPairingInProgress(void)
{
    return !appPairingIsIdle();
}

/*! @brief Initiate Earbud A2DP connection to the Handset
*/
bool appTestHandsetA2dpConnect(void)
{
    bdaddr bd_addr;

    if (appDeviceGetHandsetBdAddr(&bd_addr))
    {
        return appAvA2dpConnectRequest(&bd_addr, A2DP_CONNECT_NOFLAGS);
    }
    else
    {
        return FALSE;
    }
}

/*! @brief Return if Earbud has an Handset A2DP connection
*/
bool appTestIsHandsetA2dpConnected(void)
{
    bdaddr bd_addr;
    appDeviceAttributes attributes;
    int index = 0;

    while (appDeviceGetHandsetAttributes(&bd_addr, &attributes, &index))
    {
        /* Find handset AV instance */
        avInstanceTaskData *theInst = appAvInstanceFindFromBdAddr(&bd_addr);
        if (theInst)
            return appA2dpIsConnected(theInst);
    }

    /* If we get here then there's no A2DP connected for handset */
    return FALSE;
}

/*! @brief Return if Earbud is in A2DP streaming mode with the handset
*/
bool appTestIsHandsetA2dpStreaming(void)
{
    bdaddr bd_addr;
    appDeviceAttributes attributes;
    int index = 0;

    while (appDeviceGetHandsetAttributes(&bd_addr, &attributes, &index))
    {
        /* Find handset AV instance */
        avInstanceTaskData *theInst = appAvInstanceFindFromBdAddr(&bd_addr);
        if (theInst)
            return appA2dpIsStreaming(theInst);
    }

    /* If we get here then there's no A2DP connected for handset */
    return FALSE;
}


/*! @brief Initiate Earbud AVRCP connection to the Handset
*/
bool appTestHandsetAvrcpConnect(void)
{
    bdaddr bd_addr;

    if (appDeviceGetHandsetBdAddr(&bd_addr))
        return  appAvAvrcpConnectRequest(NULL, &bd_addr);
    else
        return FALSE;
}

/*! @brief Return if Earbud has an Handset AVRCP connection
*/
bool appTestIsHandsetAvrcpConnected(void)
{
    bdaddr bd_addr;
    appDeviceAttributes attributes;
    int index = 0;

    while (appDeviceGetHandsetAttributes(&bd_addr, &attributes, &index))
    {
        /* Find handset AV instance */
        avInstanceTaskData *theInst = appAvInstanceFindFromBdAddr(&bd_addr);
        if (theInst)
            return appAvrcpIsConnected(theInst);
    }

    /* If we get here then there's no AVRCP connected for handset */
    return FALSE;
}

/*! @brief Initiate Earbud HFP connection to the Handset
*/
bool appTestHandsetHfpConnect(void)
{
    bdaddr bd_addr;

    if (appDeviceGetHandsetBdAddr(&bd_addr))
        return appHfpConnectWithBdAddr(&bd_addr, hfp_handsfree_107_profile);
    else
        return FALSE;
}

/*! @brief Return if Earbud has an Handset HFP connection
*/
bool appTestIsHandsetHfpConnected(void)
{
    return appHfpIsConnected();
}

/*! @brief Return if Earbud has an Handset HFP SCO connection
*/
bool appTestIsHandsetHfpScoActive(void)
{
    return appHfpIsScoActive();
}

/*! @brief Initiate Earbud HFP Voice Dial request to the Handset
*/
bool appTestHandsetHfpVoiceDial(void)
{
    if (appHfpIsConnected())
    {
        appHfpCallVoice();
        return TRUE;
    }
    else
        return FALSE;
}

/*! @brief Initiate Earbud HFP Voice Transfer request to the Handset
*/
bool appTestHandsetHfpVoiceTransfer(void)
{
    if (appHfpIsCall())
    {
        if (appHfpIsScoActive())
            appHfpTransferToAg();
        else
            appHfpTransferToHeadset();
        return TRUE;
    }
    else
        return FALSE;
}

bool appTestHandsetHfpMute(void)
{
    if (appHfpIsCall())
    {
        if (!appHfpIsMuted())
            appHfpMuteToggle();
        return TRUE;
    }
    else
        return FALSE;
}

bool appTestHandsetHfpUnMute(void)
{
    if (appHfpIsCall())
    {
        if (appHfpIsMuted())
            appHfpMuteToggle();
        return TRUE;
    }
    else
        return FALSE;
}

bool appTestHandsetHfpVoiceTransferToAg(void)
{
    if (appHfpIsCall() && appHfpIsScoActive())
    {
        appHfpTransferToAg();
        return TRUE;
    }
    else
        return FALSE;
}

bool appTestHandsetHfpVoiceTransferToHeadset(void)
{
    if (appHfpIsCall())
    {
        appHfpTransferToHeadset();
        return TRUE;
    }
    else
        return FALSE;
}

bool appTestHandsetHfpCallAccept(void)
{
    if (appHfpIsCall())
    {
        appHfpCallAccept();
        return TRUE;
    }
    else
        return FALSE;
}

bool appTestHandsetHfpCallReject(void)
{
    if (appHfpIsCall())
    {
        appHfpCallReject();
        return TRUE;
    }
    else
        return FALSE;
}

bool appTestHandsetHfpCallHangup(void)
{
    if (appHfpIsCall())
    {
        appHfpCallHangup();
        return TRUE;
    }
    else
        return FALSE;
}

bool appTestHandsetHfpCallLastDialed(void)
{
    if (appHfpIsConnected())
    {
        appHfpCallLastDialed();
        return TRUE;
    }
    else
        return FALSE;
}

bool appTestIsHandsetHfpMuted(void)
{
    return appHfpIsMuted();
}

bool appTestIsHandsetHfpCall(void)
{
    return appHfpIsCall();
}

bool appTestIsHandsetHfpCallIncoming(void) 
{
    return appHfpIsCallIncoming();
}

bool appTestIsHandsetHfpCallOutgoing(void)
{
    return appHfpIsCallOutgoing();
}

/*! @brief Return if Earbud has a connection to the Handset
*/
bool appTestIsHandsetConnected(void)
{
    return appTestIsHandsetA2dpConnected() ||
           appTestIsHandsetAvrcpConnected() ||
           appTestIsHandsetHfpConnected();
}

/*! @brief Initiate Earbud A2DP connection to the the Peer
*/
bool appTestPeerA2dpConnect(void)
{
    bdaddr bd_addr;

    if (appDeviceGetPeerBdAddr(&bd_addr))
    {
        return appAvA2dpConnectRequest(&bd_addr, A2DP_CONNECT_NOFLAGS);
    }
    else
    {
        return FALSE;
    }
}

/*! @brief Return if Earbud has a Peer A2DP connection
*/
bool appTestIsPeerA2dpConnected(void)
{
    bdaddr bd_addr;
    appDeviceAttributes attributes;

    if (appDeviceGetPeerAttributes(&bd_addr, &attributes))
    {
        /* Find peer AV instance */
        avInstanceTaskData *theInst = appAvInstanceFindFromBdAddr(&bd_addr);
        if (theInst)
            return appA2dpIsConnected(theInst);
    }

    /* If we get here then there's no A2DP connected for handset */
    return FALSE;
}


/*! @brief Return if Earbud is in A2DP streaming mode with peer Earbud
*/
bool appTestIsPeerA2dpStreaming(void)
{
    bdaddr bd_addr;
    appDeviceAttributes attributes;

    if (appDeviceGetPeerAttributes(&bd_addr, &attributes))
    {
        /* Find peer AV instance */
        avInstanceTaskData *theInst = appAvInstanceFindFromBdAddr(&bd_addr);
        if (theInst)
            return appA2dpIsStreaming(theInst);
    }

    /* If we get here then there's no A2DP connected for peer */
    return FALSE;
}


/*! @brief Initiate Earbud AVRCP connection to the the Peer
*/
bool appTestPeerAvrcpConnect(void)
{
    bdaddr bd_addr;

    if (appDeviceGetPeerBdAddr(&bd_addr))
        return  appAvAvrcpConnectRequest(NULL, &bd_addr);
    else
        return FALSE;
}

/*! @brief Return if Earbud has a Peer AVRCP connection
*/
bool appTestIsPeerAvrcpConnected(void)
{
    bdaddr bd_addr;
    appDeviceAttributes attributes;

    while (appDeviceGetPeerAttributes(&bd_addr, &attributes))
    {
        /* Find handset AV instance */
        avInstanceTaskData *theInst = appAvInstanceFindFromBdAddr(&bd_addr);
        if (theInst)
            return appAvrcpIsConnected(theInst);
    }

    /* If we get here then there's no AVRCP connected for handset */
    return FALSE;
}

/*! @brief Send the Avrcp pause command to the Handset
*/
void appTestAvPause(void)
{
    appAvPause(FALSE);
}

/*! @brief Send the Avrcp play command to the Handset
*/
void appTestAvPlay(void)
{
    appAvPlay(FALSE);
}

/*! @brief Send the Avrcp stop command to the Handset
*/
void appTestAvStop(void)
{
    appAvStop(FALSE);
}

/*! @brief Send the Avrcp forward command to the Handset
*/
void appTestAvForward(void)
{
    appAvForward();
}

/*! @brief Send the Avrcp backward command to the Handset
*/
void appTestAvBackward(void)
{
    appAvBackward();
}

/*! @brief Send the Avrcp fast forward state command to the Handset
*/
void appTestAvFastForwardStart(void)
{
    appAvFastForwardStart();
}

/*! @brief Send the Avrcp fast forward stop command to the Handset
*/
void appTestAvFastForwardStop(void)
{
    appAvFastForwardStop();
}

/*! @brief Send the Avrcp rewind start command to the Handset
*/
void appTestAvRewindStart(void)
{
    appAvRewindStart();
}

/*! @brief Send the Avrcp rewind stop command to the Handset
*/
void appTestAvRewindStop(void)
{
    appAvRewindStop();
}

/*! @brief Send the Avrcp volume change command to the Handset
*/
bool appTestAvVolumeChange(int8 step)
{
    return appAvVolumeChange(step);
}

/*! @brief Send the Avrcp pause command to the Handset
*/
void appTestAvVolumeSet(uint8 volume)
{
   appAvVolumeSet(volume, NULL);
}

void appTestAvVolumeSetDb(int8 gain)
{
    /* Set default volume as set in av_headset_config.h */
    const int rangeDb = appConfigMaxVolumedB() - appConfigMinVolumedB();

    if (    gain < appConfigMinVolumedB()
        ||  gain > appConfigMaxVolumedB())
    {
        DEBUG_LOGF("appTestAvVolumeSetDb. Gain %d outside range %d-%d",
                        gain,appConfigMinVolumedB(),appConfigMaxVolumedB());
    }

    appAvVolumeSet((gain - appConfigMinVolumedB()) * 127 / rangeDb, NULL);
}

void appTestPowerAllowDormant(bool enable)
{
#ifdef INCLUDE_POWER_CONTROL
    powerTaskData *thePower = appGetPowerControl();

    thePower->allow_dormant = enable;
#else
    DEBUG_LOGF("appTestPowerAllowDormant(%d): Power Control/Dormant is not in this build.",enable);
#endif
}


extern void TestLinkkeyGen(void)
{
    bdaddr bd_addr;
    uint16 lk[8];
    uint16 lk_out[8];

    bd_addr.nap = 0x0002;
    bd_addr.uap = 0x5B;
    bd_addr.lap = 0x00FF02;

    lk[0] = 0x9541;
    lk[1] = 0xe6b4;
    lk[2] = 0x6859;
    lk[3] = 0x0791;
    lk[4] = 0x9df9;
    lk[5] = 0x95cd;
    lk[6] = 0x9570;
    lk[7] = 0x814b;

    appPairingGenerateLinkKey(&bd_addr, lk, 0x74777332UL, lk_out);

#if 0
    bd_addr.nap = 0x0000;
    bd_addr.uap = 0x74;
    bd_addr.lap = 0x6D7031;

    lk[0] = 0xec02;
    lk[1] = 0x34a3;
    lk[2] = 0x57c8;
    lk[3] = 0xad05;
    lk[4] = 0x3410;
    lk[5] = 0x10a6;
    lk[6] = 0x0a39;
    lk[7] = 0x7d9b;
#endif

    appPairingGenerateLinkKey(&bd_addr, lk, 0x6c656272UL, lk_out);

}

extern void TestH6(void)
{
    uint8 key_h7[16] = {0xec,0x02,0x34,0xa3,0x57,0xc8,0xad,0x05,0x34,0x10,0x10,0xa6,0x0a,0x39,0x7d,0x9b};
    //uint32 key_id = 0x6c656272;
    uint32 key_id = 0x7262656c;
    uint8 key_h6[16];

    CryptoVmH6(key_h7, key_id, key_h6);
    printf("H6: ");
    for (int h6_i = 0; h6_i < 16; h6_i++)
        printf("%02x ", key_h6[h6_i]);
    printf("\n");
}


extern void appTestHandsetInfo(void)
{
    appDeviceAttributes attributes;
    bdaddr bd_addr;
    if (appDeviceGetHandsetAttributes(&bd_addr, &attributes, NULL))
    {
        DEBUG_LOGF("appTestHandsetInfo, bdaddr %04x,%02x,%06lx, version %u.%02u",
                   bd_addr.nap, bd_addr.uap, bd_addr.lap,
                   attributes.tws_version >> 8, attributes.tws_version & 0xFF);
        DEBUG_LOGF("appTestHandsetInfo, supported %02x, connected %02x",
                   attributes.supported_profiles, attributes.connected_profiles);
        DEBUG_LOGF("appTestHandsetInfo, a2dp volume %u",
                   attributes.a2dp_volume);
        DEBUG_LOGF("appTestHandsetInfo, flags %02x",
                   attributes.flags);
    }
}

#include "av_headset_log.h"


extern void TestDebug(void)
{
    DEBUG_LOGF("test %d %d", 1, 2);
    DEBUG_LOG("test");
}

/*! \brief Generate event that Earbud is now in the case. */
void appTestPhyStateInCaseEvent(void)
{
    appPhyStateInCaseEvent();
}

/*! \brief Generate event that Earbud is now out of the case. */
void appTestPhyStateOutOfCaseEvent(void)
{
    appPhyStateOutOfCaseEvent();
}

/*! \brief Generate event that Earbud is now in ear. */
void appTestPhyStateInEarEvent(void)
{
    appPhyStateInEarEvent();
}

/*! \brief Generate event that Earbud is now out of the ear. */
void appTestPhyStateOutOfEarEvent(void)
{
    appPhyStateOutOfEarEvent();
}

/*! \brief Generate event that Earbud is now moving */
void appTestPhyStateMotionEvent(void)
{
    appPhyStateMotionEvent();
}

/*! \brief Generate event that Earbud is now not moving. */
void appTestPhyStateNotInMotionEvent(void)
{
    appPhyStateNotInMotionEvent();
}

/*! \brief Reset an Earbud to factory defaults.
    Will drop any connections, delete all pairing and reboot.
*/
void appTestFactoryReset(void)
{
    appSmFactoryReset();
}

void appTestConnectHandset(void)
{
    appSmConnectHandset();
}

bool appTestIsPeerSyncComplete(void)
{
    return appSmIsPeerSyncComplete();
}

#ifdef INCLUDE_POWER_CONTROL
void appTestPowerOff(void)
{
    appPowerOff();
}
#endif


