/*!
\copyright  Copyright (c) 2008 - 2018 Qualcomm Technologies International, Ltd.\n
            All Rights Reserved.\n
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    Part of 6.2
\file       av_headset.h
\brief      Main application header file

\mainpage   Earbuds application.

            This is a working sample application for earbuds supporting
            True Wireless Stereo (TWS) and True Wireless Stereo Plus (TWS+).

            The application is usable as delivered, but expectation is that
            customers will adapt the application. Options for doing this
            include
            \li Build defines to control features included
            \li Changes to the configuration (\ref av_headset_config.h)
            \li Software changes
			
			@startuml

			class init << (T,Green) >> #paleGreen {  
				Initialise the application
				....
				Initialises all other modules
				at startup
			}
			class led << (T,lightblue) >> {     
				Manages the LEDs
				}
			class hfp << (T,lightblue) >> { 
				Manages the HFP state machine
				}
			class ui << (T,lightblue) >> { 
				Manages the UI 
				}
			class sm << (T,lightblue) >> #lightBlue { 
				Main application state machine 
				}
			class lp << (T,lightblue) >> {
				Manages the device link policy settings
				}
			class av << (T,lightblue) >> {
				Manages the A2DP and AVRCP connections
				}
			class charger << (T,lightblue) >> { 
				Manages the charger state machine
				}
			class battery << (T,lightblue) >> { 
				Manages the battery status
				}
			class pairing << (T,lightblue) >> { 
				Manages the device pairing state machine 
				}
			class scanning << (T,lightblue) >> { 
				Manages the device scanning state 
				}
			class device << (T,lightblue) >> { 
				Manages the paired devices
				}
			class con_manager << (T,lightblue) >> { 
				Manages the device connection state machine 
				}
			class peer_sig << (T,lightblue) >> { 
				Manages peer signalling messages
				}
			class handset_sig << (T,lightblue) >> { 
				Manages the handset signalling messages 
				}
			class phy_state << (T,lightblue) >> { 
				Manages the physcial state of the device
				}
			class kymera << (T,lightblue) >> { 
				Manages the audio subsystem
				}
			class upgrade << (T,red) >> #lightGrey {     
				Manages the device upgrade 
				}
			class proximity << (T,lightblue) >> { 
				Manages the proximity sensor device 
				}
			class accelerometer << (T,lightblue) >> { 
				Manages the accelerometer device 
				}
			class rules << (.,red) >> #yellow { 
				Application rules engine 
				}
			class chains << (.,red) >> #pink { 
				User defined audio chains 
				}
			class buttons << (.,red) >> #pink { 
				User defined button inputs 
				}

			class A << (T,lightblue) >> {
			}

			class B << (T,lightblue) >> {
			}

			init -[hidden]right->upgrade
			init -[hidden]down->charger
			upgrade -[hidden]right->A

			A -right[#blue]> B : Registered Event
			A -right-> B : Function Call
			A -d[hidden]-> charger

			rules<-r->sm
			chains-l->kymera
			buttons-l->ui

			' Physical Inputs
			proximity -d-> phy_state : in/out ear
			accelerometer -d-> phy_state :  motion status
			charger -d-> phy_state : charger status
			charger -[hidden]r->proximity
			proximity -[hidden]r->accelerometer

			phy_state -[hidden]down->sm
			phy_state -down[#blue]> sm : phy state

			charger -d-> battery : charger status
			battery --> sm : battery status

			'UI control
			sm -r-> ui : ui indications
			ui -u-> led : enable/disable
			ui -d-> kymera : tones

			'Registered con_manager tasks
			con_manager -u[#blue]-> av : av con status
			con_manager -u[#blue]-> hfp : hfp con status
			con_manager -u[#blue]-> device : dev status
			con_manager -u[#blue]> sm : con status

			sm -d-> scanning : enable/disable
			pairing -l-> scanning : enable/disable

			handset_sig --> sm : handset status

			av -u[#blue]-> sm : av status
			av -[#blue]-> peer_sig : con/dis/sig
			av --> device : dev info
			av -[#blue]-> handset_sig : con/dis/sig
			av --> lp : policy settings
			av -r-> kymera : stereo music

			hfp -u[#blue]-> sm : hfp status
			hfp --> device : dev info
			hfp -[#blue]-> handset_sig : con/dis/sig
			hfp --> lp : policy settings
			hfp -r-> kymera : voice call

			peer_sig -r[#blue]-> sm : peer status
			peer_sig -d[#blue]-> pairing : status
			peer_sig -l-> scanning : enable/disable

			@enduml		
*/

#ifndef _AV_HEADSET_H_
#define _AV_HEADSET_H_

#include <connection.h>
#include <a2dp.h>
#include <avrcp.h>
#include <hfp.h>
#include <message.h>
#include <stream.h>
#include <bdaddr.h>
#include <stdio.h>
#include <source.h>
#include <stdlib.h>
#include <pio.h>

