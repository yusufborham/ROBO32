/*
 * IR_prv.h
 *
 *  Created on: Aug 30, 2025
 *      Author: drago
 */

#ifndef HAL_IR_IR_PRV_H_
#define HAL_IR_IR_PRV_H_



#include "../../LIB/STD_TYPES.h"

/* Internal state structure */
typedef struct
{
    volatile u8 startFlag;
    volatile u8 edgeCount;
    volatile u8 lastCode;
    volatile u32 edgesUs[IR_BUFFER_LEN];
} IR_State_t;

extern IR_State_t g_irState;

/* Internal handlers */
static void IR_EXTI_Handler(void);
static void IR_DecodeFrame(void);





#endif /* HAL_IR_IR_PRV_H_ */
