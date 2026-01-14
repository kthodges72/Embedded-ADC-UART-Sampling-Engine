/**
 * timer.c
 * --------
 * Timer-based scheduling module for ADC sampling & display update.
 *
 * Uses hardware timers to generate fixed-rate software ticks
 * for ADC sampling and display updates.
 *
 * Timer ISRs set software flags that are serviced in the main loop
 * to avoid blocking or heavy processing in interrupt context.
 **/

#include "timer.h"
#include "stm32f4xx_ll_tim.h"

// initialize global adc_tick and display_tick software flags
volatile uint8_t adc_tick = 0;
volatile uint8_t display_tick = 0;

// initialize global TIM_Handle_t instance
TIM_Handle_t timer;

/**
  * @brief  Initialize timer module
  * @param  *timer Pointer to the TIM_Handle_t instance
  * @retval Void
**/
void timer_init(TIM_Handle_t* timer) {

	// initialize software state (prescaler and autoreload values)
	timer->prescaler2 = TIM2_PRESCALER;
	timer->autoreload2 = TIM2_AUTORELOAD;
	timer->prescaler3 = TIM3_PRESCALER;
	timer->autoreload3 = TIM3_AUTORELOAD;

	// set timer 2 prescaler and autoreload values with LL functions
	LL_TIM_SetPrescaler(TIM2, timer->prescaler2);
	LL_TIM_SetAutoReload(TIM2, timer->autoreload2);

	// set timer 2 prescaler and autoreload values with LL functions
	LL_TIM_SetPrescaler(TIM3, timer->prescaler3);
	LL_TIM_SetAutoReload(TIM3, timer->autoreload3);

	// enable timer 2 and 3 flags
	LL_TIM_EnableIT_UPDATE(TIM2);
	LL_TIM_EnableCounter(TIM2);
	LL_TIM_EnableIT_UPDATE(TIM3);
	LL_TIM_EnableCounter(TIM3);
}

/**
  * @brief  Update adc_tick software flag
  * @param  Void
  * @retval Void
**/
void timer_handle_irq2(void) {
	if (LL_TIM_IsActiveFlag_UPDATE(TIM2)) {
			LL_TIM_ClearFlag_UPDATE(TIM2);

			// update adc_tick
			adc_tick = 1;
	}
}

/**
  * @brief  Update display_tick software flag
  * @param  Void
  * @retval Void
**/
void timer_handle_irq3(void) {
	if (LL_TIM_IsActiveFlag_UPDATE(TIM3)) {
			LL_TIM_ClearFlag_UPDATE(TIM3);

			// update display_tick
			display_tick = 1;
	}
}
