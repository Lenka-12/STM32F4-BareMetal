#include "stm32f411xe.h"

const uint32_t DELAY = 1000000;
uint8_t flag=0;
int main(void){
    RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOAEN |RCC_AHB1ENR_GPIOCEN);
    RCC ->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    GPIOC->MODER &= ~(GPIO_MODER_MODER13);
    GPIOA->MODER |= GPIO_MODER_MODER5_0;
    //GPIOC->PUPDR |= GPIO_PUPDR_PUPDR13_0;
    SYSCFG->EXTICR[3] &= ~(0xF<<4);
    SYSCFG->EXTICR[3] |= (0x2<<4);
    EXTI->IMR  |= EXTI_IMR_IM13;
    EXTI->FTSR |= EXTI_FTSR_TR13;
    EXTI->RTSR &= ~EXTI_RTSR_TR13;
   
    NVIC_SetPriority(EXTI15_10_IRQn,0);
    NVIC_EnableIRQ(EXTI15_10_IRQn);
    
         for (;;){
         if (flag){
         	 GPIOA->ODR   ^= GPIO_ODR_ODR_5;
		 for (uint32_t i=0; i<DELAY; i++){
		    __asm volatile("NOP");
		 }
         }
       
    }


}

void EXTI15_10_IRQHandler(void){
        EXTI->PR |= EXTI_PR_PR13;
	flag = 1;

}
