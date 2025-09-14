/*
 * Servos_cmd.h
 *
 *  Created on: Sep 14, 2025
 *      Author: Koshok
 */

#ifndef ROBO_INT_SERVOS_CMD_H_
#define ROBO_INT_SERVOS_CMD_H_

#include "../lib/BIT_MATH.h"
#include "../lib/STD_TYPES.h"
#include "includes_int.h"

void Servo_Init(void);
void Servo_Steer(u8 angle); // angle in degrees (0-180)
void Servo_Ultasonic(u8 angle); // angle in degrees (0-180)

#endif /* ROBO_INT_SERVOS_CMD_H_ */
