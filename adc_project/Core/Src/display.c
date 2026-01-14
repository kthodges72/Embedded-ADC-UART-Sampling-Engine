/**
 * display.c
 * ----------
 * Display and visualization module for ADC-based audio level metering.
 *
 * Converts processed ADC samples into a stable, human-readable
 * ASCII bar graph over UART.
 *
 * Includes envelope smoothing to improve visual responsiveness.
 **/

#include <display.h>
#include "circbuf.h"
#include "adc.h"
#include "uart.h"

// initialize global DISPLAY_Handle_t instance
DISPLAY_Handle_t disp;

/**
  * @brief  Initialize display module
  * @param  disp Pointer to the DISP_Handle_t instance
  * @retval Void
**/
void display_init(DISPLAY_Handle_t* disp) {

	// set isf field
	disp->isf = 16;
}

/**
  * @brief  Process sample value using envelope filter
  * @param  disp Pointer to the DISP_Handle_t instance
  * @param  raw sample value to process
  * @retval Void
**/
uint16_t display_envelope_filter(DISPLAY_Handle_t* disp, uint16_t raw) {

	// initialize envelope value
	 static int32_t env = 0;

	 // remove DC bias, take absolute value
	 int32_t x = raw - 2048;
	 if (x < 0) x = -x;

	 // apply low-pass filter formula, return processed value
	 env = env + (x - env) / disp->isf;
	 return (uint16_t)env;
}

