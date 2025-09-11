/*
 * SerMux_int.h
 *
 *  Created on: Sep 9, 2025
 *      Author: Ahmed
 */

#ifndef APP_HAL_SERMUX_SERMUX_INT_H_
#define APP_HAL_SERMUX_SERMUX_INT_H_

#include "../../LIB/STD_TYPES.h"

void SerMux_init(u8 port_id, u8 first_pin, u8 num_servos);
void SerMux_setAngle(u8 servo_index, u8 angle);

#define MAX_SERVOS 2
#define SLOT_TIME_US 2500u

#endif /* APP_HAL_SERMUX_SERMUX_INT_H_ */
