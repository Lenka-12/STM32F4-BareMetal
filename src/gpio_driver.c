/*
 *@Author: Malefetsane Lenka
 *@Date:   05/08/2023
 *@Brief: gpio_driver.c library
 */

#include "gpio_driver.h"


void GPIO_ClockConfig(GPIO_TypeDef* GPIOx,uint8_t EnOrDi){
    if (EnOrDi){
        if (GPIOx==GPIOA){
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
        }

        else if (GPIOx==GPIOB){
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
        }
        else if (GPIOx==GPIOC){
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
        }
        else if (GPIOx==GPIOD){
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
        }
        else if (GPIOx==GPIOE){
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
        }
        else if (GPIOx==GPIOH){
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
        }

    }

    else{
        if (GPIOx==GPIOA){
            RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOAEN;
        }

        else if (GPIOx==GPIOB){
            RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOBEN;
        }
        else if (GPIOx==GPIOC){
            RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOCEN;
        }
        else if (GPIOx==GPIOD){
            RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIODEN;
        }
        else if (GPIOx==GPIOE){
            RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOEEN;
        }
        else if (GPIOx==GPIOH){
            RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOHEN;
        }

    }
}


void GPIO_Init(GPIO_Handle_t* GPIOx_Handle){

	uint8_t PinNumber = GPIOx_Handle->GPIOx_Cofig.GPIO_PinNumber;
	GPIOx_Handle->GPIOx->MODER &=~(GPIO_MODE_ANALOG<<2*PinNumber);  //Clear Mode Register
    //1. configure the Mode of gpio pin
    if (GPIOx_Handle->GPIOx_Cofig.GPIO_PinMode<=GPIO_MODE_ANALOG){
        //Non interrupt mode

        GPIOx_Handle->GPIOx->MODER |= (GPIOx_Handle->GPIOx_Cofig.GPIO_PinMode<<2*(PinNumber));


    }
    else{
    	//1. Configure Edge properties
    	if (GPIOx_Handle->GPIOx_Cofig.GPIO_PinMode==GPIO_MODE_IT_RT){
    		//Configure RTSR
    		EXTI->RTSR |= (0x01U<<PinNumber);
    		//CLear RTSR
    		EXTI->FTSR &= ~(0x01U<<PinNumber);
    	}
    	else if ((GPIOx_Handle->GPIOx_Cofig.GPIO_PinMode==GPIO_MODE_IT_FT)){
    		//Configure FTSR
    		EXTI->FTSR |= (0x01U<<PinNumber);
    		//RTSR
    		EXTI->RTSR &= ~(0x01U<<PinNumber);
    	}

    	else if ((GPIOx_Handle->GPIOx_Cofig.GPIO_PinMode==GPIO_MODE_IT_RFT)){
    		//Configure FTSR and RTSR
    		EXTI->FTSR |= (0x01U<<PinNumber);
    		EXTI->RTSR |= (0x01U<<PinNumber);

    	}

    	//2. Configure GPIO port selection in SYSCFG_EXIXR
    	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    	uint8_t EXTICRx = PinNumber/4;
    	uint8_t  EXTICRpos = PinNumber%4;

    	uint8_t portcode = GPIO_BASEADDR_TO_CODE(GPIOx_Handle->GPIOx);

    	SYSCFG->EXTICR[EXTICRx] |= (portcode<<4*EXTICRpos);

    	//3. Unmask the pin in EXTI_IMR
    	EXTI->IMR |= (0x01U<<PinNumber);

    }
    //2.Pin Speed
    GPIOx_Handle->GPIOx->OSPEEDR &=~(GPIO_MODE_ANALOG<<2*PinNumber);
    GPIOx_Handle->GPIOx->OSPEEDR |=(GPIOx_Handle->GPIOx_Cofig.GPIO_PinSpeed<<2*PinNumber);

    //3.PullUp or PullDown
    GPIOx_Handle->GPIOx->PUPDR &=~(GPIO_MODE_ANALOG<<2*PinNumber);
    GPIOx_Handle->GPIOx->PUPDR |=(GPIOx_Handle->GPIOx_Cofig.GPIO_PullUpPullDown_Ctrl<<2*PinNumber);

    //4.Output Type
    GPIOx_Handle->GPIOx->OTYPER &=~(GPIO_MODE_ANALOG<<PinNumber);
    GPIOx_Handle->GPIOx->OTYPER |=(GPIOx_Handle->GPIOx_Cofig.GPIO_OPType<<PinNumber);

   //5.Alt functionality
    if (GPIOx_Handle->GPIOx_Cofig.GPIO_PinMode==GPIO_MODE_Alt){
    	//Configure Alt registers
    	uint8_t AFRn = PinNumber/8;
    	uint8_t AFSELy = PinNumber%8;
    	GPIOx_Handle->GPIOx->AFR[AFRn] &=~(0xFU<<4*AFSELy);

    	GPIOx_Handle->GPIOx->AFR[AFRn] |= (GPIOx_Handle->GPIOx_Cofig.GPIO_PinAltFnMode<<4*AFSELy);
    }
}

