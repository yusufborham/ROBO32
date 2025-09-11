/*
#include "Includes_int.h"
#include <stdio.h>
#include <string.h>

int main(void) {
    // Initialize system clock first
    MRCC_vInit();

    // Enable clocks for GPIO and TIM1
    MRCC_vEnableClk(RCC_AHB1, GPIOAEN);
    MRCC_vEnableClk(RCC_APB2, TIM1EN); // Enable clock for TIM1

    // Add small delay after clock enable (good practice)
    for(volatile int i = 0; i < 1000; i++);

    // Configure PA8 for TIM1_CH1 PWM output
    GPIOx_PinConfig_t pwmPin = {
        .port = GPIO_PORTA,
        .pin = PIN8,
        .mode = GPIO_MODE_ALTFUNC,
        .outputType = GPIO_PUSHPULL,
        .speed = GPIO_HIGH_SPEED,
        .pull = GPIO_NOPULL,
        .altFunc = GPIO_AF1_TIM1_TIM2
    };

    // Initialize the GPIO pin
    MGPIO_vPinInit(&pwmPin);

    GPIOx_PinConfig_t pwmPin1 = {
            .port = GPIO_PORTA,
            .pin = PIN9,
            .mode = GPIO_MODE_ALTFUNC,
            .outputType = GPIO_PUSHPULL,
            .speed = GPIO_HIGH_SPEED,
            .pull = GPIO_NOPULL,
            .altFunc = GPIO_AF1_TIM1_TIM2
        };

        // Initialize the GPIO pin
        MGPIO_vPinInit(&pwmPin1);

    // PWM configuration structure
    PWM_Config_t pwmConfig = {
        .PWM_Mode = PWM_FAST_PWM,      // Use fast PWM mode
        .PWM_InitialDuty = 0           // Start at 0% duty cycle
    };

    // Initialize PWM
    PWM_vInit(&pwmConfig);

    // Add delay to ensure PWM is fully initialized
    for(volatile int i = 0; i < 10000; i++);

    // Test: Sweep PWM duty cycle from 0% to 100% and back
    while (1) {
        // Increase duty cycle
        for (u8 duty = 0; duty <= 100; duty += 10) {
            PWM_vSetDutyCycle_Channel(1, duty);
            PWM_vSetDutyCycle_Channel(2, duty);
            for (volatile int i = 0; i < 100000; i++); // Delay
        }
        // Decrease duty cycle
        for (u8 duty = 100; duty > 0; duty -= 10) {
            PWM_vSetDutyCycle_Channel(1, duty);
            PWM_vSetDutyCycle_Channel(2, duty);
            for (volatile int i = 0; i < 100000; i++); // Delay
        }
    }
}
*/
