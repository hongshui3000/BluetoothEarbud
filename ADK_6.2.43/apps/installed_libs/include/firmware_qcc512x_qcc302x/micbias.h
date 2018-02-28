#ifndef __MICBIAS_H__
#define __MICBIAS_H__
#include <app/mic_bias/mic_bias_if.h>

/*! file  @brief Traps to control the Microphone bias hardware */

#if TRAPSET_MICBIAS

/**
 *  \brief Control of Microphone bias hardware 
 *     
 *     Note: The dedicated mic bias pin is only present on some BlueCore variants.
 *     Only chips from BC5 onwards have this feature. In addition at least one ADC
 *     or DAC on chip must be enabled for the MicBias hardware to operate.
 *     Consult the datasheet for your BlueCore variant for more information.
 *  \param mic Which microphone bias pin to configure 
 *  \param key Which aspect of the microphone bias to configure 
 *  \param value Which value to use
 *  \return TRUE if the "mic" parameter is valid and the input value is in valid range,
 *  else FALSE.
 * 
 * \note This trap may NOT be called from a high-priority task handler
 * 
 * \ingroup trapset_micbias
 */
bool MicbiasConfigure(mic_bias_id mic, mic_bias_config_key key, uint16 value);
#endif /* TRAPSET_MICBIAS */
#endif
