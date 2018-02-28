/* Copyright (c) 2016 - 2017 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */
/****************************************************************************
FILE
    prodtestpdu.h  -  Production test packet structure

DESCRIPTION
    This file explains the protocol used for production testing. It has no
    dependency on transport (TRB/USB) and expects the command in the following
    format:
    PROD_TEST_CMD_ID | PADDING | PAYLOAD LEN | SEQ_NUM |  STATUS  |  PAYLOAD  |
        1 byte       | 1 byte  |   2 bytes   | 2 bytes |  2 bytes |  N bytes  |

....The payload is defined by prod test generic command category. If command is
    MIB then payload should follow MIB command structure. If command is
    bccmd then it should follow bccmdpdu structure.

    APPS0 commands (i.e LED, XIO, ADC, PIO etc) are defined in respective
    structures in this file.

    So if prod test generic command is APPS0 then following command structure
    is used as payload:

      APPS0_CMD_ID   |  LOCAL CMD PAYLOAD  |
        1 byte       |       n Bytes       |

    APPS0 commands payload are defined in this file
    For example: If user wants to map PIO23, then following command should be
    used:

    PROD_TEST_COMMAND_ID    = 0x03
    Padding                 = 0x00
    Payload Length          = 0x0010
    seq_num                 = 0x0000
    status                  = 0x0000 (placeholder for command status)
    APPS0_COMMAND_ID        = 0x00 (APPS0_COMMAND_ID_PIO)
    pio_cmd ----------------= 0x01 (SET_MAP)
    bank -------------------= 0x0000
    mask -------------------= 0x00800000
    value ------------------= 0x00800000

    When device sends response back to Host it uses similar format as used for
    request. For example, if user wants to map PIO23; then device will respond
    back with command status (PROD_TEST_COMMAND_SUCCESS/
    PROD_TEST_COMMAND_FAILURE) and response value, after PIO is configured, with
    the following format.

    PROD_TEST_COMMAND_ID    = 0x03
    Padding                 = 0x00
    Payload Length          = 0x0010
    seq_num                 = 0x0000
    status                  = 0x0000 (command success)
    APPS0_COMMAND_ID        = 0x00 (APPS0_COMMAND_ID_PIO)
    pio_cmd                 = 0x01 (SET_MAP)
    Padding                 = 0x0000
    value                   = 0x00000000
    Note: Value is a 32 bit mask. If any bit in this mask is high then that PIO
    could not be mapped or unmapped.

    If the command fails status is populated as PROD_TEST_COMMAND_FAILURE.
    Response paylaod will be sent if module has asked it to be sent.
    For example if above command fails, following will be the response format.

    PROD_TEST_COMMAND_ID    = 0x03
    Padding                 = 0x00
    Payload Length          = 0x0010
    seq_num                 = 0x0000
    status                  = 0xFFFF (command failure)
    APPS0_COMMAND_ID        = 0x00 (APPS0_COMMAND_ID_PIO)
    pio_cmd                 = 0x01 (SET_MAP)
    Padding                 = 0x0000
    value                   = 0xXXXXXXXX (Value sent by PIO as response)

    Note: 16-bit/32-bit integers have their bytes arranged in Little Endian
    format. This means that these commands are effectively a series of 8-bit
    octets and least significant byte need to be sent first.

    Note: PROD_TEST_COMMAND_ID_APP_DISABLE will disable P1 processor.
    The response of this command will be success. There is no payload in this
    command.
*/
#ifndef    __PRODTESTPDU_H__
#define    __PRODTESTPDU_H__

/* 
 * Production test version number for PDU format 
 * Version 1: ADK6.0.1
 *     - Original implementation. 
 * Version 2: ADK6.1
 *     - Addition to Charger parameters
 *     - Addition of SetPIOFunction to PIO command
 * Version 3: ADK6.2
 *     - Addition of Ps keys store and retrieve command
 *     - Addition of Ps Audio keys update and read command
 */
#define PROD_TEST_VERSION_NUM 3UL