#include "av_headset_message.h"
#include "av_headset_config.h"
#include "av_headset_led.h"
#include "av_headset_power.h"
#include "av_headset_av.h"
#include "av_headset_a2dp.h"
#include "av_headset_avrcp.h"
#include "av_headset_hfp.h"
#include "av_headset_battery.h"
#include "av_headset_charger.h"
#include "av_headset_ui.h"
#include "av_headset_link_policy.h"
#include "av_headset_kymera.h"
#include "av_headset_gatt.h"
#include "av_headset_device.h"
#include "av_headset_pairing.h"
#include "av_headset_scan_manager.h"
#include "av_headset_con_manager.h"
#include "av_headset_init.h"
#include "av_headset_peer_signalling.h"
#include "av_headset_sm.h"
#include "av_headset_handset_signalling.h"
#include "av_headset_phy_state.h"
#include "av_headset_kymera.h"
#include "av_headset_test.h"
#include "av_headset_conn_rules.h"
#include "av_headset_upgrade.h"
#include "av_headset_gaia.h"
#include "av_headset_accelerometer.h"
#include "av_headset_proximity.h"

/*!@{ \name Defines used to the class of device */
#define AUDIO_MAJOR_SERV_CLASS      0x200000
#define RENDER_MAJOR_SERV_CLASS     0x040000
#define AV_MAJOR_DEVICE_CLASS       0x000400
#define HEADSET_MINOR_DEVICE_CLASS  0x000004
/*!@}*/

/*! Minor version number used for the GAIA interface */
#define GAIA_API_MINOR_VERSION 5

/*! Internal messages */
enum
{
                                            /*!  Connecting reminder timeout */
    APP_INTERNAL_UI_CONNECTING_TIMEOUT= INTERNAL_MESSAGE_BASE,     
    APP_INTERNAL_UI_INQUIRY_TIMEOUT,        /*!< Inquiry reminder timeout */
    APP_INTERNAL_PAIRING_INIT
};


/*! Trusted device index used for AV devices */
#define APP_AV_TDI  (1)

/*! Trusted device index used for HFP devices */
#define APP_HFP_TDI (2)

/*! User PSKEY base for attributes */
#define APP_ATTRIBUTE_PS_BASE   (16)

/*! User PSKEY to store AV configuration */
#define PS_AV_CONFIG            (0)

/*! User PSKEY to store HFP configuration */
#define PS_HFP_CONFIG           (1)

/*! Application task data */
typedef struct appTaskData
{
    TaskData            task;                   /*!< Application task */
    TaskData            systask;                /*!< Handler for system messages */
    Task                codec_task;             /*!< CODEC task */
#if 0
    /* \todo DELETE */
    unsigned            auto_connect_enable:1;  /*!< Flag set when auto connect enabled */
#endif

    initData            init;                   /*!< Structure used while initialising */
    ledTaskData         led;                    /*!< LED data structure */
#ifdef INCLUDE_HFP
    hfpTaskData         hfp;                    /*!< HFP data structure */
#endif
    uiTaskData          ui;                     /*!< UI data structure */
    lpTaskData          lp;                     /*!< Link Policy Manager data structure */
#ifdef INCLUDE_AV
    avTaskData          av;                     /*!< AV data structure */
#endif
#ifdef INCLUDE_CHARGER
    chargerTaskData     charger;                /*!< Charger data structure */
#endif
    batteryTaskData     battery;                /*!< Battery data structure */
    gattTaskData        gatt;                   /*!< App GATT component task */
    pairingTaskData     pairing;                /*!< App pairing task */
    scanManagerTaskData scanning;               /*!< Scan Manager data structure. */
    deviceTaskData      device;                 /*!< App device management task */
    conManagerTaskData  con_manager;            /*!< Connection manager task data */
    peerSigTaskData     peer_sig;               /*!< Peer earbud signalling */
    handsetSigTaskData  handset_sig;            /*!< Handset signalling */
    phyStateTaskData    phy_state;              /*!< Physical state of the Earbud. */
    kymeraTaskData      kymera;                 /*!< State data for the DSP configuration */
    smTaskData          sm;                     /*!< Application state machine. */
    connRulesTaskData   conn_rules;             /*!< Connection rules. */
#ifdef INCLUDE_DFU
    gaiaTaskData        gaia;                   /*!< Task information for GAIA support */
    upgradeTaskData     upgrade;                /*!< Task information for UPGRADE support */
#endif
#ifdef INCLUDE_PROXIMITY
    proximityTaskData   proximity;              /*!< Task information for proximity sensor */
#endif
#ifdef INCLUDE_ACCELEROMETER
    accelTaskData       accelerometer;          /*!< Task information for accelerometer */
#endif
#ifdef INCLUDE_POWER_CONTROL
    powerTaskData       power;                  /*!< Information for power_control */
#endif
} appTaskData;

/*! The global application data structure. 

\note Do not access directly */
extern appTaskData globalApp;

/*! Get pointer to application data structure */    
#define appGetApp()         (&globalApp)

/*! Get pointer to application task */    
#define appGetAppTask()     (&globalApp.task)

/*! Get pointer to the system message task */    
#define appGetSysTask()     (&globalApp.systask)

/*! Get pointer to init data structure */
#define appGetInit()        (&globalApp.init)

