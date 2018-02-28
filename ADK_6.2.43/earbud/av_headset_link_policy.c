/*!
\copyright  Copyright (c) 2008 - 2017 Qualcomm Technologies International, Ltd.
            All Rights Reserved.
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    Part of 6.2
\file       av_headset_link_policy.c
\brief      Link policy manager
*/

#include <panic.h>
#include <connection.h>
#include <sink.h>

#include "av_headset.h"
#include "av_headset_log.h"
#include <app/bluestack/dm_prim.h>

/*! Make and populate a bluestack DM primitive based on the type. 

    \note that this is a multiline macro so should not be used after a
    control statement (if, while) without the use of braces
 */
#define MAKE_PRIM_C(TYPE) MESSAGE_MAKE(prim,TYPE##_T); prim->common.op_code = TYPE; prim->common.length = sizeof(TYPE##_T);

/*! Lower power table for A2DP */
static const lp_power_table powertable_a2dp[]=
{
    /* mode,        min_interval, max_interval, attempt, timeout, duration */
    {lp_active,     0,            0,            0,       0,       5},  /* Active mode for 5 sec */
    {lp_passive,	0,            0,            0,       0,       1},  /* Passive mode for 1 sec */
    {lp_sniff,      48,           800,          2,       4,       0}   /* Enter sniff mode*/
};

/*! Lower power table for the A2DP with media streaming as source */
static const lp_power_table powertable_a2dp_streaming_source[]=
{
    /* mode,        min_interval, max_interval, attempt, timeout, duration */
    {lp_passive,    0,            0,            0,       0,       0},  /* Passive mode */
};

/*! Lower power table for the A2DP with media streaming as TWS sink */
static const lp_power_table powertable_a2dp_streaming_tws_sink[]=
{
    /* mode,        min_interval, max_interval, attempt, timeout, duration */
    {lp_active,     0,            0,            0,       0,       5},  /* Active mode for 5 sec */
    {lp_passive,	0,            0,            0,       0,       1},  /* Passive mode for 1 sec */
    {lp_sniff,      48,           48,           2,       4,       0}   /* Enter sniff mode*/
};

/*! Lower power table for the A2DP with media streaming as sink */
static const lp_power_table powertable_a2dp_streaming_sink[]=
{
    /* mode,        min_interval, max_interval, attempt, timeout, duration */
    {lp_active,     0,            0,            0,       0,       5},  /* Active mode for 5 sec */
    {lp_passive,	0,            0,            0,       0,       1},  /* Passive mode for 1 sec */
    {lp_sniff,      48,           48,           2,       4,       0}   /* Enter sniff mode*/
};

/*! Lower power table for the HFP. */
static const lp_power_table powertable_hfp[]=
{
    /* mode,        min_interval, max_interval, attempt, timeout, duration */
    {lp_active,     0,            0,            0,       0,       1},  /* Active mode for 1 second */
    {lp_sniff,      48,           800,          2,       1,       0}   /* Enter sniff mode (30-500ms)*/
};

/*! Lower power table for the HFP when an audio connection is open */
static const lp_power_table powertable_hfp_sco[]=
{
    /* mode,        min_interval, max_interval, attempt, timeout, duration */
    {lp_passive,	0,            0,            0,       0,       1},  /* Passive mode */
    {lp_sniff,		48, 	      144,			2,		 8,	      0}   /* Enter sniff mode (30-90ms)*/
};

/*! Lower power table for TWS+ HFP when an audio connection is open */
static const lp_power_table powertable_twsplus_hfp_sco[]=
{
    /* mode,        min_interval, max_interval, attempt, timeout, duration */
    {lp_sniff,		48, 	      144,			2,		 8,	      0}   /* Enter sniff mode (30-90ms)*/
};

/*! \cond helper */
#define ARRAY_AND_DIM(ARRAY) (ARRAY), ARRAY_DIM(ARRAY)
/*! \endcond helper */

/*! Structure for storing power tables */
struct powertable_data
{
    const lp_power_table *table;
    uint16 rows;
};

/*! Array of structs used to store the power tables for standard phones */
static const struct powertable_data powertables_standard[] = {
    [POWERTABLE_A2DP] =                    {ARRAY_AND_DIM(powertable_a2dp)},
    [POWERTABLE_A2DP_STREAMING_SOURCE] =   {ARRAY_AND_DIM(powertable_a2dp_streaming_source)},
    [POWERTABLE_A2DP_STREAMING_TWS_SINK] = {ARRAY_AND_DIM(powertable_a2dp_streaming_tws_sink)},
    [POWERTABLE_A2DP_STREAMING_SINK] =     {ARRAY_AND_DIM(powertable_a2dp_streaming_sink)},
    [POWERTABLE_HFP] =                     {ARRAY_AND_DIM(powertable_hfp)},
    [POWERTABLE_HFP_SCO] =                 {ARRAY_AND_DIM(powertable_hfp_sco)},
};

