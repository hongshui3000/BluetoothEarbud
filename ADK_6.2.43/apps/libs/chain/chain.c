/****************************************************************************
Copyright (c) 2016 - 2018 Qualcomm Technologies International, Ltd.

FILE NAME
    chain.c

DESCRIPTION
    Chain library implementation.
*/

#include "chain.h"
#include "chain_list.h"
#include "chain_stream.h"
#include "chain_connect.h"
#include "chain_config.h"
#include "chain_bundle_management.h"

#include <vmal.h> 
#include <panic.h>
#include <stream.h>
#include <stdlib.h>
#include <operators.h>
#include <print.h>
#include <string.h>
#include <audio_processor.h>

/* Helper to convert chain->operator_list into a contiguous list with no 
   INVALID_OPERATOR entries and execte a function eg. OperatorStartMultiple */
#define OP_MULTIPLE(func, chain) \
{ \
    Operator* list = PanicUnlessMalloc(chain->config->number_of_operators * sizeof(Operator)); \
    uint16 size_list = chainGetFilteredOperatorList(chain, list); \
    PanicFalse(func(size_list, list, NULL)); \
    free(list); \
}

/* Free all memory associated with the chain */
static void chainFreeMemory(kymera_chain_t *chain)
{
    if (chain != NULL)
    {
        free(chain->operator_list);
        free(chain->filters.operator_filters);
        free(chain);
    }
}

/* Allocate all memory associated with the chain */
static kymera_chain_t* chainAllocateMemory(const chain_config_t *config, const operator_filters_t* filter)
{
    kymera_chain_t *chain;
    
    if(!config)
        return NULL;
    
    chain = calloc(1, sizeof(*chain));
    
    if (chain)
    {
        chain->operator_list = calloc(config->number_of_operators, sizeof(Operator));
        
        if(chain->operator_list)
        {
            if(!filter)
                return chain;
            
            chain->filters.operator_filters = calloc(filter->num_operator_filters, sizeof(operator_config_t));
            if(chain->filters.operator_filters)
                return chain;
        }
        
        chainFreeMemory(chain);
    }
    
    return NULL;
}

/* Create and add all non-filtered operators to the chain */
static void chainAddOperators(kymera_chain_t *chain)
{
    unsigned i;
    const chain_config_t *config = chain->config;
    
    for (i = 0; i < config->number_of_operators; i++)
    {
        const operator_config_t* op_config = chainConfigGetOperatorConfig(chain, i);
        if(op_config)
        {
            /* Operator role must be unique within the chain */
            PanicFalse(ChainGetOperatorByRole(chain, op_config->role) == INVALID_OPERATOR);
            chain->operator_list[i] = OperatorsCreateWithSetup(op_config->capability_id, op_config->processor_id, op_config->priority, &op_config->setup);
        }
    }
}

/* Get contiguous list of operators in the chain with INVALID_OPERATORs removed */
static uint16 chainGetFilteredOperatorList(kymera_chain_t* chain, Operator* list)
{
    unsigned i;
    uint16 num_operator_filters = 0;
    
    for (i = 0; i < chain->config->number_of_operators; i++)
    {
        if(chain->operator_list[i] != INVALID_OPERATOR)
        {
            list[num_operator_filters] = chain->operator_list[i];
            num_operator_filters++;
        }
    }
    
    return num_operator_filters;
}

/* Start any required DSP processors, main processor is always required to be on */
static void processorsEnable(kymera_chain_t *chain)
{
    PanicFalse(chain->chain_enabled == FALSE);

    PanicFalse(VmalOperatorFrameworkEnableMainProcessor(TRUE));

    if (chainConfigUsesSecondProcessor(chain))
    {
        PanicFalse(VmalOperatorFrameworkEnableSecondProcessor(TRUE));
    }

    chain->chain_enabled = TRUE;
}