/*! Get pointer to UI data structure */    
#define appGetUi()          (&globalApp.ui)

/*! Get pointer to UI task */    
#define appGetUiTask()      (&globalApp.ui.task)

/*! Get pointer to LED controller data structure */    
#define appGetLed()         (&globalApp.led)

/*! Get pointer to the proximity sensor data structure */
#define appGetProximity()   (&globalApp.proximity)

/*! Get pointer to the acceleromter data structure */
#define appGetAccelerometer() (&globalApp.accelerometer)

/*! Get pointer to Link Policy Manager data structure */    
#define appGetLp()          (&globalApp.lp)

/*! Get pointer to CODEC task */    
#define appGetCodecTask()   (globalApp.codec_task)

/*! Get pointer to HFP data structure */    
#define appGetHfp()         (&globalApp.hfp)

/*! Get pointer to HFP task */    
#define appGetHfpTask()     (&globalApp.hfp.task)

/*! Get pointer to the GATT modules task data */
#define appGetGatt()                (&globalApp.gatt)
/*! Get pointer to the GATT modules task */
#define appGetGattTask()            (&globalApp.gatt.gatt_task)
/*! Get pointer to the GATT server info of the GATT module */
#define appGetGattServer()          (&globalApp.gatt.gatt_server)
/*! Get pointer to the GAP server info of the GATT module */
#define appGetGapServer()           (&globalApp.gatt.gap_server)

/*! Get pointer to Pairing data structure */
#define appGetPairing()             (&globalApp.pairing)

/*! Get pointer to scan manager data structure */
#define appGetScanning()            (&globalApp.scanning)

/*! Get pointer to Device Management data structure */
#define appGetDevice()              (&globalApp.device)

/*! Get pointer to Connection Management data structure */
#define appGetConManager()          (&globalApp.con_manager)

/*! Get pointer to the peer signalling modules data structure */
#define appGetPeerSig()             (&globalApp.peer_sig)

/*! Get pointer to handset signalling data structure */
#define appGetHandsetSig()          (&globalApp.handset_sig)

/*! Get pointer to physical state data structure */
#define appGetPhyState()            (&globalApp.phy_state)

/*! Get pointer to Kymera structure */
#define appGetKymera()              (&globalApp.kymera);

/*! Get pointer to application state machine task data struture. */
#define appGetSm()                  (&globalApp.sm)
/*! Get pointer to the state machine modules task. */
#define appGetSmTask()              (&globalApp.sm.task)

/*! Get pointer to the connection rules task data structure. */
#define appGetConnRules()           (&globalApp.conn_rules)

#ifdef INCLUDE_DFU
/*! Get the info for the applications Gaia support */
#define appGetGaia()                (&globalApp.gaia)

/*! Get the Task info for the applications Gaia task */
#define appGetGaiaTask()            (&globalApp.gaia.gaia_task)

/*! Get the info for the applications upgrade support */
#define appGetUpgrade()             (&globalApp.upgrade)

/*! Get the Task info for the applications Upgrade task */
#define appGetUpgradeTask()         (&globalApp.upgrade.upgrade_task)
#endif /* INCLUDE_DFU */

#ifdef INCLUDE_AV
/*! Get pointer to AV data structure */    
#define appGetAv()                  (&globalApp.av)
#endif

#ifdef INCLUDE_CHARGER
/*! Get pointer to charger data structure */    
#define appGetCharger()             (&globalApp.charger)
#endif

#ifdef INCLUDE_POWER_CONTROL
/*! Get pointer to data for power control */
#define appGetPowerControl()        (&globalApp.power)

/*! Get pointer to data for power control */
#define appGetPowerTask()           (&globalApp.power.task)
#endif


/*! Get pointer to battery data structure */
#define appGetBattery()     (&globalApp.battery)

#ifdef INCLUDE_DFU
/*! Check if application is in DFU mode */
//#define appIsDfu()          (appGetSm()->state == APP_STATE_DFU)
#else
//#define appIsDfu()          (FALSE)
#endif

#if defined(INCLUDE_AV) && (INCLUDE_HFP)
#define appHandleUnexpected(id) \
    DEBUG_LOGF("appHandleUnexpected, id = %x, sm = %d, hfp = %d, av = %d", (id), appGetApp()->sm.state, appGetApp()->hfp.state, appGetApp()->av.state)
#elif defined(INCLUDE_AV)
#define appHandleUnexpected(id) \
    DEBUG_LOGF("appHandleUnexpected, id = %x, sm = %d, av = %d", (id),  appGetApp()->sm.state, appGetApp()->av.state)
#elif defined(INCLUDE_HFP)
#define appHandleUnexpected(id) \
    DEBUG_LOGF("appHandleUnexpected, id = %x, sm = %d, hfp = %d", (id),  appGetApp()->sm.state, appGetApp()->hfp.state)
#else
#define appHandleUnexpected(id) ((void)0)
#endif

/*! Message handler function for unexpected system messages */
#define appHandleSysUnexpected(id) \
    DEBUG_LOGF("appHandleSysUnexpected, id = 0x%x (%d)", (id), (id))

#endif /* _AV_HEADSET_H_ */
