/*
 * RGB.h
 *
 *  Created on: Aug 18, 2025
 *      Author: drago
 */

#ifndef HAL_RGB_INT_H_
#define HAL_RGB_INT_H_
#include "../../LIB/STD_TYPES.h"


typedef struct{
	u8 Port;
	u8 RED_Pin;
	u8 GREEN_Pin;
	u8 BLUE_Pin;
}RGB_Init_t;

void RGB_init(RGB_Init_t* RGB);
void RGB_Color(RGB_Init_t* RGB, u8 Red, u8 Green, u8 Blue);




#endif /* HAL_RGB_INT_H_ */
