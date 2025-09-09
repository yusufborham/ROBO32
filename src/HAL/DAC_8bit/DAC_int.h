/*
 * DAC_int.h
 *
 *  Created on: Aug 27, 2025
 *      Author: Koshok
 */
#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"
#include "../../MCAL/GPIO/GPIO_int.h"
#include "../../MCAL/SYSTICK/SYSTICK_int.h"



#ifndef HAL_DAC_8BIT_DAC_INT_H_
#define HAL_DAC_8BIT_DAC_INT_H_

typedef struct{
	u8 Port;
	u8 pin[8];
}DAC_Config_t;


void HDAC_vInit(DAC_Config_t* DAC_cfg);
void HDAC_vSend(DAC_Config_t* DAC_cfg, u8* A_u8ptr, u32 index);



#endif /* HAL_DAC_8BIT_DAC_INT_H_ */