void GPIO_DeInit(GPIO_TypeDef* GPIOx){
	if (GPIOx==GPIOA){
		do{
			RCC->AHB1RSTR |=RCC_AHB1RSTR_GPIOARST;
			RCC->AHB1RSTR &=~RCC_AHB1RSTR_GPIOARST;

		}while(0);
	}

	else if (GPIOx==GPIOB){

		RCC->AHB1RSTR |=RCC_AHB1RSTR_GPIOARST;
		RCC->AHB1RSTR &=~RCC_AHB1RSTR_GPIOARST;
	}
	else if (GPIOx==GPIOB){

		RCC->AHB1RSTR |=RCC_AHB1RSTR_GPIOBRST;
		RCC->AHB1RSTR &=~RCC_AHB1RSTR_GPIOBRST;
	}
	else if (GPIOx==GPIOC){

		RCC->AHB1RSTR |=RCC_AHB1RSTR_GPIOCRST;
		RCC->AHB1RSTR &=~RCC_AHB1RSTR_GPIOCRST;
	}
	else if (GPIOx==GPIOD){

		RCC->AHB1RSTR |=RCC_AHB1RSTR_GPIODRST;
		RCC->AHB1RSTR &=~RCC_AHB1RSTR_GPIODRST;
	}
	else if (GPIOx==GPIOE){

		RCC->AHB1RSTR |=RCC_AHB1RSTR_GPIOERST;
		RCC->AHB1RSTR &=~RCC_AHB1RSTR_GPIOERST;

	}

	else if (GPIOx==GPIOH){

		RCC->AHB1RSTR |=RCC_AHB1RSTR_GPIOHRST;
		RCC->AHB1RSTR &=~RCC_AHB1RSTR_GPIOHRST;
	}

}


uint8_t GPIO_ReadPin(GPIO_TypeDef* GPIOx,GPIO_PinNumber PinNumber){
	uint8_t value;

	value =(GPIOx->IDR>>PinNumber)&0x1U;

	return value;
}


uint16_t GPIO_ReadPort(GPIO_TypeDef* GPIOx){
	return GPIOx->IDR;
}

void GPIO_WritePin(GPIO_TypeDef* GPIOx,GPIO_PinNumber PinNumber,uint8_t PinState){
	if(PinState){
		//Write zero
		GPIOx->ODR |= (0x1U<<PinNumber);
	}
	else{
		//Clear Pin
		GPIOx->ODR &= ~(0x1U<<PinNumber);
	}

}
void GPIO_WritePort(GPIO_TypeDef* GPIOx,uint16_t Value){
	GPIOx->ODR = Value;

}

void GPIO_TogglePin(GPIO_TypeDef* GPIOx,GPIO_PinNumber PinNumber){
	GPIOx->ODR ^= (0x01U<<PinNumber);
}


void GPIO_IRQConfig(IRQn_Type IRQn,uint8_t PriorityNumber,uint8_t EnOrDi){
	if (EnOrDi){
		NVIC_EnableIRQ(IRQn);
		NVIC_SetPriority(IRQn,PriorityNumber);
	}
	else{
		NVIC_DisableIRQ(IRQn);
	}

}

void GPIO_ISRHandling(GPIO_PinNumber PinNumber){
	//Clear Pending pin
	if (EXTI->PR & (1U<< PinNumber)){
		EXTI->PR |= (0x01U<<PinNumber);

	}

}
