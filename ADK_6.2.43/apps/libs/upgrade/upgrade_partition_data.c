/****************************************************************************
Copyright (c) 2014 - 2015 Qualcomm Technologies International, Ltd.
Part of 6.2

FILE NAME
    upgrade_partition_data.c

DESCRIPTION
    Upgrade file processing state machine.
    It is parsing and validating headers.
    All received data are passed to MD5 validation.
    Partition data are written to SQIF.

NOTES

*/

#include <string.h>
#include <stdlib.h>

#include <byte_utils.h>
#include <panic.h>
#include <print.h>

#include "upgrade_partition_data.h"
#include "upgrade_partition_data_priv.h"
#include "upgrade_ctx.h"
#include "upgrade_fw_if.h"
#include "upgrade_psstore.h"
#include "upgrade_partitions.h"



static UpgradeHostErrorCode BigDataHandler(void);
static UpgradeHostErrorCode ParseCompleteData(uint8 *data, uint16 len, bool reqComplete);

static UpgradeHostErrorCode HandleFooterState(uint8 *data, uint16 len, bool reqComplete);

void UpgradePartitionDataDestroy(void)
{
    UpgradePartitionDataCtx *ctx = UpgradeCtxGetPartitionData();

    if(ctx)
    {
        if(ctx->signature)
        {
            free(ctx->signature);
            ctx->signature = 0;
        }

        free(ctx);
        UpgradeCtxSetPartitionData(0);
    }
}

uint32 UpgradePartitionDataGetNextReqSize(void)
{
    UpgradePartitionDataCtx *ctx = UpgradeCtxGetPartitionData();

    if(ctx->incompleteData.inProgress)
    {
        return 0;
    }
    else
    {
        return ctx->nextReqSize;
    }
}

uint32 UpgradePartitionDataGetNextOffset(void)
{
    UpgradePartitionDataCtx *ctx = UpgradeCtxGetPartitionData();

    return ctx->nextOffset;
}

/****************************************************************************
NAME
    UpgradePartitionDataParse  -  Handler for smaller than requested packets.

DESCRIPTION
    Checks whether the store requested is one of those that we support, then
    finds the length of the allocated data (if any), allocates storage and
    loads the data from Persistent store into memory.

RETURNS
    Upgrade library error code.
*/
UpgradeHostErrorCode UpgradePartitionDataParse(uint8 *data, uint16 len)
{
    switch (UpgradePartitionDataParseDataCopy(data,len))
    {
    case UPGRADE_PARTITION_DATA_XFER_ERROR:
    default:
        return UPGRADE_HOST_ERROR_BAD_LENGTH_PARTITION_PARSE;

    case UPGRADE_PARTITION_DATA_XFER_IN_PROGRESS:
        return UPGRADE_HOST_SUCCESS;
        
    case UPGRADE_PARTITION_DATA_XFER_COMPLETE:
        {
            return BigDataHandler();
        }
    }
}

/****************************************************************************
NAME
    UpgradePartitionDataParseDataCopy  -  Handler for smaller than requested packets.

DESCRIPTION
    Function to process incoming data and deal with according to the pending 
    request.

    The return value indicates if the packet, held in incompleteData variable 
    is now complete, in progress, or if an error occurred.

RETURNS
    UpgradePartitionDataPartialTransferState - state of the data transfer
*/
UpgradePartitionDataPartialTransferState UpgradePartitionDataParseDataCopy(uint8 *data, uint16 len)
{
    UpgradePartitionDataCtx *ctx = UpgradeCtxGetPartitionData();
    uint16 newLength = ctx->incompleteData.size + len;

    if (   (newLength > ctx->nextReqSize)
        || (0 == ctx->nextReqSize))
    {
        /* Ensure we generate errors from now on */
        ctx->nextReqSize = 0;
        /* And tidy up incompleteData */
        ctx->incompleteData.inProgress = FALSE;
        ctx->incompleteData.size = 0;
        return UPGRADE_PARTITION_DATA_XFER_ERROR;
    }

    /* Initialise for the special first case */
    if (0 == ctx->incompleteData.size)
    {
        memset(&ctx->incompleteData, 0, sizeof(ctx->incompleteData));
        ctx->incompleteData.inProgress = TRUE;
    }

    memmove(&ctx->incompleteData.data[ctx->incompleteData.size], data, len);
    ctx->incompleteData.size += len;

    if (newLength == ctx->nextReqSize)
    {
        /* We have completed the last packet */
        ctx->incompleteData.inProgress = FALSE;
        ctx->incompleteData.size = 0;
        
        return UPGRADE_PARTITION_DATA_XFER_COMPLETE;
    }

    return UPGRADE_PARTITION_DATA_XFER_IN_PROGRESS;
}

