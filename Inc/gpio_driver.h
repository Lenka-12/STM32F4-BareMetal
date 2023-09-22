/*
 *@File:   gpio.h  
 *@Author: Malefetsane Lenka
 *@Date:   05/08/2023
 */
#ifndef  GPIO_H_
#define  GPIO_H_

#include "stm32f411xe.h"
#include <stdint.h>


#define PullUp                  1U
#define NoPullUpOrPullDown      0U
#define PullDown                2U 


#define PushPull                 0U
#define OpenDrain                1U

#define ENABLE                   1U
#define DISABLE                  0U

#define GPIO_PIN_SET             ENABLE
#define GPIO_PIN_RESET           DISABLE

#define GPIO_BASEADDR_TO_CODE(GPIOx)      ((GPIOx==GPIOA) ? 0 :\
										   (GPIOx==GPIOB) ? 1 :\
										   (GPIOx==GPIOC) ? 2 :\
										   (GPIOx==GPIOD) ? 3 :\
										   (GPIOx==GPIOE) ? 4 :\
										   (GPIOx==GPIOH) ? 7 :0)

/*
 *@Brief:  General purpose Input/OutPut Configurable Modes
 */
typedef enum {
   GPIO_MODE_IN,
   GPIO_MODE_OUT,
   GPIO_MODE_Alt,
   GPIO_MODE_ANALOG,
   GPIO_MODE_IT_RT,
   GPIO_MODE_IT_FT,
   GPIO_MODE_IT_RFT,

}Mode_t;

/*
 *@Brief: GPIO user configurable speeds
 */
typedef enum{
    LowSpeed,
    MediumSpeed,
    FastSpeed,
    HighSpeed,

}Speed_t;

/*
 * @Brief; GPIO  Alternate function Possible Values
 */
typedef enum{
    AF0,
    AF1,
    AF2,
    AF3,
    AF4,
    AF5,
    AF6,
    AF7,
    AF8,
    AF9,
    AF10,
    AF11,
    AF12,
    AF13,
    AF14,
    AF15,

}Altfn_t;


/*
 *@Brief: GPIO pin Numbers
 */

typedef enum{
    GPIO_PIN_0,
    GPIO_PIN_1,
    GPIO_PIN_2,
    GPIO_PIN_3,
    GPIO_PIN_4,
    GPIO_PIN_5,
    GPIO_PIN_6,
    GPIO_PIN_7,
    GPIO_PIN_8,
    GPIO_PIN_9,
    GPIO_PIN_10,
    GPIO_PIN_11,
    GPIO_PIN_12,
    GPIO_PIN_13,
    GPIO_PIN_14,
    GPIO_PIN_15,

}GPIO_PinNumber;

/*
 *@Brief: GPIO configuration structure
 */

typedef struct{
    GPIO_PinNumber GPIO_PinNumber;
    Mode_t  GPIO_PinMode;
    Speed_t GPIO_PinSpeed;
    uint8_t GPIO_OPType;
    Altfn_t GPIO_PinAltFnMode;
    uint8_t GPIO_PullUpPullDown_Ctrl;
}GPIO_Config_t;


typedef struct 
{
    GPIO_TypeDef* GPIOx;
    GPIO_Config_t GPIOx_Cofig;
}GPIO_Handle_t;


void GPIO_ClockConfig(GPIO_TypeDef* GPIOx,uint8_t EnOrDi);
void GPIO_Init(GPIO_Handle_t* GPIOx_Handle);
void GPIO_DeInit(GPIO_TypeDef* GPIOx);

uint8_t GPIO_ReadPin(GPIO_TypeDef* GPIOx,GPIO_PinNumber PinNumber);
uint16_t GPIO_ReadPort(GPIO_TypeDef* GPIOx);

void GPIO_WritePin(GPIO_TypeDef* GPIOx,GPIO_PinNumber PinNumber,uint8_t PinState);
void GPIO_WritePort(GPIO_TypeDef* GPIOx,uint16_t Value);

void GPIO_TogglePin(GPIO_TypeDef* GPIOx,GPIO_PinNumber PinNumber);
void GPIO_IRQConfig(IRQn_Type IRQn,uint8_t PriorityNumber,uint8_t EnOrDi);
void GPIO_ISRHandling(GPIO_PinNumber PinNumber);
#endif
