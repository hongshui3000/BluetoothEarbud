/*!
\copyright  Copyright (c) 2017 Qualcomm Technologies International, Ltd.
            All Rights Reserved.
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    Part of 6.2
\file       av_headset_test.h
\brief      Interface for specifc application testing functions
*/

#ifndef AV_HEADSET_TEST_H
#define AV_HEADSET_TEST_H

#include "av_headset.h"

/*! Test value for battery voltage. If non-zero, the function
    appBatteryGetVoltage() will return this value instead of
    a real reading.
 */
extern uint16 appTestBatteryVoltage;

/*! @brief Put Earbud into Handset Pairing mode 
*/
void appTestPairHandset(void);

/*! @brief Delete all Handset pairing
*/
void appTestDeleteHandset(void);

/*! @brief Put Earbud into Peer Pairing mode
*/
void appTestPairPeer(void);

/*! @brief Delete Earbud peer pairing

    Attempts to delete pairing to earbud. Check the debug output if
    this test command fails, as it will report failure reason.

    \return FALSE if there is no peer pairing, or still connected to the device.
        TRUE if pairing was successfully removed.
*/
bool appTestDeletePeer(void);

/*! @brief Return if Earbud is in a Pairing mode

    @return bool TRUE Earbud is in pairing mode
                 FALSE Earbud is not in pairing mode
*/
bool appTestIsPairingInProgress(void);

/*! @brief Initiate Earbud A2DP connection to the Handset

    @return bool TRUE if A2DP connection is initiated
                 FALSE if no handset is paired 
*/
bool appTestHandsetA2dpConnect(void);

/*! @brief Return if Earbud has an Handset A2DP connection

    @return bool TRUE Earbud has A2DP Handset connection
                 FALSE Earbud does not have A2DP Handset connection
*/
bool appTestIsHandsetA2dpConnected(void);

/*! @brief Return if Earbud is in A2DP streaming mode with the handset

    @return bool TRUE Earbud is in A2DP streaming mode
                     FALSE Earbud is not in A2DP streaming mode
*/
bool appTestIsHandsetA2dpStreaming(void);

/*! @brief Initiate Earbud AVRCP connection to the Handset

    @return bool TRUE if AVRCP connection is initiated
                 FALSE if no handset is paired 
*/
bool appTestHandsetAvrcpConnect(void);

/*! @brief Return if Earbud has an Handset AVRCP connection

    @return bool TRUE Earbud has AVRCP Handset connection
                 FALSE Earbud does not have AVRCP Handset connection
*/
bool appTestIsHandsetAvrcpConnected(void);

/*! @brief Initiate Earbud HFP connection to the Handset

    @return bool TRUE if HFP connection is initiated
                 FALSE if no handset is paired 
*/
bool appTestHandsetHfpConnect(void);

/*! @brief Return if Earbud has an Handset HFP connection

    @return bool TRUE Earbud has HFP Handset connection
                 FALSE Earbud does not have HFP Handset connection
*/
bool appTestIsHandsetHfpConnected(void);

/*! @brief Return if Earbud has an Handset HFP SCO connection

    @return bool TRUE Earbud has HFP SCO Handset connection
                 FALSE Earbud does not have HFP SCO Handset connection
*/
bool appTestIsHandsetHfpScoActive(void);

/*! @brief Initiate Earbud HFP Voice Dial request to the Handset

    @return bool TRUE if Voice Dial request is initiated
                 FALSE if HFP is not connected 
*/
bool appTestHandsetHfpVoiceDial(void);

/*! @brief Initiate Earbud HFP Voice Transfer request to the Handset

    @return bool TRUE if Voice Transfer request is initiated
                 FALSE if HFP is not connected 
*/
bool appTestHandsetHfpVoiceTransfer(void);

/*! @brief Mute Microphone audio on HFP SCO conenction to handset

    @return bool TRUE if mute request is successful
                 FALSE if HFP is not connected
*/
bool appTestHandsetHfpMute(void);

/*! @brief Unmute Microphone audio on HFP SCO conenction to handset

    @return bool TRUE if unmute request is successful
                 FALSE if HFP is not connected
*/
bool appTestHandsetHfpUnMute(void);

/*! @brief Transfer HFP voice to the Handset

    @return bool TRUE if Voice Transfer request is initiated
                 FALSE if HFP is not connected
*/
bool appTestHandsetHfpVoiceTransferToAg(void);

/*! @brief Transfer HFP voice to the Earbud

    @return bool TRUE if Voice Transfer request is initiated
                 FALSE if HFP is not connected
*/
bool appTestHandsetHfpVoiceTransferToHeadset(void);

/*! @brief Accept incoming call

    @return bool TRUE if call was accepted
                 FALSE if HFP is not connected
*/
bool appTestHandsetHfpCallAccept(void);

/*! @brief Reject incoming call

    @return bool TRUE if call was rejected
                 FALSE if HFP is not connected
*/
bool appTestHandsetHfpCallReject(void);

/*! @brief End current call

    @return bool TRUE if call was ended
                 FALSE if HFP is not connected
*/
bool appTestHandsetHfpCallHangup(void);

/*! @brief Initiated last number redial

    @return bool TRUE if last number redial was initiated
                 FALSE if HFP is not connected
*/
bool appTestHandsetHfpCallLastDialed(void);

/*! @brief Get microphone mute status

    @return bool TRUE if microphone muted,
                 FALSE if not muted
*/
bool appTestIsHandsetHfpMuted(void);

