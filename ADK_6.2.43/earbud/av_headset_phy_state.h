/*!
\copyright  Copyright (c) 2015 - 2017 Qualcomm Technologies International, Ltd.\n
            All Rights Reserved.\n
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    Part of 6.2
\file       av_headset_phy_state.h
\brief	    Definition of the physical state of an Earbud.
*/

#ifndef AV_HEADSET_PHY_STATE_H
#define AV_HEADSET_PHY_STATE_H

#include "av_headset_message.h"
#include "av_headset_tasklist.h"

/*! Clients will receive state update messages via #PHY_STATE_CHANGED_IND
    conforming to the following state sequence.
    @startuml

    IN_CASE : Earbud is in the case
    IN_CASE -right-> OUT_OF_EAR : Remove from case

    OUT_OF_EAR : Earbud is not in the case nor in the ear.
    OUT_OF_EAR -right-> IN_EAR : Put in ear
    OUT_OF_EAR -left-> IN_CASE : Put in case
    OUT_OF_EAR -down-> OUT_OF_EAR_AT_REST : No motion for N seconds

    OUT_OF_EAR_AT_REST : Earbud is not moving
    OUT_OF_EAR_AT_REST -up-> OUT_OF_EAR : Motion detected

    IN_EAR : Earbud is in the ear
    IN_EAR : Usable as a microphone and speaker.
    IN_EAR -right-> OUT_OF_EAR : Remove from ear

    @enduml
*/

/*! \brief Enumeration of the physical states an Earbud can be in.
 */
typedef enum
{
    /*! The earbud physical state is unknown. It will be known after the first
        sensor report. This state value will not be reported to clients. */
    PHY_STATE_UNKNOWN,
    /*! The earbud is in the case. */
    PHY_STATE_IN_CASE,
    /*! The earbud is not in the case nor is it in the ear.
     *  It *may* be in motion or at rest. */
    PHY_STATE_OUT_OF_EAR,
    /*! The earbud is not in the case not is it in the ear, and no motion
     * has been detected for configurable period of time */
    PHY_STATE_OUT_OF_EAR_AT_REST,
    /*! The earbud is in the ear and usuable as a microphone and speaker. */
    PHY_STATE_IN_EAR
} phyState;

/*! \brief Physial State module state. */
typedef struct
{
    /*! Physical State module message task. */
    TaskData task;
    /*! Current physical state of the device. */
    phyState state;
    /*! List of tasks to receive #PHY_STATE_CHANGED_IND notifications. */
    TaskList* client_tasks;
    /*! Stores the motion state */
    bool in_motion;
    /*! Stores the proximity state */
    bool in_proximity;
} phyStateTaskData;

/*! \brief Messages which may be sent by the Physical State module. */
typedef enum av_headset_phy_state_messages
{
    /*! Indication of a changed physical state */
    PHY_STATE_CHANGED_IND = PHY_STATE_MESSAGE_BASE
};

/*! \brief Definition of #PHY_STATE_CHANGED_IND message. */
typedef struct
{
    /*! The physical state which the device is now in. */
    phyState new_state;
} PHY_STATE_CHANGED_IND_T;

/*! \brief Internal messages the physical state module can send itself. */
typedef enum
{
    PHY_STATE_INTERNAL_IN_CASE_EVENT,
    PHY_STATE_INTERNAL_OUT_OF_CASE_EVENT,
    PHY_STATE_INTERNAL_IN_EAR_EVENT,
    PHY_STATE_INTERNAL_OUT_OF_EAR_EVENT,
    PHY_STATE_INTERNAL_MOTION,
    PHY_STATE_INTERNAL_NOT_IN_MOTION,
};

/*! \brief Register a task for notification of changes in state.
    @param client_task [IN] Task to receive PHY_STATE_CHANGED_IND messages.
 */
extern void appPhyStateRegisterClient(Task client_task);

/*! \brief Unregister a task for notification of changes in state.
    @param client_task [IN] Task to unregister.
 */
extern void appPhyStateUnregisterClient(Task client_task);

/*! \brief Get the current physical state of the device.
    \return phyState Current physical state of the device.
*/
extern phyState appPhyStateGetState(void);

/*! \brief Handle notification that Earbud is now in the case. */
extern void appPhyStateInCaseEvent(void);

/*! \brief Handle notification that Earbud is now out of the case. */
extern void appPhyStateOutOfCaseEvent(void);

/*! \brief Handle notification that Earbud is now in ear. */
extern void appPhyStateInEarEvent(void);

/*! \brief Handle notification that Earbud is now out of the ear. */
extern void appPhyStateOutOfEarEvent(void);

/*! \brief Handle notification that Earbud is now moving */
extern void appPhyStateMotionEvent(void);

/*! \brief Handle notification that Earbud is now not moving. */
extern void appPhyStateNotInMotionEvent(void);

#endif /* AV_HEADSET_PHY_STATE_H */