/*! Array of structs used to store the power tables for TWS+ phones */
static const struct powertable_data powertables_twsplus[] = {
    [POWERTABLE_A2DP] =                    {ARRAY_AND_DIM(powertable_a2dp)},
    [POWERTABLE_A2DP_STREAMING_SOURCE] =   {ARRAY_AND_DIM(powertable_a2dp_streaming_source)},
    [POWERTABLE_A2DP_STREAMING_TWS_SINK] = {ARRAY_AND_DIM(powertable_a2dp_streaming_tws_sink)},
    [POWERTABLE_A2DP_STREAMING_SINK] =     {ARRAY_AND_DIM(powertable_a2dp_streaming_sink)},
    [POWERTABLE_HFP] =                     {ARRAY_AND_DIM(powertable_hfp)},
    [POWERTABLE_HFP_SCO] =                 {ARRAY_AND_DIM(powertable_twsplus_hfp_sco)},
};

/* \brief Re-check and select link settings to reduce power consumption 
        where possible

    This function checks what activity the application currently has,
    and decides what the best link settings are for the connection 
    to the specified device. This may include full power (#lp_active), 
    sniff (#lp_sniff), or passive(#lp_passive) where full power is 
    no longer required but the application would prefer not to enter
    sniff mode yet.

    \param bd_addr  Bluetooth address of the device to update link settings
*/
void appLinkPolicyUpdatePowerTable(const bdaddr *bd_addr)
{
    lpPowerTableIndex pt_index = POWERTABLE_UNASSIGNED;
    avInstanceTaskData *av_inst = NULL;
    Sink sink = 0;
    lpPerConnectionState lp_state = {0};

#ifdef INCLUDE_HFP
    if (appHfpIsScoActive())
    {
        pt_index = POWERTABLE_HFP_SCO;
        sink = appHfpGetSink();
    }
#endif
#ifdef INCLUDE_AV
    if (pt_index == POWERTABLE_UNASSIGNED)
    {
        av_inst = appAvInstanceFindFromBdAddr(bd_addr);
        if (av_inst)
        {
            if (appA2dpIsStreaming(av_inst))
            {
                if (appA2dpIsSinkNonTwsCodec(av_inst))
                {
                    pt_index = POWERTABLE_A2DP_STREAMING_SINK;
                }
                else if (appA2dpIsSinkTwsCodec(av_inst))
                {
                    pt_index = POWERTABLE_A2DP_STREAMING_TWS_SINK;
                }
                else if (appA2dpIsSourceCodec(av_inst))
                {
                    pt_index = POWERTABLE_A2DP_STREAMING_SOURCE;
                }
            }
            else if (!appA2dpIsDisconnected(av_inst))
            {
                pt_index = POWERTABLE_A2DP;
            }
            sink = appAvGetSink(av_inst);
        }
    }
#endif
#ifdef INCLUDE_HFP
    if (pt_index == POWERTABLE_UNASSIGNED)
    {
        if (appHfpIsConnected())
        {
            pt_index = POWERTABLE_HFP;
            sink = appHfpGetSink();
        }
    }
#endif

    appConManagerGetLpState(bd_addr, &lp_state);
    if ((pt_index != lp_state.pt_index) && sink && (pt_index < POWERTABLE_UNASSIGNED))
    {
        const struct powertable_data *selected = appDeviceIsTwsPlusHandset(bd_addr) ?
                                                    &powertables_twsplus[pt_index] :
                                                    &powertables_standard[pt_index];
        ConnectionSetLinkPolicy(sink, selected->rows, selected->table);
        DEBUG_LOGF("appLinkPolicyUpdatePowerTable, index=%d, prev=%d", pt_index, lp_state.pt_index);
        lp_state.pt_index = pt_index;
        appConManagerSetLpState(bd_addr, &lp_state);
    }
}

/*! \brief Allow role switching 

    Update the policy for the connection (if any) to the specified
    Bluetooth address, so as to allow future role switching.

    \param  bd_addr The Bluetooth address of the device
*/
void appLinkPolicyAllowRoleSwitch(const bdaddr *bd_addr)
{
    MAKE_PRIM_C(DM_HCI_WRITE_LINK_POLICY_SETTINGS_REQ);
    BdaddrConvertVmToBluestack(&prim->bd_addr, bd_addr);
    prim->link_policy_settings = ENABLE_MS_SWITCH | ENABLE_SNIFF;
    VmSendDmPrim(prim);
    DEBUG_LOG("appLinkPolicyAllowRoleSwitch");
}

