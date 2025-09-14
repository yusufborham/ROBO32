/*
 * Servo_ex.c
 *
 *  Created on: Sep 14, 2025
 *      Author: Koshok
 */
/*

#include "Includes_int.h"
#include <stdio.h>
#include <string.h>



int main(void){
	MRCC_vInit();
	MRCC_vEnableClk(RCC_AHB1, RCC_GPIOA);

	SerMux_init(GPIO_PORTA, 0, 1);

	    // Set initial positions
	    SerMux_setAngle(0, 45);   // Servo 0 to 45 degrees


	    // Main loop - sweep servos back and forth
	    while (1) {
	        // Sweep from 0 to 180 degrees
	        for (u8 angle = 0; angle <= 180; angle += 10) {
	            SerMux_setAngle(0, 90);


	            for(volatile u32 i = 0; i < 100000; i++); // Simple delay
	        }

	        // Sweep back from 180 to 0 degrees
	        for (u8 angle = 180; angle > 0; angle -= 10) {
	            SerMux_setAngle(0, angle);

	            for(volatile u32 i = 0; i < 100000; i++); // Simple delay
	        }
	    }



	    return 0;
	}
*/
