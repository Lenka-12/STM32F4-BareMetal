/*
 * timebase.c
 *
 *  Created on: Sep 22, 2023
 *      Author: Malefetsane Lenka
 */

#include "stm32f411xe.h"
#include "timebase.h"


volatile uint32_t g_curr_tick;
volatile uint32_t g_curr_p;
volatile uint32_t tick_freq = 1;

static void tick_increment(void);
/*
 * @Brief: Initialises SysTick to generate exception every second
 * @Param: None
 * @Retval: None
 */
void timebase_init(void){
	/*disable global interrupts*/
	__disable_irq();

	/*Reload timer with number of cycles for a milli second*/
	SysTick->LOAD = MILLI_SEC_LOAD-1;

	/*clear systick current value register*/
	SysTick->VAL = 0;

	/*select internal clock source and enable exception*/
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk|SysTick_CTRL_TICKINT_Msk;

	/*enable systick*/
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	__enable_irq(); /*enable all global interrupts*/

}

/*
 * @Brief: Increment tick count
 * @Param: None
 * @Retval: None
 */
static void tick_increment(void){
	g_curr_tick+=tick_freq;
}

void SysTick_Handler(void){
	tick_increment();
}

/*
 * @Brief: Returns current ticks count
 * @Param: None
 * @Retval: uint32_t, current ticks count
 */
uint32_t get_tick(){
	/*disable all global interrupts*/
	__disable_irq();

	g_curr_p = g_curr_tick; /*get current number of ticks*/

	/*enable global interrupts*/
	__enable_irq();
	return g_curr_p;
}

/*
 * @Brief: Generates Hardware delay
 * @Param: uint32_t, delay in milliseconds
 *
 * */
void delay(uint32_t msdelay){
	uint32_t tickstart = get_tick();            /*get current number of ticks*/
	uint32_t wait = msdelay;

	while((get_tick()-tickstart)<wait);         /*wait until wiat time expires*/
}



