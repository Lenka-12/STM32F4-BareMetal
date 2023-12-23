/*
 * i2c_driver.h
 *
 *  Created on: Aug 6, 2023
 *      Author: malef
 */

#ifndef I2C_DRIVER_H_
#define I2C_DRIVER_H_

#include "stm32f411xe.h"

/*
 *@I2C Configurable items
 */

typedef struct{
	uint32_t I2C_SCLSpeed;
	uint16_t I2C_FMDutyCycle;

}I2C_Config_t;

/*
 * @I2C Handle structure, used for initialization
 */
typedef struct{
	I2C_TypeDef* I2Cx;
	I2C_Config_t I2C_Config;

}I2C_Handle_t;

/*
 * @I2C_SCLSpeed
 */
#define I2C_SCLSPEED_SM100K        100000
#define I2C_SCLSPEED_SM10K         10000
#define I2C_SCLSPEED_FM            400000

/*
 * @I2C_AckControl
 */
#define I2C_ACK_ENABLE         1U
#define I2C_ACK_DISABLE        0U

/*
 * @I2C_FMDutyCycle
 *
 */
#define I2C_FM_DUTY2           0U
#define I2C_FM_DUTY16_9        1U

/*
* Peripheral clock SetUp
*/
void I2C_ClockConfig(I2C_TypeDef* I2Cx,uint8_t EnOrDi);

/*
* Init and Denit
*/
void I2C_Init(I2C_Handle_t* I2Cx_Handle);
void I2C_DeInit(I2C_TypeDef* I2Cx);

/*
* Data Send and Receive APIs
*/
void I2C_Master_Transmit(I2C_TypeDef* I2Cx,uint8_t address,uint8_t *pData,uint32_t Size);
void I2C_Master_Recieve(I2C_TypeDef* I2Cx,uint8_t address,uint8_t *pData,uint32_t Size);
void I2C_Mem_Read(I2C_TypeDef* I2Cx,uint8_t address,uint16_t Address,uint16_t Mem_Size,uint8_t *pData,uint16_t Size);

/*
* IRQ Configuration and ISR Handling
*
*/
void I2C_IRQConfig(IRQn_Type IRQn,uint8_t PriorityNumber,uint8_t EnOrDi);
void I2C_ISRHandling(I2C_Handle_t* I2C_Handle);


#endif /* I2C_DRIVER_H_ */