typedef enum
{
   PROD_TEST_COMMAND_ID_CURATOR     = 0x0000,
   /* MIB is now part of CURATOR, 0x0001 can be reused for a new command. */
   /* NFC is now part of APPS0, 0x0002 can be reused for a new command. */
   PROD_TEST_COMMAND_ID_APPS0       = 0x0003,
   PROD_TEST_COMMAND_ID_APP_DISABLE = 0x0004,
   PROD_TEST_COMMAND_ID_INVALID     = 0x0005
} PROD_TEST_COMMAND_ID;

/*
 * Commands status (success/failure)
 */
typedef enum
{
    PROD_TEST_COMMAND_SUCCESS   = 0x0000,
    PROD_TEST_COMMAND_FAILURE   = 0xFFFF
} PROD_TEST_COMMAND_STATUS;

typedef enum
{
   APPS0_COMMAND_ID_PIO               = 0x0000,
   APPS0_COMMAND_ID_XIO               = 0x0001,
   APPS0_COMMAND_ID_LED               = 0x0002,
   APPS0_COMMAND_ID_ADC               = 0x0003,
   APPS0_COMMAND_ID_FS_READ           = 0x0004,
   APPS0_COMMAND_ID_FS_WRITE          = 0x0005,
   APPS0_COMMAND_ID_LICENSE           = 0X0006,
   APPS0_COMMAND_ID_USB               = 0x0007,
   APPS0_COMMAND_ID_FW_VER            = 0x0008,
   APPS0_COMMAND_ID_CAPACITIVE_SENSOR = 0x0009,
   APPS0_COMMAND_ID_NFC               = 0x000a,
   APPS0_COMMAND_ID_PS                = 0x000b,
   APPS0_COMMAND_ID_INVALID           = 0x000c
} APPS0_COMMAND_ID;

/*
 * Commands handled in PROD_TEST_COMMAND_ID_CURATOR category
 */
typedef enum
{
    CURATOR_COMMAND_ID_MIB      = 0x0000,
    CURATOR_COMMAND_ID_INVALID  = 0x0001
} CURATOR_COMMAND_ID ;


/*
 * Commands supported by PIO function.
 */
typedef enum
{
    PIO_MAP          = 0x0000,
    PIO_GET          = 0x0001,
    PIO_SET          = 0x0002,
    PIO_GET_DIR      = 0x0003,
    PIO_SET_DIR      = 0x0004,
    PIO_SET_FUNCTION = 0x0005
} PIO_CMD_ID;

/*
 * Functions supported by PIO_SET_FUNCTION.
 */
typedef enum
{
    PIO_LED          = 0x0000,
    PIO_XIO_ANALOGUE = 0x0001,
    PIO_OTHER        = 0x0002
} PIO_FUNCTION_ID;

/*
 * Commands supported by XIO function.
 */
typedef enum
{
    XIO_MEASURE_VOLTAGE = 0x0000
} XIO_CMD_ID;

/*
 * Commands supported by LED function.
 */
typedef enum
{
    LED_CONFIG              = 0x0000
} LED_CMD_ID;

/*
 * Commands supported by ADC function.
 */
typedef enum
{
    ADC_READ_REQUEST = 0x0000
} ADC_CMD_ID;

/*
 * MIB operations
 */
typedef enum
{
    MIB_SET     = 0x0000
} MIB_OPERATION_ID;

/*
 * USB Device and charger status
 */
typedef enum
{
    USB_CHGR_CONFIGURE      = 0x0000,
    USB_CHGR_STATUS         = 0x0002
} USB_CMD_ID;

/*!
 * Commands supported by NFC function.
 */
typedef enum
{
    NFC_MSG_CONFIG_TT2_NDEF_TEXT = 0x0000,
    NFC_MSG_DECONFIGURE_NFC      = 0x0001,
    NFC_MSG_MAX                  = 0x0002 /* Max message supported by NFC handling */
} NFC_MSG_ID;

/*
 * Commands supported by PS function.
 */
typedef enum
{
    PS_STORE = 0x0000,
    PS_RETRIEVE = 0x0001,
    PS_UPDATE_AUDIO = 0x0002,
    PS_READ_AUDIO = 0x0003
} PS_CMD_ID;

/*
 * FW version
 */
typedef enum
{
    FW_VER_APPSP0       = 0x0000,
    FW_VER_APPSP1       = 0x0001,
    FW_VER_PROD_TEST    = 0x0002
} FW_VER_CMD_ID;

 /*
  * Commands supported by CapacitiveSensor function.
  */
