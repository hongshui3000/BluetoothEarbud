/*!
\copyright  Copyright (c) 2017 Qualcomm Technologies International, Ltd.\n
            All Rights Reserved.\n
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    Part of 6.2
\file       
\brief      Header file for handling the GAIA transport interface
*/

#ifndef _AV_HEADSET_GAIA_H_
#define _AV_HEADSET_GAIA_H_

#ifdef INCLUDE_DFU

#include <gaia.h>

/*! Messages which may be sent externally by the gaia module */
enum av_headset_gaia_messages
{
    APP_GAIA_CONNECT_ALLOWED_IND = APP_GAIA_MESSAGE_BASE,
};

/*! Structure of the #APP_GAIA_CONNECT_ALLOWED_IND message */
typedef struct
{
    bool        in_dfu_mode;    /*!< Flag to indicate if the application is in DFU mode.
                                    TRUE will allow a GAIA connection, FALSE will block */
} APP_GAIA_CONNECT_ALLOWED_IND_T;


/*! Data used by the GAIA module */
typedef struct
{
        /*! Task for handling messaging from upgrade library */
    TaskData        gaia_task;  
        /*! The current transport (if any) for GAIA */
    GAIA_TRANSPORT *transport;
} gaiaTaskData;

/*! Get the transport for the current GAIA connection */
#define appGetGaiaTransport()           (appGetGaia()->transport)

/*! Set the transport for the current GAIA connection */
#define appSetGaiaTransport(_transport)  do { \
                                            appGetGaia()->transport = (_transport);\
                                           } while(0)

/*! Initialise the GAIA Module */
extern void appGaiaInit(void);


/*! \brief Disconnect any active gaia connection
 */
extern void appGaiaDisconnect(void);


#endif /* INCLUDE_DFU */
#endif /* _AV_HEADSET_GAIA_H_ */