/*! @brief Check if call is in progress

    @return bool TRUE if call in progress,
                 FALSE if no call, or not connected
*/
bool appTestIsHandsetHfpCall(void);

/*! @brief Check if incoming call

    @return bool TRUE if call incoming,
                 FALSE if no call, or not connected
*/
bool appTestIsHandsetHfpCallIncoming(void);

/*! @brief Check if outgoing call

    @return bool TRUE if call outgoing,
                 FALSE if no call, or not connected
*/
bool appTestIsHandsetHfpCallOutgoing(void);

/*! @brief Return if Earbud has a connection to the Handset

    This can be HFP, A2DP or AVRCP

    @return bool TRUE Earbud has a connection to the Handset
                 FALSE Earbud does not have a connection to the Handset
*/
bool appTestIsHandsetConnected(void);

/*! @brief Initiate Earbud A2DP connection to the the Peer

    @return bool TRUE if A2DP connection is initiated
                 FALSE if no Peer is paired 
*/
bool appTestPeerA2dpConnect(void);

/*! @brief Return if Earbud has a Peer A2DP connection

    @return bool TRUE Earbud has A2DP Peer connection
                 FALSE Earbud does not have A2DP Peer connection
*/
bool appTestIsPeerA2dpConnected(void);

/*! @brief Check if Earbud is in A2DP streaming mode with peer Earbud

    \return TRUE if A2DP streaming to peer device
*/
bool appTestIsPeerA2dpStreaming(void);

/*! @brief Initiate Earbud AVRCP connection to the the Peer

    @return bool TRUE if AVRCP connection is initiated
                 FALSE if no Peer is paired 
*/
bool appTestPeerAvrcpConnect(void);

/*! @brief Return if Earbud has a Peer AVRCP connection

    @return bool TRUE Earbud has AVRCP Peer connection
                 FALSE Earbud does not have AVRCP Peer connection
*/
bool appTestIsPeerAvrcpConnected(void);

/*! @brief Send the Avrcp pause command to the Handset
*/
void appTestAvPause(void);

/*! @brief Send the Avrcp play command to the Handset
*/
void appTestAvPlay(void);

/*! @brief Send the Avrcp stop command to the Handset
*/
void appTestAvStop(void);

/*! @brief Send the Avrcp forward command to the Handset
*/
void appTestAvForward(void);

/*! @brief Send the Avrcp backward command to the Handset
*/
void appTestAvBackward(void);

/*! @brief Send the Avrcp fast forward state command to the Handset
*/
void appTestAvFastForwardStart(void);

/*! @brief Send the Avrcp fast forward stop command to the Handset
*/
void appTestAvFastForwardStop(void);

/*! @brief Send the Avrcp rewind start command to the Handset
*/
void appTestAvRewindStart(void);

/*! @brief Send the Avrcp rewind stop command to the Handset
*/
void appTestAvRewindStop(void);

/*! @brief Send the Avrcp volume change command to the Handset

    @param step [IN] Step change to apply to volume
    @return bool TRUE volume step change sent
                 FALSE volume step change was not sent
*/
bool appTestAvVolumeChange(int8 step);

/*! @brief Send the Avrcp pause command to the Handset

    @param volume   New volume level to set (0-127).
*/
void appTestAvVolumeSet(uint8 volume);

/*! \brief Set the volume level in dB's 

    The volume is scaled based on the range of dB supported,
    which is specified by appConfigMaxVolumedB() and 
    appConfigMinVolumedB().
    
    \param  gain    Level to set, in dBs, within the allowed range.
 */
void appTestAvVolumeSetDb(int8 gain);

/*! \brief Allow tests to control whether the earbud will enter dormant.

    If an earbud enters dormant, cannot be woken by a test. 

    \note Even if dormant mode is enabled, the application may not
        enter dormant. Dormant won't be used if the application is 
        busy, or connected to a charger - both of which are quite
        likely for an application under test.

    \param  enable  Use FALSE to disable dormant, or TRUE to enable.
*/
void appTestPowerAllowDormant(bool enable);

/*! \brief Generate event that Earbud is now in the case. */
void appTestPhyStateInCaseEvent(void);

/*! \brief Generate event that Earbud is now out of the case. */
void appTestPhyStateOutOfCaseEvent(void);

/*! \brief Generate event that Earbud is now in ear. */
void appTestPhyStateInEarEvent(void);

/*! \brief Generate event that Earbud is now out of the ear. */
void appTestPhyStateOutOfEarEvent(void);

/*! \brief Generate event that Earbud is now moving */
void appTestPhyStateMotionEvent(void);

/*! \brief Generate event that Earbud is now not moving. */
void appTestPhyStateNotInMotionEvent(void);

/*! \brief Generate event that Earbud is now (going) off. */
void appTestPhyStateOffEvent(void);

/*! \brief Reset an Earbud to factory defaults.
    Will drop any connections, delete all pairing and reboot.
*/
void appTestFactoryReset(void);

/*! \brief Connect to default handset. */
void appTestConnectHandset(void);

/*! \brief  Check if peer synchronisation was successful 

    \returns TRUE if we are synchronised with the peer.
*/
bool appTestIsPeerSyncComplete(void);

/*! \brief Power off.
    Drops any connections then powers off.
*/
void appTestPowerOff(void);



#endif // AV_HEADSET_TEST_H