typedef enum
{
    CAPACITIVE_SENSOR_ENABLE_PAD     = 0x0000,
    CAPACITIVE_SENSOR_QUERY_PAD      = 0x0001
} CAPACITIVE_SENSOR_CMD_ID;

/*
 * Commands supported by License
 */
typedef enum
{
    LICENSE_VALIDATE    = 0x0000
} LICENSE_CMD_ID;

typedef struct
{
    APPS0_COMMAND_ID    apps0_cmd;
    PIO_CMD_ID          pio_cmd;
    uint16              bank;   /**< Bank ID */
    uint32              mask;   /**< Bit mask indicating which PIOs to
                                    change or request information about */
    uint32              value;  /**< Depending on the PIO_CMD_ID, this value
                                    gets changed. For example: 
                                    if a pull resistor is
                                    enabled or going to be enabled (0-disable,
                                    1-enable), the current or requested pull
                                    direction (0-low, 1-high), current or
                                    requested pull strength (0-weak, 1-strong),
                                    if the sender is the PIO owner or a request
                                    for/release of PIO ownership.
                                    If Set Dir is requested then this will
                                    represent the dir value*/
} APPS0_PIO_CMD_REQ;

typedef struct
{
    APPS0_COMMAND_ID    apps0_cmd;
    PIO_CMD_ID          pio_cmd;
    uint16              padding;
    uint32              value;  /**< Bit mask indicating operation on which
                                    PIOs were success/failure
                                    (0-success, 1-failure) */
} APPS0_PIO_CMD_RESP;

typedef struct
{
    APPS0_COMMAND_ID    apps0_cmd;
    XIO_CMD_ID          xio_cmd;
    uint8               first_xio;      /* xio or first xio in difference pair
                                         * to use for input. Refer #xio_source
                                         * in voltsense_if.h */
    uint8               second_xio;     /* xio or second xio in difference pair
                                         * to use for input. Refer #xio_source
                                         * in voltsense_if.h */
    uint8               voltage_gain;   /* VGA value, 0.85, 1, 2, 4, 8. Refer
                                         * #vga_gain in voltsense_if.h */
} APPS0_XIO_CMD_REQ;

typedef struct
{
    APPS0_COMMAND_ID    apps0_cmd;
    XIO_CMD_ID          xio_cmd;
    int16               reading;    /*!< Reading derived from the hardware. */
} APPS0_XIO_CMD_RESP;

typedef struct
{
    APPS0_COMMAND_ID    apps0_cmd;
    LED_CMD_ID          led_cmd;
    uint8               led_id;
    uint8               key;
    uint16              value;
} APPS0_LED_CMD_REQ;

typedef struct
{
    APPS0_COMMAND_ID    apps0_cmd;
    LED_CMD_ID          led_cmd;
} APPS0_LED_CMD_RESP;

typedef struct
{
    APPS0_COMMAND_ID    apps0_cmd;
    ADC_CMD_ID          adc_cmd;
    uint8               adc_source; /* Refer #vm_adc_source_type in adc_if.h */
    uint8               extra_flag; /* Refer #vm_adc_extra_flag in adc_if.h */
    uint16              delay;
    /* Adc readings for various analogue sources may require a delay to let
     * external circuitry settle down into stable state and not impact the
     * reading (for example external capacitors need time to charge when the
     * internal current source is enabled).
     * This delay needs to be provided by customer where they see appropriate.
     */
     /* Note: Please refer adc.h for more details. */
} APPS0_ADC_CMD_REQ;

typedef struct
{
    APPS0_COMMAND_ID    apps0_cmd;
    ADC_CMD_ID          adc_cmd;
    uint16              reading;    /*!< Reading derived from the hardware. */
} APPS0_ADC_CMD_RESP;

typedef struct APPS0_FS_READ_REQ
{
    APPS0_COMMAND_ID    apps0_cmd;
} APPS0_FS_READ_REQ;

typedef struct APPS0_FS_READ_RESP
{
    APPS0_COMMAND_ID    apps0_cmd;
    /* The read data follows this structure in memory. */
} APPS0_FS_READ_RESP;

