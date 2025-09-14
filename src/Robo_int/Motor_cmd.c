/*
 * Motor_cmd.c
 *
 *  Created on: Sep 14, 2025
 *      Author: Koshok
 */

#include"Motor_cmd.h"

void Motor_Init(void) {
    MRCC_vEnableClk(RCC_AHB1, GPIOAEN);

    GPIOx_PinConfig_t pwmPin = {
        .port = GPIO_PORTA,
        .pin = PIN8,
        .mode = GPIO_MODE_ALTFUNC,
        .outputType = GPIO_PUSHPULL,
        .speed = GPIO_HIGH_SPEED,
        .pull = GPIO_NOPULL,
        .altFunc = GPIO_AF1_TIM1_TIM2
    };
    GPIOx_PinConfig_t pwmPin1 = {
        .port = GPIO_PORTA,
        .pin = PIN9,
        .mode = GPIO_MODE_ALTFUNC,
        .outputType = GPIO_PUSHPULL,
        .speed = GPIO_HIGH_SPEED,
        .pull = GPIO_NOPULL,
        .altFunc = GPIO_AF1_TIM1_TIM2
    };
    MGPIO_vPinInit(&pwmPin);
    MGPIO_vPinInit(&pwmPin1);

    PWM_Config_t pwmConfig = {
        .PWM_Mode = PWM_FAST_PWM,
        .PWM_InitialDuty = 0
    };
    PWM_vInit(&pwmConfig);

    GPIOx_PinConfig_t Dir = {
        .port = GPIO_PORTA,
        .pin = PIN6,
        .mode = GPIO_MODE_OUTPUT,
        .outputType = GPIO_PUSHPULL,
        .speed = GPIO_LOW_SPEED,
        .pull = GPIO_NOPULL,
        .altFunc = 0  
    };
    GPIOx_PinConfig_t Dir1 = {
        .port = GPIO_PORTA,
        .pin = PIN7,
        .mode = GPIO_MODE_OUTPUT,
        .outputType = GPIO_PUSHPULL,
        .speed = GPIO_LOW_SPEED,
        .pull = GPIO_NOPULL,
        .altFunc = 0
    };
    MGPIO_vPinInit(&Dir);
    MGPIO_vPinInit(&Dir1);

    for(volatile int i = 0; i < 10000; i++);

}
void Motor_FWD(u8 PWM) {
    MGPIO_vSetPinValue(GPIO_PORTA, PIN6, GPIO_LOW);
    MGPIO_vSetPinValue(GPIO_PORTA, PIN7, GPIO_LOW);
    PWM_vSetDutyCycle_Channel(1, PWM);
    PWM_vSetDutyCycle_Channel(2, PWM);
}
void Motor_BWD(u8 PWM) {
    MGPIO_vSetPinValue(GPIO_PORTA, PIN6, GPIO_HIGH);
    MGPIO_vSetPinValue(GPIO_PORTA, PIN7, GPIO_HIGH);
    PWM_vSetDutyCycle_Channel(1, PWM);
    PWM_vSetDutyCycle_Channel(2, PWM);
}
void Motor_Stop(void) {
    PWM_vSetDutyCycle_Channel(1, 0);
    PWM_vSetDutyCycle_Channel(2, 0);
}
void Motor_Left_FWD(u8 PWM) {
    MGPIO_vSetPinValue(GPIO_PORTA, PIN6, GPIO_LOW);
    PWM_vSetDutyCycle_Channel(1, PWM);
}
void Motor_Left_BWD(u8 PWM) {
    MGPIO_vSetPinValue(GPIO_PORTA, PIN6, GPIO_HIGH);
    PWM_vSetDutyCycle_Channel(1, PWM);
}
void Motor_Right_FWD(u8 PWM) {
    MGPIO_vSetPinValue(GPIO_PORTA, PIN7, GPIO_LOW);
    PWM_vSetDutyCycle_Channel(2, PWM);
}
void Motor_Right_BWD(u8 PWM) {
    MGPIO_vSetPinValue(GPIO_PORTA, PIN7, GPIO_HIGH);
    PWM_vSetDutyCycle_Channel(2, PWM);
}
void Motor_Stop_Left(void) {
    PWM_vSetDutyCycle_Channel(1, 0);
}
void Motor_Stop_Right(void) {
    PWM_vSetDutyCycle_Channel(2, 0);
}


