/*
 * IR_cfg.h
 *
 *  Created on: Aug 30, 2025
 *      Author: drago
 */

#ifndef HAL_IR_IR_CFG_H_
#define HAL_IR_IR_CFG_H_


#include "../../MCAL/GPIO/GPIO_int.h"
#include "../../MCAL/EXTI/EXTI_int.h"


#define IR_GPIO_PORT     GPIO_PORTA
#define IR_GPIO_PIN      PIN0

#define IR_EXTI_LINE     EXTI_LINE0
#define IR_NVIC_IRQ_NUM  6

#define IR_ENABLE_GPIO_CLK()   MRCC_vEnableClk(RCC_AHB1, IR_GPIO_PORT)
#define IR_ENABLE_SYSCFG_CLK() MRCC_vEnableClk(RCC_APB2, 14)


#define IR_TICK_TO_US(ticks)   ((u32)((ticks)/3.125f))

#define IR_ZERO_MIN_US   1000
#define IR_ZERO_MAX_US   1250
#define IR_ONE_MIN_US    2000
#define IR_ONE_MAX_US    2450

/* Data bits count */
#define IR_DATA_BITS     8
#define IR_BUFFER_LEN    50



#endif /* HAL_IR_IR_CFG_H_ */