typedef struct APPS0_FS_WRITE_REQ
{
    APPS0_COMMAND_ID    apps0_cmd;
    /* The data to write follows this structure in memory */
} APPS0_FS_WRITE_REQ;

typedef struct APPS0_FS_WRITE_RESP
{
    APPS0_COMMAND_ID    apps0_cmd;
} APPS0_FS_WRITE_RESP;

typedef struct
{
    APPS0_COMMAND_ID    apps0_cmd;
    NFC_MSG_ID          nfc_cmd;
} APPS0_NFC_CMD_REQ;

typedef struct
{
    APPS0_COMMAND_ID    apps0_cmd;
    NFC_MSG_ID          nfc_cmd;
} APPS0_NFC_CMD_RESP;

typedef struct
{
    uint32              key;            /* 32 bit key ID */
    uint16              words;          /* number of words to read */
    uint16              offset;         /* offset from which to read (audio only) */
    uint16              key_length;     /* key length (audio only)*/
    uint16              padding;        /* padding as sizeof operator boundary rounds up to multiples of 4 */
    /* The first byte in the ps value array follows this structure in memory */
} PS_PAYLOAD;

typedef struct
{
    APPS0_COMMAND_ID    apps0_cmd;
    PS_CMD_ID           ps_cmd;
    uint16              padding;    /* For 32-bit alignment of payload struct */
    PS_PAYLOAD          ps_payload;
} APPS0_PS_CMD_REQ;

typedef struct
{
    APPS0_COMMAND_ID    apps0_cmd;
    PS_CMD_ID           ps_cmd;
    uint16              padding;    /* For 32-bit alignment of payload struct */
    PS_PAYLOAD          ps_payload;
} APPS0_PS_CMD_RESP;

typedef struct
{
    APPS0_COMMAND_ID    apps0_cmd;
    LICENSE_CMD_ID      lic_cmd;
    uint16              feature; /* This is feature_id enum from
                                  * app/feature/feature_if.h
                                  */
} APPS0_LICENSE_CMD_REQ;

typedef struct
{
    APPS0_COMMAND_ID    apps0_cmd;
    LICENSE_CMD_ID      lic_cmd;
    uint16              padding2;
    bool                result;
} APPS0_LICENSE_CMD_RESP;

typedef struct
{
    CURATOR_COMMAND_ID  cur_cmd;
    MIB_OPERATION_ID    op;
    uint16              mib_id;
    uint16              payload_len;
    uint8               payload[1]; /* Struct hack */
} CURATOR_MIB_CMD_REQ;

typedef struct
{
    CURATOR_COMMAND_ID  cur_cmd;
    MIB_OPERATION_ID    op;
} CURATOR_MIB_CMD_RESP;

typedef struct
{
    uint16              pre_external_milliohms;
    uint16              fast_external_milliohms;
    uint16              trickle_milliamps;
    uint16              pre_milliamps;
    uint16              fast_milliamps;
    uint16              pre_fast_threshold;
    uint16              float_constant_voltage;
    uint16              termination_current;
    uint16              termination_debounce;
    uint16              standby_fast_hysteris;
    uint8               charger_configure_type;
                        /* Above is of enum type CCP_CHARGER_CONFIGURE_TYPE */
    uint8               padding1;
    uint16              unused_expansion2;
    uint16              enable;
} USB_CHARGER_CONFIG;

typedef struct
{
    uint16              nothing;
} USB_CHARGER_STATUS;

typedef union
{
    USB_CHARGER_CONFIG      charger_config;
    USB_CHARGER_STATUS      charger_status;
} USB_PAYLOAD;

typedef struct
{
    APPS0_COMMAND_ID    apps0_cmd;
    USB_CMD_ID          usb_cmd;
    uint16              padding;    /* For 32-bit alignment of payload struct */
    USB_PAYLOAD         payload;
} APPS0_USB_CMD_REQ;

typedef struct
{
    APPS0_COMMAND_ID    apps0_cmd;
    USB_CMD_ID          usb_cmd;
    uint16              result;
} APPS0_USB_CMD_RESP;

