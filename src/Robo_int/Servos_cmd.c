/*
 * Servos_cmd.c
 *
 *  Created on: Sep 14, 2025
 *      Author: Koshok
 */
#include "Servos_cmd.h"

void Servo_Init(void) {
    MRCC_vEnableClk(RCC_AHB1, GPIOAEN);
    MRCC_vEnableClk(RCC_AHB1, RCC_GPIOA);

	SerMux_init(GPIO_PORTA, 0, 2); // Assuming servos connected to PA0 and PA1
	    SerMux_setAngle(0, 90);   // Servo 0 to 45 degrees
    
}

void Servo_Steer(u8 angle) {
    SerMux_setAngle(0, angle); // Control servo at index 0
}

void Servo_Ultasonic(u8 angle) {
    SerMux_setAngle(1, angle); // Control servo at index 1
}