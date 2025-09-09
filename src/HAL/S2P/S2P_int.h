/*
 * S2P_int.h
 *
 *  Created on: Aug 28, 2025
 *      Author: Koshok
 */

#ifndef HAL_S2P_S2P_INT_H_
#define HAL_S2P_S2P_INT_H_

#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"
#include "../../MCAL/GPIO/GPIO_int.h"
#include "../../MCAL/SYSTICK/SYSTICK_int.h"


typedef struct{
	u8 DataPort;
	u8 DataPin;

	u8 ShiftCLKPort;
	u8 ShiftCLKPin;

	u8 LatchCLKPort;
	u8 LatchCLKPin;

}S2P_Config_t;


void S2P_vInit(S2P_Config_t* S2P_cfg);
void S2P_ShiftData(S2P_Config_t* S2P_cfg);
void S2P_LatchData(S2P_Config_t* S2P_cfg);
void S2P_SendData(S2P_Config_t* S2P_cfg,u32 A_u32Byte);

#endif /* HAL_S2P_S2P_INT_H_ */
