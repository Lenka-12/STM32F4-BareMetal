/*
 * spi_driver.c
 *
 *  Created on: Aug 5, 2023
 *      Author: Malefetsane Lenka
 */

#include "spi_driver.h"


/*
 * @Brief:  Configures Clock for given SPI
 * @Param: SPIx, SPI Register definition structure see @SPI_Typedef in MCU Header file
 * @Param: EnOrDi, Enable or Disable Macro see @Enable and Disable in MCU header file
 * @Retval: None
 */
void SPI_ClockConfig(SPI_TypeDef* SPIx,uint8_t EnOrDi){
	if (EnOrDi){
		if (SPIx==SPI1){
			RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
		}
		else if (SPIx==SPI2){
			RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
		}
		else if (SPIx==SPI3){
			RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;
		}
		else if (SPIx==SPI4){
			RCC->APB2ENR |= RCC_APB2ENR_SPI4EN;
		}

		else if (SPIx==SPI5){
			RCC->APB2ENR |= RCC_APB2ENR_SPI5EN;
		}

	}
	else{
		if (SPIx==SPI1){
			RCC->APB2ENR &= ~RCC_APB2ENR_SPI1EN;
		}
		else if (SPIx==SPI2){
			RCC->APB1ENR &= ~RCC_APB1ENR_SPI2EN;
		}
		else if (SPIx==SPI3){
			RCC->APB1ENR &= ~RCC_APB1ENR_SPI3EN;
		}
		else if (SPIx==SPI4){
			RCC->APB2ENR &= ~RCC_APB2ENR_SPI4EN;
		}

		else if (SPIx==SPI5){
			RCC->APB2ENR &= ~RCC_APB2ENR_SPI5EN;
		}

	}
}

/*
 *@Brief: Initializes SPI with given the SPI configuration settings
 */

void SPI_Init(SPI_Handle_t* SPIx_Handle){
	//1.Configure CR1

	//a. Configure device mode
	uint32_t tempreg = 0;
	tempreg |= (SPIx_Handle->SPI_Config.SPI_DeviceMode<<SPI_CR1_MSTR_Pos);
	//b. Configure BusConfig
	if(SPIx_Handle->SPI_Config.SPI_BusConfig==SPI_FULLDUPLEX){
		//BIDIMODE is cleared
		tempreg &= ~SPI_CR1_BIDIMODE;
	}

	else if(SPIx_Handle->SPI_Config.SPI_BusConfig==SPI_HALFDUPLEX){
		//Set Bidimode
		tempreg |= SPI_CR1_BIDIMODE;

	}
	else if(SPIx_Handle->SPI_Config.SPI_BusConfig==SPI_SIMPLEX_RXONLY){
		//BIDIMODE is cleared
		tempreg &= ~SPI_CR1_BIDIMODE;
		//RXONLY high
		tempreg |= SPI_CR1_RXONLY;

	}
	//c. Configure spi clock speed
	tempreg |= SPIx_Handle->SPI_Config.SPI_SclkSpeed<<SPI_CR1_BR_Pos;

	//d. Configure DFF
	tempreg |= SPIx_Handle->SPI_Config.SPI_DFF<<SPI_CR1_DFF_Pos;

	//e. configure CPOL and CPHA
	tempreg |= SPIx_Handle->SPI_Config.SPI_CPHA<<SPI_CR1_CPHA_Pos;
	tempreg |= SPIx_Handle->SPI_Config.SPI_CPOL<<SPI_CR1_CPOL_Pos;

	if (SPIx_Handle->SPI_Config.SPI_SSM){
		tempreg |= SPI_CR1_SSM|SPI_CR1_SSI;

	}

	else{
		//Clear SMM
		tempreg &= ~SPI_CR1_SSM;
		//Set SSOE
		SPIx_Handle->SPIx->CR2 |= SPI_CR2_SSOE;
	}
	SPIx_Handle->SPIx->CR1 |= tempreg;
	if (SPIx_Handle->SPI_Config.SPI_SSM){
		SPIx_Handle->SPIx->CR1 |= SPI_CR1_SPE; //Enable SPI

	}





}
/*
 * @Brief: Reset all Registers of given SPI
 * @Param: SPIx, SPI Register definition structure see @SPI_Typedef in MCU Header file
 * @Retval: None
 */
void SPI_DeInit(SPI_TypeDef* SPIx){
	if (SPIx==SPI1){

		RCC->APB2RSTR |= RCC_APB2RSTR_SPI1RST;
		RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI1RST;
	}
	else if (SPIx==SPI2){

		RCC->APB1RSTR |= RCC_APB1RSTR_SPI2RST;
		RCC->APB1RSTR &=~RCC_APB1RSTR_SPI2RST;
	}
	else if (SPIx==SPI3){
		RCC->APB1RSTR |= RCC_APB1RSTR_SPI3RST;
		RCC->APB1RSTR &=~RCC_APB1RSTR_SPI3RST;
	}
	else if (SPIx==SPI4){
		RCC->APB2RSTR |= RCC_APB2RSTR_SPI4RST;
		RCC->APB2RSTR &=~RCC_APB2RSTR_SPI4RST;

	}

	else if (SPIx==SPI5){
		RCC->APB2RSTR |= RCC_APB2RSTR_SPI5RST;
		RCC->APB2RSTR &=~RCC_APB2RSTR_SPI5RST;
	}


}
/*
 * @Brief: Sent data via SPI
 * @Param: SPIx, SPI Register definition structure see @SPI_Typedef in MCU Header file
 * @Param: pTxBuffer, Pointer to Transmit buffer
 * @Param: Size, Number of bytes to be transmitted
 * @Note: This does not check potential bus erros
 */
void SPI_SendData(SPI_TypeDef* SPIx,uint8_t* pTxBuffer,uint32_t Size){
	while(Size){
		//1.Wait until TXE is Set
		while (!(SPIx->SR&SPI_SR_TXE));
		//2.Check DFF bit

		if (SPIx->CR1 &SPI_CR1_DFF){
			//16 bit DFF
			SPIx->DR = *(uint16_t*)pTxBuffer;
			(uint16_t*)pTxBuffer++;
			Size-=2;
		}
		else{
			//8bit data
			SPIx->DR = *pTxBuffer;
			pTxBuffer++;
			Size--;
		}
	}

}

void SPI_RecieveData(SPI_TypeDef* SPIx,uint8_t* pRxBuffer,uint32_t Size){
	while(Size){
		//1.Wait until RXNE is Set
		while (!(SPIx->SR&SPI_SR_RXNE));
		//2.Check DFF bit

		if (SPIx->CR1 &SPI_CR1_DFF){
			//16 bit DFF
			*(uint16_t*)pRxBuffer = SPIx->DR ;

		    (uint16_t*)pRxBuffer++;
			Size-=2;
		}
		else{
			//8bit data
			*pRxBuffer=SPIx->DR ;
			pRxBuffer++;
			Size--;
		}
	}

}
