Copyright (c) 2016 Qualcomm Technologies International, Ltd.
  %%version
FILE
        tcp.msg

DESCRIPTION

        Define the messages used by the PAN firmware for TCP

        Based on a header by mgr which will be generated from this file.

MESSAGE_PREFIX          TCP

MESSAGE_HEADER
#include "pantypes.h"
#include "ip.h"
MESSAGE_HEADER_END

MESSAGE_CONSTANT        TCP_PRIM_BASE           0x200
MESSAGE_CONSTANT        ENUM_SEP_TCP_FIRST_PRIM TCP_PRIM_BASE
MESSAGE_CONSTANT        ENUM_SEP_TCP_LAST_PRIM  (TCP_PRIM_BASE + 16)

MESSAGE_TAGS
        OPEN_REQ                (TCP_PRIM_BASE + 1)
        OPEN_CFM
        OPEN_IND
        OPEN_RSP
MESSAGE_IF !BLUELAB
        DATA_REQ
        DATA_CFM
        DATA_IND
        DATA_RSP
MESSAGE_END_IF !BLUELAB
        CLOSE_REQ               (TCP_OPEN_RSP + 5)
        CLOSE_IND
        CLOSED_IND
        CLOSED_RSP
        ABORT_REQ
        ABORT_IND
        ABORT_RSP
MESSAGE_TAGS_END

MESSAGE_COMMON
        uint16                  type
MESSAGE_COMMON_END

MESSAGE_BEGIN OPEN_REQ
        phandle_t               phandle
        IP_ADDR                 rem_addr
        uint16                  rem_port
        uint16                  out_size
        uint16                  loc_port
        uint16                  in_size
MESSAGE_END

MESSAGE_ENUM TCP_OPEN_CFM_STATUS
        TCP_OPEN_CFM_STATUS_SUCCESS
        TCP_OPEN_CFM_STATUS_BADARG
        TCP_OPEN_CFM_STATUS_NOMEM
        TCP_OPEN_CFM_STATUS_NOLOCADDR
MESSAGE_ENUM_END

MESSAGE_BEGIN OPEN_CFM
        phandle_t               phandle
        TCP_OPEN_CFM_STATUS     status
        BUFFER_HANDLE           b_in
        BUFFER_HANDLE           b_out
MESSAGE_END


MESSAGE_ENUM TCP_OPEN_IND_STATUS
        TCP_OPEN_IND_STATUS_ACCEPT
        TCP_OPEN_IND_STATUS_REJECT
MESSAGE_ENUM_END

MESSAGE_BEGIN OPEN_IND
        phandle_t               phandle
        BUFFER_HANDLE           b_out
        TCP_OPEN_IND_STATUS     status
        IP_ADDR                 rem_addr
        uint16                  rem_port
MESSAGE_END

MESSAGE_ENUM TCP_OPEN_RSP_STATUS
        TCP_OPEN_RSP_STATUS_ACCEPT
        TCP_OPEN_RSP_STATUS_REJECT
MESSAGE_ENUM_END

MESSAGE_BEGIN OPEN_RSP
        BUFFER_HANDLE           b
        TCP_OPEN_RSP_STATUS     status
MESSAGE_END


MESSAGE_IF !BLUELAB

MESSAGE_BEGIN DATA_REQ
        BUFFER_HANDLE           b
        uint16                  len
MESSAGE_IF !BTCHIP
        uint8 *                 data
MESSAGE_END_IF !BTCHIP
MESSAGE_END

MESSAGE_BEGIN DATA_CFM
        phandle_t               phandle
        BUFFER_HANDLE           b_out
        uint16                  len
MESSAGE_END

MESSAGE_BEGIN DATA_IND
        phandle_t               phandle
MESSAGE_IF BTCHIP
        MBLK_T_PTR              mblk
MESSAGE_ELSE
        uint16                  b_out
        uint16                  len
        uint8 *                 data
MESSAGE_END_IF BTCHIP
MESSAGE_END

MESSAGE_BEGIN DATA_RSP
        BUFFER_HANDLE           b
        uint16                  len
MESSAGE_END

MESSAGE_END_IF !BLUELAB

MESSAGE_BEGIN CLOSE_REQ
        BUFFER_HANDLE           b
MESSAGE_END

MESSAGE_BEGIN CLOSE_IND
        phandle_t               phandle
        BUFFER_HANDLE           b_out
MESSAGE_END

MESSAGE_BEGIN CLOSED_IND
        phandle_t               phandle
        BUFFER_HANDLE           b_out
MESSAGE_END

MESSAGE_BEGIN CLOSED_RSP
        BUFFER_HANDLE           b
MESSAGE_END

MESSAGE_BEGIN ABORT_REQ
        BUFFER_HANDLE           b
MESSAGE_END

MESSAGE_BEGIN ABORT_IND
        phandle_t               phandle
        BUFFER_HANDLE           b_out
MESSAGE_END

MESSAGE_ENUM TCP_ABORT_RSP_FLAGS
        TCP_ABORT_RSP_FLAGS_COMPLETE
        TCP_ABORT_RSP_FLAGS_BACKTOLISTEN
MESSAGE_ENUM_END

MESSAGE_BEGIN ABORT_RSP
        BUFFER_HANDLE           b
        TCP_ABORT_RSP_FLAGS     flags
MESSAGE_END
