/*
 * Seven_Segment.h
 *
 *  Created on: Aug 18, 2025
 *      Author: drago
 */

#ifndef HAL_SEVEN_SEGMENT_INT_H_
#define HAL_SEVEN_SEGMENT_INT_H_

#include "../../LIB/STD_TYPES.h"
#include "../../MCAL/GPIO/GPIO_int.h"

typedef struct{
	u8 Port;
	u8 PinNo[8];
}Segment_Init_t;


void _7_Segment_Init(Segment_Init_t* SEG);
void _7_Segment_Write(Segment_Init_t* SEG,u8 value);
void DisplayHexNumber(u8 hexValue, Segment_Init_t *LCD1, Segment_Init_t *LCD2);




#endif /* HAL_SEVEN_SEGMENT_INT_H_ */