/* Stop any required DSP processors */
static void processorsDisable(kymera_chain_t *chain)
{
    PanicFalse(chain->chain_enabled == TRUE);

    if (chainConfigUsesSecondProcessor(chain))
    {
        PanicFalse(VmalOperatorFrameworkEnableSecondProcessor(FALSE));
    }
    PanicFalse(VmalOperatorFrameworkEnableMainProcessor(FALSE));

    chain->chain_enabled = FALSE;
}

/******************************************************************************/
kymera_chain_handle_t ChainCreate(const chain_config_t *config)
{
    return ChainCreateWithFilter(config, NULL);
}

/******************************************************************************/
kymera_chain_handle_t ChainCreateWithFilter(const chain_config_t *config, const operator_filters_t* filter)
{
    kymera_chain_t *chain;
    
    chain = chainAllocateMemory(config, filter);
    
    if(!chain)
        return NULL;
    
    chainConfigStore(chain, config, filter);
    
    if(chainConfigIsWholeChainFiltered(chain))
    {
        chainFreeMemory(chain);
        return NULL;
    }

    chain->chain_enabled = FALSE;

    processorsEnable(chain);
    ChainCapabilityBundlesLoad(chain);
    chainListAdd(chain);
    chainAddOperators(chain);
    AudioProcessorAddUseCase(config->audio_ucid);

    PRINT(("ChainCreate() 0x%x\n", (unsigned)chain));

    return chain;
}

/******************************************************************************/
void ChainDestroy(kymera_chain_handle_t handle)
{
    kymera_chain_t *chain = handle;
    PRINT(("ChainDestroy() 0x%x\n", (unsigned)chain));

    if(chain != NULL)
    {
        OP_MULTIPLE(OperatorDestroyMultiple, chain);
        
        AudioProcessorRemoveUseCase(chain->config->audio_ucid);
        chainListRemove(chain);
        ChainCapabilityBundlesUnload(chain);
        processorsDisable(chain);
        chainFreeMemory(chain);
    }
}

/******************************************************************************/
Operator ChainGetOperatorByRole(kymera_chain_handle_t handle, unsigned operator_role)
{
    kymera_chain_t *chain = handle;
    if(chain)
    {
        const chain_config_t *config = chain->config;
        unsigned i;
        for(i = 0; i < config->number_of_operators; ++i)
        {
            if(config->operator_config[i].role == operator_role)
            {
                return chain->operator_list[i];
            }
        }
    }

    /* No match */
    return INVALID_OPERATOR;
}

/******************************************************************************/
Sink ChainGetInput(kymera_chain_handle_t handle, unsigned input_role)
{
    kymera_chain_t *chain = handle;
    Sink sink = NULL;

    PRINT(("Get Input %d\n", input_role));
    if(chain)
    {
        if(chainConfigIsStreamBased(chain))
        {
            sink = chainStreamGetInput(chain, input_role);
        }
        else
        {
            sink = chainConnectGetInput(chain, input_role);
        }
    }
    return sink;
}

/******************************************************************************/
Source ChainGetOutput(kymera_chain_handle_t handle, unsigned output_role)
{
    kymera_chain_t *chain = handle;
    Source source = NULL;

    PRINT(("Get Output %d\n", output_role));
    if(chain)
    {
        if(chainConfigIsStreamBased(chain))
        {
            source = chainStreamGetOutput(chain, output_role);
        }
        else
        {
            source = chainConnectGetOutput(chain, output_role);
        }
    }
    return source;
}

/******************************************************************************/
void ChainConnect(kymera_chain_handle_t handle)
{
    kymera_chain_t *chain = handle;
    PanicNull(chain);
    
    if(chainConfigIsStreamBased(chain))
    {
        chainStreamConnect(chain);
    }
    else
    {
        chainConnectAllOperators(chain);
    }
}

bool ChainConnectInput(kymera_chain_handle_t handle, Source source, unsigned input_role)
{
    return !!StreamConnect(source, ChainGetInput(handle, input_role));
}

