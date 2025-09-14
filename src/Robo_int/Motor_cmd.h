/*
 * Motor_cmd.h
 *
 *  Created on: Sep 14, 2025
 *      Author: Koshok
 */

#ifndef ROBO_INT_MOTOR_CMD_H_
#define ROBO_INT_MOTOR_CMD_H_

#include "../lib/BIT_MATH.h"
#include "../lib/STD_TYPES.h"
#include "includes_int.h"

void Motor_Init(void);

void Motor_FWD(u8 PWM);
void Motor_BWD(u8 PWM);
void Motor_Stop(void);

void Motor_Left_FWD(u8 PWM);
void Motor_Left_BWD(u8 PWM);

void Motor_Right_FWD(u8 PWM);
void Motor_Right_BWD(u8 PWM);

void Motor_Stop_Left(void);
void Motor_Stop_Right(void);

#endif /* ROBO_INT_MOTOR_CMD_H_ */