/****************************************************************************
NAME
    UpgradePartitionDataStopData  -  Stop processing incoming data

DESCRIPTION
    Function to stop the processing of incoming data.
*/
void UpgradePartitionDataStopData(void)
{
    UpgradeCtxGetPartitionData()->nextReqSize = 0;
}



/****************************************************************************
NAME
    UpgradePartitionDataRequestData

DESCRIPTION
    Determine size of the next data request
*/
void UpgradePartitionDataRequestData(uint32 size)
{
    UpgradePartitionDataCtx *ctx = UpgradeCtxGetPartitionData();

    ctx->bigReqSize = size;

    /* Determine size of the next data request. */
    ctx->nextReqSize = (ctx->bigReqSize > UPGRADE_PARTITION_DATA_BLOCK_SIZE(UpgradeCtxGet())) ?
            UPGRADE_PARTITION_DATA_BLOCK_SIZE(UpgradeCtxGet()) : ctx->bigReqSize;

    ctx->bigReqSize -= ctx->nextReqSize;

    PRINT(("PART_DATA: UpgradePartitionDataRequestData size %ld, big %ld, next %ld\n", size, ctx->bigReqSize, ctx->nextReqSize));
}

/****************************************************************************
NAME
    BigDataHandler  -  Handling of requests of size bigger than the block size

DESCRIPTION
    It determines size of the next data request send to a host.
    It informs parser when the last parser's request for data is fulfilled,
    by setting reqComplete flag.

RETURNS
    Upgrade library error code.
*/
static UpgradeHostErrorCode BigDataHandler(void)
{
    UpgradePartitionDataCtx *ctx = UpgradeCtxGetPartitionData();
    uint8 *data = ctx->incompleteData.data;
    uint16 len = ctx->nextReqSize;

    bool reqComplete = FALSE;

    /* Since we have received some data offset should be cleared */
    ctx->nextOffset = 0;

    /* Determine size of the next data request. */
    ctx->nextReqSize = (ctx->bigReqSize > UPGRADE_PARTITION_DATA_BLOCK_SIZE(UpgradeCtxGet())) ?
            UPGRADE_PARTITION_DATA_BLOCK_SIZE(UpgradeCtxGet()) : ctx->bigReqSize;

    /* Determine if this last packet in a 'big' request.
     * Decrement data counter otherwise.
     */
    if(ctx->bigReqSize == 0)
    {
        reqComplete = TRUE;
    }
    else
    {
        ctx->bigReqSize -= ctx->nextReqSize;
    }

    /* Parse received data */
    return ParseCompleteData(data, len, reqComplete);
}

