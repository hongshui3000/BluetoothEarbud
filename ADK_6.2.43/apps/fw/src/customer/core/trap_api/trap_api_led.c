/* Copyright (c) 2016 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */
/**
 * \file
 * Implementation of LED hardware control.
 */

#include <csrtypes.h>
#include <led.h>
#include <pio.h>
#include "led_cfg/led_cfg.h"
#include "hydra_log/hydra_log.h"

#ifndef DESKTOP_TEST_BUILD


/** PIO index of the first LED */
#define PIO_CFG_FIRST_LED_PIO_IDX DEFAULT_LED0_PIO
/** PIO index of the last LED */
#ifdef CHIP_FAMILY_CRESCENDO
#define PIO_CFG_LAST_LED_PIO_IDX 72
#else
#define PIO_CFG_LAST_LED_PIO_IDX 71
#endif /* CHIP_FAMILY_CRESCENDO */

static led_instance_mask initialised_leds = 0;

bool LedConfigure(led_id led, led_config_key key, uint16 value)
{
    uint32 mask, status;
    uint16 pio = NUMBER_OF_PIOS, bank;
    bool pin_acquired = FALSE;

    if (led >= NUM_OF_LEDS)
    {
        L2_DBG_MSG1("LedConfigure: Invalid LED ID %d", led);
        return FALSE;
    }

    if (0 == (initialised_leds & (1 << led)))
    {
        /* In the future we'd like the app to configure these. Until then we
           default LED pads to the LED function to maintain backwards
           compatibility. */
        if ((!led_cfg_led_has_pio(led)) &&
            (PIO_CFG_FIRST_LED_PIO_IDX + (led - (uint32)LED_0) <=
             PIO_CFG_LAST_LED_PIO_IDX))
        {
            pio = (uint16)(PIO_CFG_FIRST_LED_PIO_IDX + (led - (uint32)LED_0));
            bank = (uint16)(pio / 32);
            mask = 1 << (pio % 32);
            
            status = PioSetMapPins32Bank(bank, mask, 0);
            L2_DBG_MSG1("LED defaults: PioSetMapPins32Bank status 0x%08x",
                        status);
            if (status)
            {
                return FALSE;
            }
            status = PioSetFunction(pio, LED);
            L2_DBG_MSG2("LED defaults: PioSetFunction to LED for pio %d, "
                        "status %d", pio, status);
            if (status == FALSE)
            {
                return FALSE;
            }
            pin_acquired = TRUE;
        }

        if (led_cfg_initialise_led(led))
        {
            initialised_leds |= (led_instance_mask)(1 << led);
        }
        else
        {
            if (pin_acquired)
            {
                /* release newly acquired PIO */
                status = PioSetFunction(pio, OTHER);
                L2_DBG_MSG2("LED defaults: PioSetFunction to OTHER for pio %d, "
                            "status %d", pio, status);
            }
            return FALSE;
        }
    }

    return led_cfg_set_config(led, key, value);
}


#endif /* DESKTOP_TEST_BUILD */
