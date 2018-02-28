/****************************************************************************
Copyright (c) 2017 Qualcomm Technologies International, Ltd.

FILE NAME
    chain_stream.c

DESCRIPTION
    Functions to parse streams
*/

#include <stream.h>

#include "chain_stream.h"
#include "chain_connect.h"

#define for_all_streams(config, stream) for (stream = config->streams; stream < (config->streams + config->number_of_streams); stream++)

#define isNodeValid(stream, node)  (node < (stream->nodes + stream->number_of_nodes))
#define isNodeCreated(chain, node) (node && (ChainGetOperatorByRole(chain, node->operator_role) != INVALID_OPERATOR))

/******************************************************************************/
static const operator_stream_node_t* getNextCreatedNode(kymera_chain_t* chain, const operator_stream_t* stream, const operator_stream_node_t* node)
{
    const operator_stream_node_t* next_node;

    if(isNodeValid(stream, node))
    {
        for(next_node = node + 1; isNodeValid(stream, next_node); next_node++)
        {
            if(isNodeCreated(chain, next_node))
                return next_node;
        }
    }
    return NULL;
}

/******************************************************************************/
static const operator_stream_node_t* getFirstCreatedNode(kymera_chain_t* chain, const operator_stream_t* stream)
{
    const operator_stream_node_t* first_node = stream->nodes;

    if(isNodeCreated(chain, first_node))
        return first_node;

    return getNextCreatedNode(chain, stream, first_node);
}

/******************************************************************************/
static const operator_stream_node_t* getLastCreatedNode(kymera_chain_t* chain, const operator_stream_t* stream)
{
    const operator_stream_node_t* last_node = getFirstCreatedNode(chain, stream);
    const operator_stream_node_t* next_node = getNextCreatedNode(chain, stream, last_node);

    while(next_node)
    {
        last_node = next_node;
        next_node = getNextCreatedNode(chain, stream, last_node);
    }

    return last_node;
}

/******************************************************************************/
Sink chainStreamGetInput(kymera_chain_t *chain, unsigned input_role)
{
    if(chain)
    {
        const chain_config_t *config = chain->config;

        if(config->streams)
        {
            const operator_stream_t* stream;

            for_all_streams(config, stream)
            {
                if(stream->stream_role == input_role && (stream->type & stream_with_input))
                {
                    const operator_stream_node_t* node = getFirstCreatedNode(chain, stream);
                    if (node)
                    {
                        Operator op = ChainGetOperatorByRole(chain, node->operator_role);
                    return StreamSinkFromOperatorTerminal(op, (uint16)node->input_terminal);
                    }
                }
            }
        }
    }
    return NULL;
}

/******************************************************************************/
Source chainStreamGetOutput(kymera_chain_t *chain, unsigned output_role)
{
    if(chain)
    {
        const chain_config_t *config = chain->config;

        if(config->streams)
        {
            const operator_stream_t* stream;

            for_all_streams(config, stream)
            {
                if(stream->stream_role == output_role && (stream->type & stream_with_output))
                {
                    const operator_stream_node_t* node = getLastCreatedNode(chain, stream);

                    Operator op = ChainGetOperatorByRole(chain, node->operator_role);
                    return StreamSourceFromOperatorTerminal(op, (uint16)node->output_terminal);
                }
            }
        }
    }
    return NULL;
}

/******************************************************************************/
void chainStreamConnect(kymera_chain_t *chain)
{
    const chain_config_t *config;

    if(!chain)
        return;

    config = chain->config;

    if(config->streams)
    {
        const operator_stream_t* stream;

        for_all_streams(config, stream)
        {
            const operator_stream_node_t* node = getFirstCreatedNode(chain, stream);

            while(node)
            {
                const operator_stream_node_t* next_node = getNextCreatedNode(chain, stream, node);

                if(next_node)
                {
                    chainConnectOperatorTerminals(chain,
                                                        node->operator_role,
                                                        node->output_terminal,
                                                        next_node->operator_role,
                                                        next_node->input_terminal);
                }

                node = next_node;
            }
        }
    }
}