/*! \brief Prevent role switching 

    Update the policy for the connection (if any) to the specified
    Bluetooth address, so as to prevent any future role switching.

    \param  bd_addr The Bluetooth address of the device
*/
void appLinkPolicyPreventRoleSwitch(const bdaddr *bd_addr)
{
    MAKE_PRIM_C(DM_HCI_WRITE_LINK_POLICY_SETTINGS_REQ);
    BdaddrConvertVmToBluestack(&prim->bd_addr, bd_addr);
    prim->link_policy_settings = ENABLE_SNIFF;
    VmSendDmPrim(prim);
    DEBUG_LOG("appLinkPolicyPreventRoleSwitch");
}

/*! \brief Check and update links

    This function checks the role of the individual links and attempts
    role switches when required.
    
    Role switches are only attempts to avoid scatternets.
*/    
static void appLinkPolicyCheckRole(void)
{
    avInstanceTaskData *av_inst_source, *av_inst_sink, *tws_inst_sink;
    lpTaskData *theLp = appGetLp();
    
    DEBUG_LOG("appLinkPolicyCheckRole");

    tws_inst_sink = appAvGetA2dpSink(AV_CODEC_TWS);
    if (tws_inst_sink)
    {
        /* Check AV isn't streaming */
        if (!appAvIsStreaming())
        {
            /* Try and be slave of AV link */
            if (theLp->av_sink_role != hci_role_slave)
            {
                ConnectionSetRole(&theLp->task, appAvGetSink(tws_inst_sink), hci_role_slave);
                theLp->av_sink_role = hci_role_slave;
            }
        }
    }
    else
    {
        /* Get AV source and sink instances (NULL if not connected) */
        av_inst_sink = appAvGetA2dpSink(AV_CODEC_NON_TWS);
        av_inst_source = appAvGetA2dpSource();

        /* Check if we link to AV sink */
        if (av_inst_sink != NULL)
        {
            /* Check if we also have link to AV relay */
            if (av_inst_source != NULL)
            {
                /* Try and be master of both links */
                if (theLp->av_source_role != hci_role_master)
                {
                    ConnectionSetRole(&theLp->task, appAvGetSink(av_inst_source), hci_role_master);
                    theLp->av_source_role = hci_role_master;
                }

                /* Only attempt to set role of links if we are not streaming,
                   as during role-switch only 1-slot data is allowed */
                if (!appAvIsStreaming())
                {
                    if (theLp->av_sink_role != hci_role_master)
                    {
                        ConnectionSetRole(&theLp->task, appAvGetSink(av_inst_sink), hci_role_master);
                        theLp->av_sink_role = hci_role_master;
                    }
                }
            }
    
            /* Check if we also have link to HFP */
            if (appHfpIsConnected())
            {
                tp_bdaddr hfp_bd_addr;

                /* Check AV & HFP are on different links */
                if (SinkGetBdAddr(appHfpGetSink(), &hfp_bd_addr) &&
                    !appAvIsBdAddr(&hfp_bd_addr.taddr.addr))
                {
                    /* Only attempt to set role of links if SCO is not active and we are
                       not streaming, as role-switch isn't allowed when a SCO link is
                       active and during role-switch only 1-slot data is allowed */

                    /* Check HFP doesn't have active SCO */
                    if (!appHfpIsScoActive())
                    {
                        /* Try and be master of HFP link */
                        if (theLp->hfp_role != hci_role_master)
                        {
                            ConnectionSetRole(&theLp->task, appHfpGetSink(), hci_role_master);
                            theLp->hfp_role = hci_role_master;
                        }
                    }

                    /* Check AV isn't streaming */
                    if (!appAvIsStreaming())
                    {
                        /* Try and be master of AV link */
                        if (theLp->av_sink_role != hci_role_master)
                        {
                            ConnectionSetRole(&theLp->task, appAvGetSink(av_inst_sink), hci_role_master);
                            theLp->av_sink_role = hci_role_master;
                        }
                    }
                }
            }
        }
    }
}

