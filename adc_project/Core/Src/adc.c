/**
 * adc.c
 * ------
 * ADC driver for single-channel sampling using interrupts.
 *
 * Handles ADC initialization, software-triggered conversions,
 * and ISR-based sample acquisition.
 *
 * Raw samples are processed through an
 * envelope filter before being stored.
 **/




#include <display.h>
#include "adc.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "stm32f4xx_ll_adc.h"
#include "stm32f4xx.h"
#include "circbuf.h"

// initialize global ADC_Handle_t instance
ADC_Handle_t adc;

/**
  * @brief  Initialize adc module
  * @param  *adc Pointer to the ADC_Handle_t instance
  * @param  *circ_buf Pointer to CircBuf instance
  * @retval Void
**/
void adc_init(ADC_Handle_t* adc, CircBuf* circ_buf) {


	// initialize software state
	adc->Instance = ADC1;
	adc->sample = 0;
	adc->circ_buffer = circ_buf;

	// clear potential flags + flush DR
		LL_ADC_ClearFlag_EOCS(adc->Instance);
		LL_ADC_ClearFlag_OVR(adc->Instance);
		(void)LL_ADC_REG_ReadConversionData12(adc->Instance);

	// enable adc instance
	LL_ADC_Enable(adc->Instance);

	// enable flags
	LL_ADC_EnableIT_EOCS(adc->Instance);
}

/**
  * @brief  Start Analog-to-Digital Conversion
  * @param  *adc Pointer to the ADC_Handle_t instance
  * @retval Void
**/
void adc_start_conversion(ADC_Handle_t* adc) {

	// start conversion
	LL_ADC_REG_StartConversionSWStart(adc->Instance);
}

/**
  * @brief  Read, process, and store ADC sample upon IT
  * @param  *adc Pointer to the ADC_Handle_t instance
  * @retval Void
**/
void adc_handle_irq(ADC_Handle_t* adc) {

	// variable for new sample
	uint16_t raw_data;
	uint16_t data;

	// if End of Conversion flag active, configure and load new sample
	if (LL_ADC_IsActiveFlag_EOCS(adc->Instance)) {

		// read sample data (clear flag to be safe)
		raw_data = LL_ADC_REG_ReadConversionData12(adc->Instance);
		LL_ADC_ClearFlag_EOCS(adc->Instance);

		// apply envelope / smoothing filter for stable visual output
		data = display_envelope_filter(&disp, raw_data);

		// set processed sample to adc->sample (update adc->sample)
		adc->sample = data;
	}

}