/****************************************************************************
NAME
    ParseCompleteData  -  Parser state machine

DESCRIPTION
    Calls state handlers depending of current state.
    All state handlers are setting size of next data request.

RETURNS
    Upgrade library error code.
*/
UpgradeHostErrorCode ParseCompleteData(uint8 *data, uint16 len, bool reqComplete)
{
    UpgradePartitionDataCtx *ctx = UpgradeCtxGetPartitionData();

    UpgradeHostErrorCode rc = UPGRADE_HOST_ERROR_INTERNAL_ERROR_1;

    {
        uint16 i;
        for(i = 0; i < len; ++i)
        {
            PRINT(("data parse data[%d] 0x%x\n", i, data[i]));
        }
    }

    switch(ctx->state)
    {
    case UPGRADE_PARTITION_DATA_STATE_GENERIC_1ST_PART:
        PRINT(("PART_DATA: UpgradePartitionDataHandleGeneric1stPartState\n"));
        rc = UpgradePartitionDataHandleGeneric1stPartState(data, len, reqComplete);
        break;

    case UPGRADE_PARTITION_DATA_STATE_HEADER:
        PRINT(("PART_DATA: UpgradePartitionDataHandleHeaderState\n"));
        rc = UpgradePartitionDataHandleHeaderState(data, len, reqComplete);
        break;

    case UPGRADE_PARTITION_DATA_STATE_DATA_HEADER:
        PRINT(("PART_DATA: UpgradePartitionDataHandleDataHeaderState\n"));
        rc = UpgradePartitionDataHandleDataHeaderState(data, len, reqComplete);
        break;

    case UPGRADE_PARTITION_DATA_STATE_DATA:
        PRINT(("PART_DATA: UpgradePartitionDataHandleDataState len %d, complete %d\n", len, reqComplete));
        rc = UpgradePartitionDataHandleDataState(data, len, reqComplete);
        break;

    case UPGRADE_PARTITION_DATA_STATE_FOOTER:
        PRINT(("PART_DATA: HandleFooterState\n"));
        rc = HandleFooterState(data, len, reqComplete);
        break;
    }

    return rc;
}

/****************************************************************************
NAME
    HandleFooterState  -  Signature data handling.

DESCRIPTION
    Collects MD5 signature data and sends it for validation.
    Completion of this step means that data were download to a SQIF.

RETURNS
    Upgrade library error code.
*/
UpgradeHostErrorCode HandleFooterState(uint8 *data, uint16 len, bool reqComplete)
{
    UpgradePartitionDataCtx *ctx = UpgradeCtxGetPartitionData();

    PRINT(("HandleFooterState\n"));

    UpgradePartitionDataCopyFromStream(ctx->signature, ctx->signatureReceived, data, len);

    ctx->signatureReceived += len;

    if(reqComplete)
    {
        bool signatureValid;

        PRINT(("PART_DATA: Upgrade file download is finished\n"));

        signatureValid = UpgradeFWIFValidateFinalize(ctx->signature);

        free(ctx->signature);
        ctx->signature = 0;
        ctx->signatureReceived = 0;

        if(signatureValid)
        {
            return UPGRADE_HOST_OEM_VALIDATION_SUCCESS;
        }
        else
        {
            return UPGRADE_HOST_ERROR_OEM_VALIDATION_FAILED_FOOTER;
        }
    }

    return UPGRADE_HOST_SUCCESS;
}

bool UpgradePartitionDataIsDfuUpdate(void)
{
    return (UpgradeCtxGetPSKeys()->dfu_partition_num != 0);
}

uint16 UpgradePartitionDataGetDfuPartition(void)
{
    return UpgradeCtxGetPSKeys()->dfu_partition_num - 1;
}

bool UpgradeIsPartitionDataState(void)
{
    UpgradePartitionDataCtx *ctx = UpgradeCtxGetPartitionData();
    bool rc = TRUE;

    if(ctx == NULL)
    {
        return FALSE;
    }
     if(ctx->state == UPGRADE_PARTITION_DATA_STATE_DATA)
    {
        rc = TRUE;
    }
     else
    {
        rc = FALSE;
    }

    return rc;
}

uint32 UpgradeGetPartitionSizeInPartitionDataState(void)
{
    UpgradePartitionDataCtx *ctx = UpgradeCtxGetPartitionData();
    uint32 size;

    if(ctx == NULL)
    {
        size = 0;
    }
    else
    {
        size = ctx->partitionLength - ctx->nextOffset - first_word_size;
    }

    return size;
}
