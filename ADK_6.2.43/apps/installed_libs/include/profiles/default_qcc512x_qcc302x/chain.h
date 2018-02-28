/****************************************************************************
Copyright (c) 2016 - 2017 Qualcomm Technologies International, Ltd.

FILE NAME
    chain.h

DESCRIPTION
    Purpose of this library is to manage a chain of operators.
    After a chain is created, configured and connected internally
    it can then be connected to external inputs and outputs,
    started, stopped and destroyed.

    The chain is created using ChainCreate().
    Creation:
        * allocates resources the chain
        * starts any DSP processors required by the chain
        * loads any downloadable capability bundles required by the chain (if not already loaded)
        * creates the operators in the chain

    Configuration of the operators is then performed _outside_ the library. The chain Operators may be
    retrieved using the function ChainGetOperatorByRole(). Each Operator's role must be unique.

    To connect the chain to the outside world, the chain input sinks and output sources can be retrieved
    using the functions ChainGetInput() and ChainGetOutput().

    The chain may then be started and stopped using ChainStart() and ChainStop().

    Finally the chain may be destroyed using ChainDestroy().
    Destruction:
        * stops the operators in the chain
        * unloads any downloadable capability bundles no longer required by the chain or
          any other chains.
        * stops any DSP processor no longer used by the chain or any other chains.
        * frees sources allocated by the chain.

    Example configuration of 3 operators with one input and two outputs:
    const operator_config_t ops[] = {
                MAKE_OPERATOR_CONFIG(capability_id_rtp, RTP_ROLE),
                MAKE_OPERATOR_CONFIG(capability_id_sbc_decoder, SBC_ROLE),
                MAKE_OPERATOR_CONFIG(capability_id_iir_resampler, RESAMPLER_ROLE)
    };
    const operator_endpoint_t inputs[] = {{RTP_ROLE, ENCODED_DATA_INPUT_ROLE, 1}};
    const operator_endpoint_t outputs[] = {{RESAMPLER_ROLE, PCM_DATA_OUT_L_ROLE, 0},
                                           {RESAMPLER_ROLE, PCM_DATA_OUT_R_ROLE, 1}};
    const operator_connection_t connections[] = {
            {RTP_ROLE, 0, SBC_ROLE, 0, 1},
            {SBC_ROLE, 0, RESAMPLER_ROLE, 0, 2}
    };
    const chain_config_t config = MAKE_CHAIN_CONFIG(chain_id_1, ops, inputs, outputs, connections);

    Removing or replacing operators is not supported.

    The chain library will load and unload capability bundles dynamically at runtime,
    loading the bundle before the first time an operator contained in the bundle
    is created and unloading the bundle after the final operator contained in the
    bundle is destroyed. The downloadable capability bundle config must be set
    using ChainSetDownloadableCapabilityBundleConfig() before creating any chains that
    use downloadable operators.
*/

#ifndef LIBS_CHAIN_CHAIN_H_
#define LIBS_CHAIN_CHAIN_H_

#include <csrtypes.h>
#include <sink.h>
#include <source.h>
#include <operators.h>
#include <vmtypes.h>
#include <hydra_macros.h>
#include <audio_ucid.h>

typedef enum _chain_id
{
    chain_id_none,
    chain_id_audio_input_a2dp,
    chain_id_audio_input_i2s,
    chain_id_audio_input_analogue,
    chain_id_audio_input_common_forwarding,
    chain_id_audio_input_tws,
    chain_id_audio_input_usb,
    chain_id_audio_input_voice_prompts_tone,
    chain_id_audio_input_voice_prompts_decoder,
    chain_id_audio_input_voice_prompts_dummy,
    chain_id_audio_input_spdif,
    chain_id_audio_input_broadcast,
    chain_id_audio_hardware_aec,
    chain_id_audio_mixer_core,
    chain_id_audio_mixer_voice_resampler,
    chain_id_audio_mixer_music_resampler,
    chain_id_audio_mixer_prompt_resampler,
    chain_id_audio_mixer_voice_processing,
    chain_id_audio_mixer_music_processing,
    chain_id_audio_mixer_prompt_processing,
    chain_id_audio_mixer_speaker,
    chain_id_cvc_common,
    chain_id_audio_output_tws,
    chain_id_aov,
    chain_id_tuning_mode,
    chain_id_va,
    FORCE_ENUM_TO_MIN_16BIT(chain_id_t),
} chain_id_t;

typedef struct
{
    capability_id_t capability_id;
    unsigned role;
    operator_processor_id_t processor_id;
    operator_priority_t priority;
    operator_setup_t setup;
} operator_config_t;

typedef struct
{
    unsigned operator_role;
    unsigned endpoint_role;
    unsigned terminal;
} operator_endpoint_t;

