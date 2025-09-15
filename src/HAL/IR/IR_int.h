/*
 * IR_int.h
 *
 *  Created on: Aug 30, 2025
 *      Author: drago
 */

#ifndef HAL_IR_IR_INT_H_
#define HAL_IR_IR_INT_H_


#include "../../LIB/STD_TYPES.h"


#define IR_CODE_0      0x16
#define IR_CODE_1      0x0C
#define IR_CODE_2      0x18
#define IR_CODE_3      0x5E
#define IR_CODE_4      0x08
#define IR_CODE_5      0x1C
#define IR_CODE_6      0x5A
#define IR_CODE_7      0x42
#define IR_CODE_8      0x52
#define IR_CODE_9      0x4A

#define IR_CODE_PLUS   0x09
#define IR_CODE_MINUS  0x15

#define IR_CODE_RIGHT	0x43
#define IR_CODE_LEFT	0x40
#define IR_CODE_PAUSE	0x44

#define IR_CODE_EQ		0x07

#define IR_CODE_MUTE	0x47
#define IR_CODE_MODE	0x46
#define IR_CODE_POWER	0x45

#define IR_CODE_RPT	0x19
#define IR_CODE_USD	0x0D


typedef struct {
    u8 pin;
    u8 port;
    u8 activeState;
} IR_Config_t;

typedef void (*IR_Callback_t)(u8 code);

void IR_Init(IR_Callback_t cb);

u8   IR_GetLastCode(u8* outCode);

void IR_ClearLastCode(void);


#endif /* HAL_IR_IR_INT_H_ */