/**
  * @brief  Process sample value using envelope filter
  * @param  adc Pointer to ADC_Handle_t instance
  * @param  circ_buf Pointer to CircBuf instance
  * @retval Void
**/
void display_update(ADC_Handle_t* adc, CircBuf* circ_buf) {

	// set level variable to processed sample
	uint16_t level = adc->sample;

	// determine bar length based on processed sample value
	if (level < 19) {
	    uart_DMA_printf(&uart, "\r[....................]");
	} else if (level >= 19 && level < 38) {
	    uart_DMA_printf(&uart, "\r[:...................]");
	} else if (level >= 38 && level < 56) {
	    uart_DMA_printf(&uart, "\r[|...................]");
	} else if (level >= 56 && level < 75) {
	    uart_DMA_printf(&uart, "\r[|:..................]");
	} else if (level >= 75 && level < 94) {
	    uart_DMA_printf(&uart, "\r[||..................]");
	} else if (level >= 94 && level < 113) {
	    uart_DMA_printf(&uart, "\r[||:.................]");
	} else if (level >= 113 && level < 131) {
	    uart_DMA_printf(&uart, "\r[|||.................]");
	} else if (level >= 131 && level < 150) {
	    uart_DMA_printf(&uart, "\r[|||:................]");
	} else if (level >= 150 && level < 169) {
	    uart_DMA_printf(&uart, "\r[||||................]");
	} else if (level >= 169 && level < 188) {
	    uart_DMA_printf(&uart, "\r[||||:...............]");
	} else if (level >= 188 && level < 206) {
	    uart_DMA_printf(&uart, "\r[|||||...............]");
	} else if (level >= 206 && level < 225) {
	    uart_DMA_printf(&uart, "\r[|||||:..............]");
	} else if (level >= 225 && level < 244) {
	    uart_DMA_printf(&uart, "\r[||||||..............]");
	} else if (level >= 244 && level < 263) {
	    uart_DMA_printf(&uart, "\r[||||||:.............]");
	} else if (level >= 263 && level < 281) {
	    uart_DMA_printf(&uart, "\r[|||||||.............]");
	} else if (level >= 281 && level < 300) {
	    uart_DMA_printf(&uart, "\r[|||||||:............]");
	} else if (level >= 300 && level < 319) {
	    uart_DMA_printf(&uart, "\r[||||||||............]");
	} else if (level >= 319 && level < 338) {
	    uart_DMA_printf(&uart, "\r[||||||||:...........]");
	} else if (level >= 338 && level < 356) {
	    uart_DMA_printf(&uart, "\r[|||||||||...........]");
	} else if (level >= 356 && level < 375) {
	    uart_DMA_printf(&uart, "\r[|||||||||:..........]");
	} else if (level >= 375 && level < 394) {
	    uart_DMA_printf(&uart, "\r[||||||||||..........]");
	} else if (level >= 394 && level < 413) {
	    uart_DMA_printf(&uart, "\r[||||||||||:.........]");
	} else if (level >= 413 && level < 431) {
	    uart_DMA_printf(&uart, "\r[|||||||||||.........]");
	} else if (level >= 431 && level < 450) {
	    uart_DMA_printf(&uart, "\r[|||||||||||:........]");
	} else if (level >= 450 && level < 469) {
	    uart_DMA_printf(&uart, "\r[||||||||||||........]");
	} else if (level >= 469 && level < 488) {
	    uart_DMA_printf(&uart, "\r[||||||||||||:.......]");
	} else if (level >= 488 && level < 506) {
	    uart_DMA_printf(&uart, "\r[|||||||||||||.......]");
	} else if (level >= 506 && level < 525) {
	    uart_DMA_printf(&uart, "\r[|||||||||||||:......]");
	} else if (level >= 525 && level < 544) {
	    uart_DMA_printf(&uart, "\r[||||||||||||||......]");
	} else if (level >= 544 && level < 563) {
	    uart_DMA_printf(&uart, "\r[||||||||||||||:.....]");
	} else if (level >= 563 && level < 581) {
	    uart_DMA_printf(&uart, "\r[|||||||||||||||.....]");
	} else if (level >= 581 && level < 600) {
	    uart_DMA_printf(&uart, "\r[|||||||||||||||:....]");
	} else if (level >= 600 && level < 619) {
	    uart_DMA_printf(&uart, "\r[||||||||||||||||....]");
	} else if (level >= 619 && level < 638) {
	    uart_DMA_printf(&uart, "\r[||||||||||||||||:...]");
	} else if (level >= 638 && level < 656) {
	    uart_DMA_printf(&uart, "\r[|||||||||||||||||...]");
	} else if (level >= 656 && level < 675) {
	    uart_DMA_printf(&uart, "\r[|||||||||||||||||:..]");
	} else if (level >= 675 && level < 694) {
	    uart_DMA_printf(&uart, "\r[||||||||||||||||||..]");
	} else if (level >= 694 && level < 713) {
	    uart_DMA_printf(&uart, "\r[||||||||||||||||||:.]");
	} else if (level >= 713 && level < 731) {
	    uart_DMA_printf(&uart, "\r[|||||||||||||||||||.]");
	} else if (level >= 731 && level < 750) {
	    uart_DMA_printf(&uart, "\r[|||||||||||||||||||:]");
	} else {
	    uart_DMA_printf(&uart, "\r[||||||||||||||||||||]");
	}


	// Bar:         Level:
	// [....................] 30
	// [:...................] 62.5
	// [|...................]
	// [|:..................]
	// [||..................]
	// [||:.................]
	// [|||.................]
	// [|||:................]
	// [||||................]
	// [||||:...............]
	// [|||||...............]
	// [|||||:..............]
	// [||||||..............]
	// [||||||:.............]
	// [|||||||.............]
	// [|||||||:............]
	// [||||||||............]
	// [||||||||:...........]
	// [|||||||||...........]
	// [|||||||||:..........]
	// [||||||||||..........]
	// [||||||||||:.........]
	// [|||||||||||.........]
	// [|||||||||||:........]
	// [||||||||||||........]
	// [||||||||||||:.......]
	// [|||||||||||||.......]
	// [|||||||||||||:......]
	// [||||||||||||||......]
	// [||||||||||||||:.....]
	// [|||||||||||||||.....]
	// [|||||||||||||||:....]x
	// [||||||||||||||||....]
	// [||||||||||||||||:...]
	// [|||||||||||||||||...]
	// [|||||||||||||||||:..]
	// [||||||||||||||||||..]
	// [||||||||||||||||||:.]
	// [|||||||||||||||||||.]
	// [|||||||||||||||||||:]
	// [||||||||||||||||||||]

}
