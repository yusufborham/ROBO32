/*
 * BTN.h
 *
 *  Created on: Aug 18, 2025
 *      Author: drago
 */

#ifndef HAL_BTN_INT_H_
#define HAL_BTN_INT_H_
#include "../../LIB/STD_TYPES.h"

typedef struct{
	u8 Port;
	u8 PinNo;
	u8 Pull;
}BTN_Init_t;


void BTN_Init(BTN_Init_t* BTN);

u8 BTN_State(BTN_Init_t* BTN);



#endif /* HAL_BTN_INT_H_ */
