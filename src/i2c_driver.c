/*
 * i2c_driver.c
 *
 *  Created on: August 6, 2023
 *      Author: Malefetsane Lenka
 */

#include "i2c_driver.h"

#define APB1_CLOCKFREQ     16000000U

static void I2Cx_Start(I2C_TypeDef* I2Cx);
static void I2C_SentAddress(I2C_TypeDef* I2Cx, uint8_t address);
static void I2C_Stop(I2C_TypeDef* I2Cx);


static void I2C_Stop(I2C_TypeDef* I2Cx){
	I2Cx -> CR1 |= I2C_CR1_STOP;


}

void I2C_ClockConfig(I2C_TypeDef* I2Cx,uint8_t EnOrDi){
	if (EnOrDi){
		if (I2Cx==I2C1){
			RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
		}

		else if(I2Cx==I2C2){
			RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;

		}
		else if (I2Cx==I2C3){
			RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;

		}
	}
	else{
		if (I2Cx==I2C1){
			RCC->APB1ENR &= ~RCC_APB1ENR_I2C1EN;
		}

		else if(I2Cx==I2C2){
			RCC->APB1ENR &= ~RCC_APB1ENR_I2C2EN;

		}
		else if (I2Cx==I2C3){
			RCC->APB1ENR &= ~RCC_APB1ENR_I2C3EN;

	}
}

}



void I2C_Init(I2C_Handle_t* I2Cx_Handle){

		//FREQ configuration
		 I2Cx_Handle->I2Cx->CR2 |= (APB1_CLOCKFREQ/1000000);  //16 MHz Clock  SPeed
		//CCR Calculations
		 uint16_t CCR_Value=0;
		 uint8_t Trise=0;
		 if (I2Cx_Handle->I2C_Config.I2C_SCLSpeed<=I2C_SCLSPEED_SM100K){
			 //SM
			 CCR_Value = ((APB1_CLOCKFREQ)/(2*I2Cx_Handle->I2C_Config.I2C_SCLSpeed))&0xFFF;
			 Trise = (APB1_CLOCKFREQ/1000000U)+1;

		 }
		 else{
			 //FM
			 I2Cx_Handle->I2Cx->CCR|= I2C_CCR_FS;
			 I2Cx_Handle->I2Cx->CCR |= (I2Cx_Handle->I2C_Config.I2C_FMDutyCycle<<I2C_CCR_DUTY_Pos);
			 Trise = (APB1_CLOCKFREQ*3)/(1000000U)+1;

			 if (I2Cx_Handle->I2C_Config.I2C_FMDutyCycle){
				 CCR_Value = ((APB1_CLOCKFREQ)/(3*I2Cx_Handle->I2C_Config.I2C_SCLSpeed))&0xFFF;

			 }

			 else{
				 CCR_Value = ((APB1_CLOCKFREQ)/(25*I2Cx_Handle->I2C_Config.I2C_SCLSpeed))&0xFFF;


			 }



		 }
		 I2Cx_Handle->I2Cx->CCR |= CCR_Value;

		 //Configure TRISE
		 I2Cx_Handle->I2Cx->TRISE &= ~(0x1F);
		 I2Cx_Handle->I2Cx->TRISE |= Trise&(0x1F);

		 //Enable I2C
		 I2Cx_Handle->I2Cx->CR1 |= I2C_CR1_PE;

}
void I2C_Master_Transmit(I2C_TypeDef* I2Cx,uint8_t address,uint8_t *pData,uint32_t Size){
	I2Cx_Start(I2Cx);  //Start Condition

	I2C_SentAddress(I2Cx,address<<1); //Write Condition
	while (Size){
		while (!(I2Cx->SR1 & I2C_SR1_TXE)); //Wait for Data Register to be empty

		I2Cx -> DR = *pData;       //Write to Data Rgister
		pData++;
		Size--;



	}
	//while (!(I2Cx->SR1 & I2C_SR1_BTF)); //wait until BTF is set
	I2C_Stop(I2Cx);




}
void I2C_Master_Recieve(I2C_TypeDef* I2Cx,uint8_t address,uint8_t *pData,uint32_t Size){
	volatile uint16_t temp;
	I2Cx_Start(I2Cx);  //Start Condition
	if (Size==1){
		address = (address<<1)|0x01; //Sent address with 1 to indicate r
		I2C_SentAddress(I2Cx, address);

		I2Cx -> CR1 &= ~(I2C_CR1_ACK);  //Sent NACK

		I2C_Stop(I2Cx);

		while (!(I2Cx->SR1 & I2C_SR1_RXNE)); //Wait until Receive buffer is not empty

		*pData = I2Cx->DR; // Load data to receiving array


	}
	else {
		I2C_SentAddress(I2Cx,(address <<1|0x01));

		while (Size > 3){
			while (!(I2Cx->SR1 & I2C_SR1_RXNE)); //wait until RXNE is set

			*pData = I2Cx->DR; // Load data to receiving array
			I2Cx-> CR1 |= I2C_CR1_ACK;  // Acknowledge
			pData++;
			Size--;


		}
		while (!(I2Cx->SR1 & I2C_SR1_BTF)); //wait until BTF is set
		I2Cx -> CR1 &= ~(I2C_CR1_ACK);  // Sent a NACK

		*pData = I2Cx->DR; // Load data to receiving array
		pData++;
		Size--;
		while (!(I2Cx->SR1 & I2C_SR1_BTF)); //wait until BTF is set
		I2C_Stop(I2Cx);

		*pData = I2Cx->DR; // Load data to receiving array
		 pData++;

		Size--;
		// Read Last Byte

		*pData = I2Cx->DR; // Load data to receiving array


	}

}
void I2C_Mem_Read(I2C_TypeDef* I2Cx,uint8_t address,uint16_t Address,uint16_t Mem_Size,uint8_t *pData,uint16_t Size){
	if (Mem_Size==1){
		I2Cx_Start(I2Cx);
		I2C_SentAddress(I2Cx,address<<1); //Write Condition
		while (!(I2Cx->SR1 & I2C_SR1_TXE)); //wait until RXNE is set
		I2Cx -> DR = (uint8_t)(Address&0xFF);

	}
	else{
			I2Cx_Start(I2Cx);
			I2C_SentAddress(I2Cx, address<<1); //Write Condition
			while (!(I2Cx->SR1 & I2C_SR1_TXE));
			I2Cx -> DR = (uint8_t) (Address >>8);

			while (!(I2Cx->SR1 & I2C_SR1_TXE));
			I2Cx -> DR = (uint8_t) (Address&0xFF);
	}
	I2C_Master_Recieve(I2Cx,address, pData, Size);

}

static void I2Cx_Start(I2C_TypeDef* I2Cx){
	volatile uint16_t temp;
	I2Cx->CR1 |= I2C_CR1_ACK;
	I2Cx -> CR1 |= I2C_CR1_START; //Set Start Condition
	while(!(I2Cx->SR1 & I2C_SR1_SB));  //Wait Until  Start Bit is Generated
	temp = I2Cx -> SR1;  // Read Status Register to Clear SB BIT


}

static void I2C_SentAddress(I2C_TypeDef* I2Cx, uint8_t address){
	volatile uint16_t temp;
	I2Cx-> DR = address ;   //Either from Read/Write function its already left shifted
	while (!(I2Cx->SR1 & I2C_SR1_ADDR));  //Successful Address sent
	temp =  I2Cx->SR1 | I2Cx-> SR2;


}

