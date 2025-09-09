/*
 * BTN.c
 *
 *  Created on: Aug 18, 2025
 *      Author: drago
 */

#include "../../LIB/BIT_MATH.h"
#include "../../MCAL/GPIO/GPIO_int.h"
#include "BTN_int.h"

void BTN_Init(BTN_Init_t* BTN){
	GPIOx_PinConfig_t BTN_Pin = {
	        .port = BTN->Port,
	        .pin =	BTN->PinNo,
	        .mode = GPIO_MODE_INPUT,
	        .outputType = GPIO_PUSHPULL,
	        .speed = GPIO_LOW_SPEED,
	        .pull = BTN->Pull,
	        .altFunc = 0
	    };
	MGPIO_vPinInit(&BTN_Pin);
}

u8 BTN_State(BTN_Init_t* BTN){
	return MGPIO_u8GetPinValue(BTN->Port,BTN->PinNo);

}
