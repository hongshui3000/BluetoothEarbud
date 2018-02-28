/****************************************************************************
Copyright (c) 2017 Qualcomm Technologies International, Ltd.

FILE NAME
    pio_common_pin_function.c

DESCRIPTION
    Common PIO operations used by libraries and applications
    Enable pins for special functions

NOTES

*/

/****************************************************************************
    Header files
*/
#include <pio.h>
#include "pio_common.h"

/****************************************************************************
    Public functions
*/

void PioCommonConfigureSpdifInput(uint8 spdif_input_pin)
{
    PioSetFunction(spdif_input_pin, SPDIF_RX);
}

void PioCommonConfigureSpdifOutput(uint8 spdif_output_pin)
{
    PioSetFunction(spdif_output_pin, SPDIF_TX);
}
