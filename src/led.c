/*
 * led.c
 *
 *  Created on: Sep 21, 2023
 *      Author: Malefetsane Lenka
 */

#include "led.h"
#include "gpio_driver.h"

void led_init(){
	/*Enable clock for GPIOD*/
	GPIO_ClockConfig(GPIOD, ENABLE);

	/*Enable output mode for PD12*/
	GPIO_Handle_t led;

	led.GPIOx = GPIOD;

	led.GPIOx_Cofig.GPIO_OPType = PushPull;                         //OutPut PushPull
	led.GPIOx_Cofig.GPIO_PinAltFnMode = AF0;                        //Reset Alternate function

	led.GPIOx_Cofig.GPIO_PinMode = GPIO_MODE_OUT;                   //Output Mode
	led.GPIOx_Cofig.GPIO_PinNumber = GPIO_PIN_12;                   //Green led

	led.GPIOx_Cofig.GPIO_PinSpeed = LowSpeed;                       //lowspeed mode
	led.GPIOx_Cofig.GPIO_PullUpPullDown_Ctrl = NoPullUpOrPullDown;  //no pullup or pulldown

	GPIO_Init(&led);

}


void led_on(){
	/*Turn green led on*/
	GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
}

void led_off(){
	/*Turn green led off*/
	GPIO_WritePin(GPIOD, GPIO_PIN_12,GPIO_PIN_RESET);
}

void led_toggle(){
	GPIO_TogglePin(GPIOD, GPIO_PIN_12);
}


