/*!
    \copyright Copyright (c) 2018 Qualcomm Technologies International, Ltd.
        All Rights Reserved.
        Qualcomm Technologies International, Ltd. Confidential and Proprietary.
    \version Part of 6.2
    \file chain_sco_wb_2mic.h
    \brief The chain_sco_wb_2mic chain. This file is generated by C:/qtil/ADK_QCC512x_QCC302x_WIN_6.2.43/tools/chaingen/chaingen.py.
*/

#ifndef _CHAIN_SCO_WB_2MIC_H__
#define _CHAIN_SCO_WB_2MIC_H__

/*!
    @startuml
        object OPR_SCO_RECEIVE
        OPR_SCO_RECEIVE : id = CAP_ID_WBS_DEC
        object OPR_SCO_SEND
        OPR_SCO_SEND : id = CAP_ID_WBS_ENC
        object OPR_CVC_RECEIVE
        OPR_CVC_RECEIVE : id = CAP_ID_CVC_RECEIVE_WB
        object OPR_CVC_SEND
        OPR_CVC_SEND : id = CAP_ID_CVCHS2MIC_MONO_SEND_WB
        object OPR_SOURCE_SYNC
        OPR_SOURCE_SYNC : id = CAP_ID_SOURCE_SYNC
        object OPR_VOLUME_CONTROL
        OPR_VOLUME_CONTROL : id = CAP_ID_VOL_CTRL_VOL
        object OPR_SCO_AEC
        OPR_SCO_AEC : id = CAP_ID_AEC_REFERENCE
        OPR_CVC_RECEIVE "IN(0)"<-- "OUT(0)" OPR_SCO_RECEIVE
        OPR_SOURCE_SYNC "IN(0)"<-- "OUT(0)" OPR_CVC_RECEIVE
        OPR_VOLUME_CONTROL "MAIN_IN(0)"<-- "OUT(0)" OPR_SOURCE_SYNC
        OPR_SCO_AEC "INPUT1(0)"<-- "OUT(0)" OPR_VOLUME_CONTROL
        OPR_CVC_SEND "IN1(1)"<-- "OUTPUT1(3)" OPR_SCO_AEC
        OPR_CVC_SEND "IN2(2)"<-- "OUTPUT2(4)" OPR_SCO_AEC
        OPR_SCO_SEND "IN(0)"<-- "OUT(0)" OPR_CVC_SEND
        OPR_CVC_SEND "REFERENCE(0)"<-- "REFERENCE(0)" OPR_SCO_AEC
        object EPR_SCO_FROM_AIR #lightgreen
        OPR_SCO_RECEIVE "SCO_IN(0)" <-- EPR_SCO_FROM_AIR
        object EPR_SCO_MIC1 #lightgreen
        OPR_SCO_AEC "MIC1(2)" <-- EPR_SCO_MIC1
        object EPR_SCO_MIC2 #lightgreen
        OPR_SCO_AEC "MIC1b(3)" <-- EPR_SCO_MIC2
        object EPR_SCO_VOLUME_AUX #lightgreen
        OPR_VOLUME_CONTROL "AUX_IN(1)" <-- EPR_SCO_VOLUME_AUX
        object EPR_SCO_SPEAKER #lightblue
        EPR_SCO_SPEAKER <-- "SPEAKER1(1)" OPR_SCO_AEC
        object EPR_SCO_TO_AIR #lightblue
        EPR_SCO_TO_AIR <-- "SCO_OUT(0)" OPR_SCO_SEND
    @enduml
*/

#include <chain.h>

extern const chain_config_t chain_sco_wb_2mic_config;

#endif /* _CHAIN_SCO_WB_2MIC_H__ */