typedef struct
{
    APPS0_COMMAND_ID    apps0_cmd;
    FW_VER_CMD_ID       fw_ver_cmd;
} APPS0_FW_VER_CMD_REQ;

typedef struct
{
    APPS0_COMMAND_ID    apps0_cmd;
    FW_VER_CMD_ID       fw_ver_cmd;
    uint16              padding;
    uint32              fw_ver;
} APPS0_FW_VER_CMD_RESP;

typedef struct
{
    APPS0_COMMAND_ID         apps0_cmd;
    CAPACITIVE_SENSOR_CMD_ID capacitive_sensor_cmd;
    uint16              pad;
} APPS0_CAPACITIVE_SENSOR_CMD_REQ;

typedef struct
{
    APPS0_COMMAND_ID         apps0_cmd;
    CAPACITIVE_SENSOR_CMD_ID capacitive_sensor_cmd;
    uint16                   padding;
    uint32                   Cint_fF;    /* Pad absolute capacitance */
} APPS0_CAPACITIVE_SENSOR_CMD_RESP;
    

typedef union
{
    APPS0_PIO_CMD_REQ               pio_cmd;
    APPS0_XIO_CMD_REQ               xio_cmd;
    APPS0_LED_CMD_REQ               led_cmd;
    APPS0_ADC_CMD_REQ               adc_cmd;
    APPS0_FS_READ_REQ               fs_read_cmd;
    APPS0_FS_WRITE_REQ              fs_write_cmd;
    APPS0_LICENSE_CMD_REQ           license_cmd;
    APPS0_USB_CMD_REQ               usb_cmd;
    APPS0_FW_VER_CMD_REQ            fw_ver_cmd;
    APPS0_CAPACITIVE_SENSOR_CMD_REQ capacitive_sensor_cmd;
    APPS0_NFC_CMD_REQ               nfc_cmd;
    APPS0_PS_CMD_REQ                ps_cmd;
} APPS0_CMD_REQ;

typedef union
{
    APPS0_PIO_CMD_RESP               pio_rsp;
    APPS0_XIO_CMD_RESP               xio_rsp;
    APPS0_LED_CMD_RESP               led_rsp;
    APPS0_ADC_CMD_RESP               adc_rsp;
    APPS0_FS_READ_RESP               fs_read_rsp;
    APPS0_FS_WRITE_RESP              fs_write_rsp;
    APPS0_LICENSE_CMD_RESP           license_rsp;
    APPS0_USB_CMD_RESP               usb_rsp;
    APPS0_FW_VER_CMD_RESP            fw_ver_rsp;
    APPS0_CAPACITIVE_SENSOR_CMD_RESP capacitive_sensor_rsp;
    APPS0_NFC_CMD_RESP               nfc_rsp;
    APPS0_PS_CMD_RESP                ps_rsp;
} APPS0_CMD_RESP;

typedef union
{
    CURATOR_MIB_CMD_REQ   mib_cmd;
} CURATOR_CMD_REQ;

typedef union
{
    CURATOR_MIB_CMD_RESP   mib_rsp;
} CURATOR_CMD_RESP;

typedef union
{
    APPS0_CMD_REQ   apps0_req;
    CURATOR_CMD_REQ cur_req;
    /* Add other subsystems cmd req here */
} PROD_TEST_REQ_PAYLOAD;

typedef union
{
    APPS0_CMD_RESP   apps0_resp;
    CURATOR_CMD_RESP cur_resp;
    /* Add other subsystems cmd response here */
} PROD_TEST_RESP_PAYLOAD;

typedef struct
{
    PROD_TEST_COMMAND_ID    cmd_id;
    uint8                   padding;
    uint16                  payload_length;
    uint16                  seq_num;    /* value chosen by host */
    uint16                  status;     /* placeholder for response status */
    PROD_TEST_REQ_PAYLOAD   payload;
} PROD_TEST_CMD_REQ;

typedef struct
{
    PROD_TEST_COMMAND_ID    cmd_id;
    uint8                   padding;
    uint16                  payload_length;
    uint16                  seq_num;    /* value chosen by host */
    uint16                  status;     /* command response status */
    PROD_TEST_RESP_PAYLOAD  payload;
} PROD_TEST_CMD_RESP;

#endif /* __PRODTESTPDU_H__ */

