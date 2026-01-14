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

#ifndef ADC_H
#define ADC_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "stm32f4xx_ll_adc.h"
#include "stm32f4xx.h"
#include "circbuf.h"

typedef struct {
	ADC_TypeDef* Instance; // which ADC
	uint16_t sample; // converted sample
	CircBuf* circ_buffer; // pointer to circ buffer struct

} ADC_Handle_t;

// global ADC_Handle_t instance
extern ADC_Handle_t adc;

/**
  * @brief  Initialize adc module
  * @param  *adc Pointer to the ADC_Handle_t instance
  * @param  *circ_buf Pointer to CircBuf instance
  * @retval Void
**/
void adc_init(ADC_Handle_t* adc, CircBuf* circ_buf);

/**
  * @brief  Start Analog-to-Digital Conversion
  * @param  *adc Pointer to the ADC_Handle_t instance
  * @retval Void
**/
void adc_start_conversion(ADC_Handle_t* adc);

/**
  * @brief  Read, process, and store ADC sample upon IT
  * @param  *adc Pointer to the ADC_Handle_t instance
  * @retval Void
**/
void adc_handle_irq(ADC_Handle_t* adc);

#endif