typedef struct
{
    unsigned source_role;
    unsigned first_source_terminal;

    unsigned sink_role;
    unsigned first_sink_terminal;

    unsigned number_of_terminals;
} operator_connection_t;

typedef enum
{
    stream_with_no_in_or_out = 0,
    stream_with_input        = 1 << 0,
    stream_with_output       = 1 << 1,
    stream_with_in_and_out   = (stream_with_input | stream_with_output)
} stream_type_t;

typedef struct
{
    unsigned operator_role;
    unsigned input_terminal;
    unsigned output_terminal;
} operator_stream_node_t;

typedef struct
{
    unsigned stream_role;
    stream_type_t type;
    unsigned number_of_nodes;
    const operator_stream_node_t* nodes;
} operator_stream_t;

typedef struct
{
    unsigned num_operator_filters;
    const operator_config_t* operator_filters;
} operator_filters_t;

/* Note that there is no support for messages with response */
typedef struct
{
    /* The operator role to which the message will be sent */
    unsigned operator_role;
    /* The message content */
    const uint16 *message;
    /* The length of the message in uint16 units */
    const uint16 message_length;
} chain_operator_message_t;

typedef struct
{
    chain_id_t chain_id;
    audio_ucid_t audio_ucid;
    const operator_config_t *operator_config;
    unsigned number_of_operators;
    const operator_endpoint_t *inputs;
    unsigned number_of_inputs;
    const operator_endpoint_t *outputs;
    unsigned number_of_outputs;
    const operator_connection_t *connections;
    unsigned number_of_connections;
    const operator_stream_t *streams;
    unsigned number_of_streams;
} chain_config_t;

typedef struct _chain_join_roles
{
    unsigned source_role;
    unsigned sink_role;
} chain_join_roles_t;

typedef struct kymera_chain_tag * kymera_chain_handle_t;

/*! The processors on which a downloadable capabilities bundle is available */
typedef enum
{
    /*! The bundle is only available on audio processor 0 */
    capability_bundle_available_p0 = 0,
    /*! The bundle is available on both audio processors */
    capability_bundle_available_p0_and_p1 = 1,
    FORCE_ENUM_TO_MIN_16BIT(capability_bundle_processor_availability_t)
} capability_bundle_processor_availability_t;

/*! A structure to define a downloadable capability bundle.
   Each bundle has a file name, and contains a number of downloadable operators */
typedef struct
{
    /*! The file name of the bundle */
    const char* file_name;
    /*! The processors for which the downloadable capabilities are to be made available.
        The bundle may be made available on audio processor 0 or both audio processor. */
    capability_bundle_processor_availability_t processors;
} capability_bundle_t;

/*! A structure to define the configuration of a group of bundles */
typedef struct
{
    /*! Pointer to an array of capability bundles */
    const capability_bundle_t *capability_bundles;
    /*! The number of capability bundles in the array */
    unsigned number_of_capability_bundles;
} capability_bundle_config_t;

#define MAKE_CHAIN_CONFIG(id, ucid, operators, inputs, outputs, connections) { \
    (id), \
    (ucid), \
    (operators), ARRAY_DIM((operators)), \
    (inputs), ARRAY_DIM((inputs)), \
    (outputs), ARRAY_DIM((outputs)), \
    (connections), ARRAY_DIM((connections)), \
    NULL, 0}

#define MAKE_CHAIN_CONFIG_NO_CONNECTIONS(id, ucid, operators, inputs, outputs) { \
    (id), \
    (ucid), \
    (operators), ARRAY_DIM((operators)), \
    (inputs), ARRAY_DIM((inputs)), \
    (outputs), ARRAY_DIM((outputs)), \
    NULL, 0, \
    NULL, 0}

#define MAKE_CHAIN_CONFIG_NO_INPUTS_NO_CONNECTIONS(id, ucid, operators, outputs) { \
    (id), \
    (ucid), \
    (operators), ARRAY_DIM((operators)), \
    NULL, 0, \
    (outputs), ARRAY_DIM((outputs)), \
    NULL, 0, \
    NULL, 0}

#define MAKE_CHAIN_CONFIG_WITH_STREAMS(id, ucid, operators, streams) { \
    (id), \
    (ucid), \
    (operators), ARRAY_DIM((operators)), \
    NULL, 0, \
    NULL, 0, \
    NULL, 0, \
    streams, ARRAY_DIM((streams))}

#define MAKE_CHAIN_CONFIG_NO_OUTPUTS(id, ucid, operators, inputs, connections) { \
    (id), \
    (ucid), \
    (operators), ARRAY_DIM((operators)), \
    (inputs), ARRAY_DIM((inputs)), \
    NULL, 0, \
    (connections), ARRAY_DIM((connections)), \
    NULL, 0}

