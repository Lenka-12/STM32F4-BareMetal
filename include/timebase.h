/*
 * timebase.h
 *
 *  Created on: Sep 22, 2023
 *      Author: Malefetsane Lenka
 */

#ifndef TIMEBASE_H_
#define TIMEBASE_H_

#define MILLI_SEC_LOAD   16000
#define MAX_DELAY        0xFFFFFFFF

/******************************************************
 * Time Base APIs, please see c file for implementation
 ******************************************************/
uint32_t get_tick();
void timebase_init(void);
void delay(uint32_t msdelay);


#endif /* TIMEBASE_H_ */

