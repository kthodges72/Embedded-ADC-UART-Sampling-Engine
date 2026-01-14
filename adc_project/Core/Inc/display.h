/**
 * display.h
 * ----------
 * Display and visualization module for ADC-based audio level metering.
 *
 * Converts processed ADC samples into a stable, human-readable
 * ASCII bar graph over UART.
 *
 * Includes envelope smoothing to improve visual responsiveness.
 **/

#ifndef DISPLAY_H_
#define DISPLAY_H_


#include "circbuf.h"
#include "adc.h"


typedef struct {

	uint16_t isf; // inverse smoothing factor (for LPF)

} DISPLAY_Handle_t;

// global DISPLAY_Handle_t instance
extern DISPLAY_Handle_t disp;


/**
  * @brief  Initialize display module
  * @param  disp Pointer to the DISP_Handle_t instance
  * @retval Void
**/
void display_init(DISPLAY_Handle_t* disp);

/**
  * @brief  Process sample value using envelope filter
  * @param  disp Pointer to the DISP_Handle_t instance
  * @param  raw sample value to process
  * @retval Void
**/
uint16_t display_envelope_filter(DISPLAY_Handle_t* disp, uint16_t raw);

/**
  * @brief  Process sample value using envelope filter
  * @param  adc Pointer to ADC_Handle_t instance
  * @param  circ_buf Pointer to CircBuf instance
  * @retval Void
**/
void display_update(ADC_Handle_t* adc, CircBuf* circ_buf);



#endif
