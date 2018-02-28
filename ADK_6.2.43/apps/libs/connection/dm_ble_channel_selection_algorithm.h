/****************************************************************************
Copyright (c) 2017 Qualcomm Technologies International, Ltd.
Part of 6.2

FILE NAME
    dm_ble_channel_selection_algorithm.h      

DESCRIPTION
    Message handler function for forwarding the Channel Selection Algorithm
    Indication message from Bluestack to the Application task.

NOTES

*/

/****************************************************************************
NAME    
    connectionHandleDmBleChannelSelectionAlgorithmInd

DESCRIPTION
    Handle the DM_HCI_ULP_CHANNEL_SELECTION_ALGORITH_IND message from Bluestack
    and pass it to the application. 

RETURNS
    void
*/
void connectionHandleDmBleChannelSelectionAlgorithmInd( 
        const DM_HCI_ULP_CHANNEL_SELECTION_ALGORITHM_IND_T *ind
        );

