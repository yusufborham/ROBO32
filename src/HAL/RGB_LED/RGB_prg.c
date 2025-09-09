/*
 * RGB.c
 *
 *  Created on: Aug 18, 2025
 *      Author: drago
 */

#include "../../MCAL/GPIO/GPIO_int.h"
#include "RGB_int.h"


void RGB_init(RGB_Init_t* RGB){

    GPIOx_PinConfig_t REDledPin = {
        .port = RGB->Port,
        .pin = RGB->RED_Pin,
        .mode = GPIO_MODE_OUTPUT,
        .outputType = GPIO_PUSHPULL,
        .speed = GPIO_LOW_SPEED,
        .pull = GPIO_NOPULL,
        .altFunc = 0
    };
    GPIOx_PinConfig_t GREENledPin = {
            .port = RGB->Port,
            .pin = RGB->GREEN_Pin,
            .mode = GPIO_MODE_OUTPUT,
            .outputType = GPIO_PUSHPULL,
            .speed = GPIO_LOW_SPEED,
            .pull = GPIO_NOPULL,
            .altFunc = 0
        };
    GPIOx_PinConfig_t BLUEledPin = {
            .port = RGB->Port,
            .pin = RGB->BLUE_Pin,
            .mode = GPIO_MODE_OUTPUT,
            .outputType = GPIO_PUSHPULL,
            .speed = GPIO_LOW_SPEED,
            .pull = GPIO_NOPULL,
            .altFunc = 0
        };

    MGPIO_vPinInit(&REDledPin);
    MGPIO_vPinInit(&GREENledPin);
    MGPIO_vPinInit(&BLUEledPin);
}

void RGB_Color(RGB_Init_t* RGB, u8 Red, u8 Green, u8 Blue){
     MGPIO_vSetPinValue(RGB->Port, RGB->RED_Pin, Red);
     MGPIO_vSetPinValue(RGB->Port, RGB->GREEN_Pin, Green);
     MGPIO_vSetPinValue(RGB->Port, RGB->BLUE_Pin, Blue);
}

