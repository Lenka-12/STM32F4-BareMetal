/*
 * spi.h
 *
 *  Created on: Aug 5, 2023
 *      Author: malef
 */

#ifndef SPI_H_
#define SPI_H_
#include "stm32f411xe.h"

/*
 * SPI clock SPeed
 */

typedef enum{
	SPI_BR2,
	SPI_BR4,
	SPI_BR8,
	SPI_BR16,
	SPI_BR32,
	SPI_BR64,
	SPI_BR128,
	SPI_BR256,

}SPI_BR_t;
/*
 * @Brief: SPI configuration structure
 */
typedef struct{
	uint8_t SPI_DeviceMode;
	uint8_t SPI_BusConfig;
	SPI_BR_t SPI_SclkSpeed;
	uint8_t SPI_DFF;
	uint8_t SPI_CPOL;
	uint8_t SPI_CPHA;
	uint8_t SPI_SSM;

}SPI_Config_t;

/*
 * @Brief: SPI Handle Structure
 */

typedef struct{
	SPI_TypeDef* SPIx;
	SPI_Config_t SPI_Config;

}SPI_Handle_t;

/*
 * @SPI Device Mode
 */
#define SPI_MASTER             1U
#define SPI_SLAVE              0U

/*
 * @SPI Bus Configuration
 */
#define SPI_FULLDUPLEX                    0U
#define SPI_HALFDUPLEX                    1U
#define SPI_SIMPLEX_RXONLY                2U




/*
 * @SPI_DFF
 */
#define SPI_DFF_16BITS        1U
#define SPI_DFF_8BITS         0U

/*
 *@CPOL
 */

#define SPI_CPOL_HIGH         1U
#define SPI_CPOL_LOW          0U

/*
 *@CPHA
 */
#define SPI_CPHA_HIGH         1U
#define SPI_CPHA_LOW          0U

/*
 * @SPI_SSM
 */
#define SPI_SSM_EN           1U
#define SPI_SSM_DI           0U

/****************************************************
 * SPI API Requirements
 ****************************************************/
/*
 * Peripheral clock SetUp
 */
void SPI_ClockConfig(SPI_TypeDef* SPIx,uint8_t EnOrDi);

/*
 * Init and Denit
 */
void SPI_Init(SPI_Handle_t* SPIx_Handle);
void SPI_DeInit(SPI_TypeDef* SPIx);

/*
 * Data Send and Recieve APIs
 */
void SPI_SendData(SPI_TypeDef* SPIx,uint8_t* pTxBuffer,uint32_t Size);
void SPI_RecieveData(SPI_TypeDef* SPIx,uint8_t* pRxBuffer,uint32_t Size);

/*
 * IRQ Configuration and ISR Handling
 *
 */
void SPI_IRQConfig(IRQn_Type IRQn,uint8_t PriorityNumber,uint8_t EnOrDi);
void SPI_ISRHandling(SPI_Handle_t* SPI_Handle);

#endif /* SPI_H_ */