void ChainConnectOutput(kymera_chain_handle_t handle, Sink sink, unsigned output_role)
{
    PanicNull(StreamConnect(ChainGetOutput(handle, output_role), sink));
}

/******************************************************************************/
void ChainStart(kymera_chain_handle_t handle)
{
    kymera_chain_t *chain = handle;
    PanicNull(chain);
    
    PRINT(("ChainStart() 0x%x\n", (unsigned)chain));
    
    OP_MULTIPLE(OperatorStartMultiple, chain);
}

/******************************************************************************/
void ChainStop(kymera_chain_handle_t handle)
{
    kymera_chain_t *chain = handle;

    PanicNull(chain);
    PRINT(("ChainStop() 0x%x\n", (unsigned)chain));

    OP_MULTIPLE(OperatorStopMultiple, chain);
}

/******************************************************************************/
void ChainJoin(kymera_chain_handle_t source_chain, kymera_chain_handle_t sink_chain, unsigned count, const chain_join_roles_t *connect_list)
{
    unsigned i;
    Source output_src;
    Sink input_snk;

    PanicNull(source_chain);
    PanicNull(sink_chain);

    for (i = 0; i < count; i++)
    {
        output_src = ChainGetOutput(source_chain, connect_list[i].source_role);
        input_snk = ChainGetInput(sink_chain, connect_list[i].sink_role);
        PanicNull(StreamConnect(output_src, input_snk));
    }
}

void ChainConfigure(kymera_chain_handle_t handle, const chain_operator_message_t *messages,  unsigned number_of_messages)
{
    kymera_chain_t *chain = handle;
    if (chain && messages)
    {
        const chain_operator_message_t *msg;
        for (msg = messages; msg < messages + number_of_messages; msg++)
        {
            Operator op = ChainGetOperatorByRole(chain, msg->operator_role);
            PanicFalse(VmalOperatorMessage(op, msg->message, msg->message_length, NULL, 0));
        }
    }
}

/******************************************************************************/
static bool isRoleExcluded(unsigned role, const unsigned *excluded_roles, unsigned excluded_roles_count)
{
    unsigned i;

    for(i = 0; i < excluded_roles_count; i++)
    {
        if(excluded_roles[i] == role)
        {
            return TRUE;
        }
    }
    return FALSE;
}

/******************************************************************************/
void ChainConfigureSampleRate(kymera_chain_handle_t handle, uint32 sample_rate, const unsigned *excluded_roles, unsigned excluded_roles_count)
{
    kymera_chain_t *chain = handle;
    unsigned i;

    if(chain)
    {
        for(i = 0; i < chain->config->number_of_operators; i++)
        {
            Operator op;
            const operator_config_t* op_config = chainConfigGetOperatorConfig(chain, i);
            
            if(!op_config)
                continue;
            
            if(excluded_roles && isRoleExcluded(op_config->role, excluded_roles, excluded_roles_count))
                continue;
            
            op = ChainGetOperatorByRole(handle, op_config->role);
            OperatorsStandardSetSampleRate(op, sample_rate);
            OperatorsStandardSetBufferSizeFromSampleRate(op, sample_rate, &op_config->setup);
        }
    }
}

/******************************************************************************/
const Operator *ChainGetOperatorList(kymera_chain_handle_t handle, unsigned *count)
{
    const Operator *list = NULL;
    *count = 0;
    if (handle != NULL)
    {
        list = handle->operator_list;
        *count = handle->config->number_of_operators;
    }
    return list;
}

/******************************************************************************/
audio_ucid_t ChainGetUseCase(const chain_config_t *config)
{
    if(config)
    {
        return config->audio_ucid;
    }
    
    return audio_ucid_number_of_ucids;
}

/******************************************************************************/
void ChainWake(const kymera_chain_handle_t chain)
{
    processorsEnable(chain);
}

/******************************************************************************/
void ChainSleep(const kymera_chain_handle_t chain)
{
    processorsDisable(chain);
}


