/****************************************************************************
Copyright (c) 2017 Qualcomm Technologies International, Ltd.

FILE NAME
    pio_common_configure.c

DESCRIPTION
    Common PIO operations used by libraries and applications
    Enable pins for special functions

NOTES

*/

/****************************************************************************
    Header files
*/
#include <pio.h>
#include <adc.h>
#include <led.h>
#include <print.h>
#include <panic.h>
#include <vmtypes.h>

#include "pio_common.h"

/****************************************************************************
    Public functions
*/
void PioCommonConfigureSpdifInput(uint8 spdif_input_pin)
{
    UNUSED(spdif_input_pin);
}

void PioCommonConfigureSpdifOutput(uint8 spdif_output_pin)
{
    UNUSED(spdif_output_pin);
}