/*! \brief Update role of link

    This function is called whenver the role of a link has changed or been
    confirmed, it checks the Bluetooth Address of the updated link against
    the address of the HFP and A2DP links and updates the role of the matching
    link.
*/    
static void appLinkPolicyUpdateRole(const bdaddr *bd_addr, hci_role role)
{
#ifdef INCLUDE_AV
    lpTaskData *theLp = appGetLp();
    avInstanceTaskData *av_source_inst, *av_sink_inst;
               
    /* Check if role confirmation for AV sink link */
    av_sink_inst = appAvGetA2dpSink(AV_CODEC_ANY);
    if (av_sink_inst != NULL)
    {
        tp_bdaddr av_bd_addr;        
        if (SinkGetBdAddr(appAvGetSink(av_sink_inst), &av_bd_addr) &&
            BdaddrIsSame(bd_addr, &av_bd_addr.taddr.addr))
        {
            if (role == hci_role_master)
                DEBUG_LOG("appLinkPolicyUpdateRole, av sink, role=master");
            else
                DEBUG_LOG("appLinkPolicyUpdateRole, av sink, role=slave");

            theLp->av_sink_role = role;
        }
    }

    /* Check if role confirmation for AV source link  */
    av_source_inst = appAvGetA2dpSource();
    if (av_source_inst != NULL)
    {
        tp_bdaddr av_bd_addr;
        if (SinkGetBdAddr(appAvGetSink(av_source_inst), &av_bd_addr) &&
            BdaddrIsSame(bd_addr, &av_bd_addr.taddr.addr))
        {
            if (role == hci_role_master)
                DEBUG_LOG("appLinkPolicyUpdateRole, av relay, role=master");
            else
                DEBUG_LOG("appLinkPolicyUpdateRole, av relay, role=slave");

            theLp->av_source_role = role;
        }            
    }
        
#ifdef INCLUDE_HFP
    /* Check if role confirmation for HFP link */
    if (appHfpIsConnected())
    {
        tp_bdaddr hfp_bd_addr;
        if (SinkGetBdAddr(appHfpGetSink(), &hfp_bd_addr) &&
            BdaddrIsSame(bd_addr, &hfp_bd_addr.taddr.addr))
        {
            if (role == hci_role_master)
                DEBUG_LOG("appLinkPolicyUpdateRole, hfp, role=master");
            else
                DEBUG_LOG("appLinkPolicyUpdateRole, htp, role=slave");

            theLp->hfp_role = role;
        }
    }            
#endif

#endif
}

/*! \brief Confirmation of link role

    This function is called to handle a CL_DM_ROLE_CFM message, this message is sent from the
    connection library in response to a call to ConnectionGetRole().
    
    Extract the Bluetooth address of the link and call appLinkPolicyUpdateRole to update the
    role of the appropriate link.

    \param  cfm The received confirmation
*/    
void appLinkPolicyHandleClDmRoleConfirm(CL_DM_ROLE_CFM_T *cfm)
{
    tp_bdaddr bd_addr;
    if (cfm->role == hci_role_master)
        DEBUG_LOGF("appLinkPolicyHandleClDmRoleConfirm, status=%d, role=master", cfm->status);
    else
        DEBUG_LOGF("appLinkPolicyHandleClDmRoleConfirm, status=%d, role=slave", cfm->status, (cfm->role == hci_role_master) ? "master" : "slave");

    
    if (SinkGetBdAddr(cfm->sink, &bd_addr))
    {
        appLinkPolicyUpdateRole(&bd_addr.taddr.addr, cfm->role);
        if (cfm->status == hci_success)
            appLinkPolicyCheckRole();
    }
}

/*! \brief Indication of link role

    This function is called to handle a CL_DM_ROLE_IND message, this message is sent from the
    connection library whenever the role of a link changes.
    
    Extract the Bluetooth address of the link and call appLinkPolicyUpdateRole to update the
    role of the appropriate link.  Call appLinkPolicyCheckRole to check if we need to perform
    a role switch.

    \param  ind The received indication
*/
void appLinkPolicyHandleClDmRoleIndication(CL_DM_ROLE_IND_T *ind)
{
    if (ind->role == hci_role_master)
        DEBUG_LOG("appLinkPolicyHandleClDmRoleIndication, master");
    else
        DEBUG_LOG("appLinkPolicyHandleClDmRoleIndication, slave");

    appLinkPolicyUpdateRole(&ind->bd_addr, ind->role);
    appLinkPolicyCheckRole();
}

/*! \brief Get updated link role

    Request from application to check and update the link role for the
    specified sink.

    \param  sink    The Sink to check and update
*/
void appLinkPolicyUpdateRoleFromSink(Sink sink)
{
    lpTaskData *theLp = appGetLp();   

    /* Get current role for this link */
    ConnectionGetRole(&theLp->task, sink);
}

/*! \brief Initialise link policy manager

    Call as startyp to initialise the link policy manager, set all
    the store rols to 'don't care'.
*/    
void appLinkPolicyInit(void)
{
    lpTaskData *theLp = appGetLp();   

#ifdef INCLUDE_AV
    theLp->av_sink_role = hci_role_dont_care;
    theLp->av_source_role = hci_role_dont_care;
#endif
#ifdef INCLUDE_HFP
    theLp->hfp_role = hci_role_dont_care;
#endif
}