/* An operator_setup_t struct with no items */
#define NO_OPERATOR_SETUP {0, NULL}

/* Make an operator_setup_t structure using setup_items */
#define MAKE_OPERATOR_SETUP(setup_items) { ARRAY_DIM((setup_items)), setup_items }

/* Make a default operator config. By default, the operator will be created on P0 */
#define MAKE_OPERATOR_CONFIG(cap_id, role) { (cap_id), (role), OPERATOR_PROCESSOR_ID_0, DEFAULT_OPERATOR_PRIORITY, NO_OPERATOR_SETUP }

/* Same as MAKE_OPERATOR_CONFIG, but on processor 1 */
#define MAKE_OPERATOR_CONFIG_P1(cap_id, role) { (cap_id), (role), OPERATOR_PROCESSOR_ID_1, DEFAULT_OPERATOR_PRIORITY, NO_OPERATOR_SETUP }

/* Same as MAKE_OPERATOR_CONFIG, but high priority */
#define MAKE_OPERATOR_CONFIG_PRIORITY_HIGH(cap_id, role) { (cap_id), (role), OPERATOR_PROCESSOR_ID_0, operator_priority_high, NO_OPERATOR_SETUP }

/* Same as MAKE_OPERATOR_CONFIG, but low priority */
#define MAKE_OPERATOR_CONFIG_PRIORITY_LOW(cap_id, role) { (cap_id), (role), OPERATOR_PROCESSOR_ID_0, operator_priority_low, NO_OPERATOR_SETUP }

/* Same as MAKE_OPERATOR_CONFIG, but lowest priority */
#define MAKE_OPERATOR_CONFIG_PRIORITY_LOWEST(cap_id, role) { (cap_id), (role), OPERATOR_PROCESSOR_ID_0, operator_priority_lowest, NO_OPERATOR_SETUP }

/* Same as MAKE_OPERATOR_CONFIG, but includes parameters to set up following operator creation */
#define MAKE_OPERATOR_CONFIG_WITH_SETUP(cap_id, role, setup_items) { (cap_id), (role), OPERATOR_PROCESSOR_ID_0, DEFAULT_OPERATOR_PRIORITY, MAKE_OPERATOR_SETUP(setup_items) }

/* Same as MAKE_OPERATOR_CONFIG, but on processor 1 */
#define MAKE_OPERATOR_CONFIG_P1_WITH_SETUP(cap_id, role, setup_items) { (cap_id), (role), OPERATOR_PROCESSOR_ID_1, DEFAULT_OPERATOR_PRIORITY, MAKE_OPERATOR_SETUP(setup_items) }

/* Same as MAKE_OPERATOR_CONFIG_WITH_SETUP, but high priority */
#define MAKE_OPERATOR_CONFIG_PRIORITY_HIGH_WITH_SETUP(cap_id, role, setup_items) { (cap_id), (role), OPERATOR_PROCESSOR_ID_0, operator_priority_high, MAKE_OPERATOR_SETUP(setup_items) }

/****************************************************************************
DESCRIPTION
    Set the downloadable capability bundle configuration. An example configuration:
    const capability_bundle_t bundles[] = {{"bundle_file_1.dkcs",
                                            capability_bundle_available_p0};
                                           {"bundle_file_2.dkcs",
                                            capability_bundle_available_p0}};
    const capability_bundle_config_t bundle_config = {bundles, ARRAY_DIM(bundles)};

    The chain library just saves the pointer passed to this function, it does
    not copy the configuration. Therefore the caller must ensure the configuration
    is valid for the lifetime of the chain libraries use of the configuration.

    There is no unset function. Just call this function with a NULL argument to
    clear the configuration used by the library.
*/
void ChainSetDownloadableCapabilityBundleConfig(const capability_bundle_config_t *config);

/****************************************************************************
DESCRIPTION
    Create a chain as defined by the config.

    The caller must ensure that the chain_config_t pointed to by config, exists
    until the chain is destroyed using ChainDestroy().

    The chain_config_t should use the streams parameter to set up the chain.
    Each stream entry has an associated role which is used to get inputs and
    outputs. When ChainConnect is called nodes in each stream are connected in
    the order they are listed in the stream, nodes[0].output_terminal to
    nodes[1].input_terminal and so on.

    The legacy parameters inputs, outputs and connections may be omitted when a
    valid streams configuration is provided.
*/
kymera_chain_handle_t ChainCreate(const chain_config_t *config);

