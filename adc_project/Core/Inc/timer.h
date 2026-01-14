/**
 * timer.h
 * --------
 * Timer-based scheduling module for ADC sampling & display update.
 *
 * Uses hardware timers to generate fixed-rate software ticks
 * for ADC sampling and display updates.
 *
 * Timer ISRs set software flags that are serviced in the main loop
 * to avoid blocking or heavy processing in interrupt context.
 **/

#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

#define TIM2_PRESCALER 83 // 1 Mhz timer clock
#define TIM2_AUTORELOAD 49 // ~20 kHz ADC trigger
#define TIM3_PRESCALER 83 // 1 Mhz timer clock
#define TIM3_AUTORELOAD 33332// ~ 30 Hz display update


// global adc_tick and display_tick software flags
extern volatile uint8_t adc_tick;
extern volatile uint8_t display_tick;

typedef struct {

	uint32_t prescaler2; // timer 2 prescaler value
	uint32_t autoreload2; // timer 2 autoreload value
	uint32_t prescaler3; // timer 3 prescaler value
	uint32_t autoreload3; // timer 3 autoreload value

} TIM_Handle_t;

// global TIM_Handle_t instance
extern TIM_Handle_t timer;

/**
  * @brief  Initialize timer module
  * @param  *timer Pointer to the TIM_Handle_t instance
  * @retval Void
**/
void timer_init(TIM_Handle_t* timer);

/**
  * @brief  Update adc_tick software flag
  * @param  Void
  * @retval Void
**/
void timer_handle_irq2(void);

/**
  * @brief  Update display_tick software flag
  * @param  Void
  * @retval Void
**/
void timer_handle_irq3(void);

#endif