/****************************************************************************
DESCRIPTION
    As ChainCreate but with the option to modify or remove some operators using
    a filter.

    The filter consists of a list of operator_config_t configurations.

    If the role of an operator_config_t from the filter matches that of one from
    the chain_config_t, then the operator_config_t from the filter replaces the
    one from the chain_config_t.

    If the role of an operator_config_t from the filter matches that of one from
    the chain_config_t and the capability_id is set to capability_id_none, then
    the operator_config_t from the chain_config_t is not created. Any nodes in
    the chain_config_t streams which rely on the operator in question will be
    bypassed and connected to the next node with a valid operator. The same
    applies when calling ChainGetInput or ChainGetOutput, which will use the
    first and last valid operators in the stream respectively.

    Attempting to use filters with a legacy chain configuration which does not
    use streams will result in a Panic as it is not supported.
*/
kymera_chain_handle_t ChainCreateWithFilter(const chain_config_t *config, const operator_filters_t* filter);


/****************************************************************************
DESCRIPTION
    Destroy the chain.
*/
void ChainDestroy(kymera_chain_handle_t handle);

/****************************************************************************
DESCRIPTION
    Retrieve operator by its operator_role.
*/
Operator ChainGetOperatorByRole(const kymera_chain_handle_t handle, unsigned operator_role);

/****************************************************************************
DESCRIPTION
    Get the chain input. Can be used to connect voice prompt source for example.
*/
Sink ChainGetInput(kymera_chain_handle_t handle, unsigned input_role);

/****************************************************************************
DESCRIPTION
    Get the chain output.
*/
Source ChainGetOutput(kymera_chain_handle_t handle, unsigned output_role);

/****************************************************************************
DESCRIPTION
    Make connections between operators as defined by the ChainCreate config.
*/
void ChainConnect(kymera_chain_handle_t handle);

/****************************************************************************
DESCRIPTION
    Connect a source to a chain input.
*/
bool ChainConnectInput(kymera_chain_handle_t handle, Source source, unsigned input_role);

/****************************************************************************
DESCRIPTION
    Connect a chain output to a sink.
*/
void ChainConnectOutput(kymera_chain_handle_t handle, Sink sink, unsigned output_role);

/****************************************************************************
DESCRIPTION
    Start the chain.
*/
void ChainStart(kymera_chain_handle_t handle);

/****************************************************************************
DESCRIPTION
    Stop the chain.
*/
void ChainStop(kymera_chain_handle_t handle);

/*******************************************************************************
DESCRIPTION
    Connect the outputs of one chain to the inputs of another.
*/
void ChainJoin(kymera_chain_handle_t source_chain, kymera_chain_handle_t sink_chain, unsigned count, const chain_join_roles_t *connect_list);

/*******************************************************************************
DESCRIPTION
    Configure the chain with the messages. The messages are not part of the configuration
    as the required messages are likely to change dynamically.
*/
void ChainConfigure(kymera_chain_handle_t handle, const chain_operator_message_t *messages,  unsigned number_of_messages);

/*******************************************************************************
DESCRIPTION
    Sets sample rate for all operators in the chain,
    except for ones from the excluded list.
*/
void ChainConfigureSampleRate(kymera_chain_handle_t handle, uint32 sample_rate, const unsigned *excluded_roles, unsigned excluded_roles_count);

/*******************************************************************************
DESCRIPTION
    Find a chain by the specified ID and index
    This is only intended for unit test and will panic if called in a release
    build.
*/
kymera_chain_handle_t ChainFindById(chain_id_t id, unsigned index);

/*******************************************************************************
DESCRIPTION
    Get the list of operators within a chain
*/
const Operator *ChainGetOperatorList(kymera_chain_handle_t handle, unsigned *count);

/*******************************************************************************
DESCRIPTION
    Get the list of operator roles within a chain
*/
const chain_config_t *ChainGetConfig(kymera_chain_handle_t handle);

/*******************************************************************************
DESCRIPTION
    Get the use-case for the chain.
*/
audio_ucid_t ChainGetUseCase(const chain_config_t *config);

/*******************************************************************************
DESCRIPTION
    Allows the DSP to enter low power mode. For use with chains that
    support low power mode only. Do not use for other chains.
*/
void ChainSleep(const kymera_chain_handle_t chain);

/*******************************************************************************
DESCRIPTION
    Brings the DSP out of low power mode. For use with chains that
    support low power mode only. Do not use for other chains.
*/
void ChainWake(const kymera_chain_handle_t chain);

/*******************************************************************************
DESCRIPTION
    Check if the internal list of chains is empty. This is intended for use by
    unit tests.
*/
bool ChainIsListEmpty(void);

/****************************************************************************
DESCRIPTION
    Reset any static variables
    This is only intended for unit test and will panic if called in a release
    build.
*/
void ChainTestReset(void);

#endif /* LIBS_CHAIN_CHAIN_H_ */
