/*
 * SerMux_prg.c
 *
 *  Created on: Sep 9, 2025
 *      Author: Ahmed
 */

#include "SerMux_int.h"
#include "SerMux_prv.h"
#include "../../MCAL/RCC/RCC_int.h"
#include "../../MCAL/NVIC/NVIC_int.h"
#include "../../MCAL/GPIO/GPIO_int.h"

// ===================== Global Variables =====================
u8 g_port_id = GPIO_PORTA;
u8 g_first_pin = 0;
u8 g_num_servos = 0;

// ===================== Static Variables =====================
static u8  servo_angles[MAX_SERVOS];
static u8  current_servo = 0;
static u16 pulse_counter = 0;
static u16 target_pulse = 0;
static u16 slot_counter = 0;
static u16 slot_time_us = 0;
static u32 safety_counter = 0;

// ===================== Constants =====================
#define FRAME_TIME_US      20000u   // 20 ms frame
#define MIN_PULSE_US        500u
#define MAX_PULSE_US       2000u
#define SAFETY_TIMEOUT_US 100000u   // 100 ms safety timeout

// TIM2 clock = 25 MHz (HSE direct, no PLL)
#define TIMER_CLOCK_HZ  25000000UL
#define TICK_US         10          // 10 µs tick

// ===================== Helpers =====================
/**
 * @brief Convert angle (0-180°) to pulse width in µs (500–2000µs)
 */
static u16 angle_to_us(u8 angle) {
    if (angle > 180) angle = 180;
    return (u16)(MIN_PULSE_US + ((u32)angle * (MAX_PULSE_US - MIN_PULSE_US)) / 180u);
}

/**
 * @brief Initialize TIM2 for 10 µs tick with 25 MHz HSE clock
 */
static void Timer2_Init_10usTick(void) {
    // Enable TIM2 clock
    MRCC_vEnableClk(RCC_APB1, RCC_TIM2);

    // Prescaler: 25 MHz / 25 = 1 MHz → 1 µs tick
    TIM2_PSC = (TIMER_CLOCK_HZ / 1000000UL) - 1;  // = 24
    TIM2_ARR = TICK_US - 1;                       // = 9 → 10 µs period
    TIM2_CNT = 0;

    // Enable update interrupt
    TIM2_DIER |= TIM_UIE;

    // Start timer
    TIM2_CR1 |= TIM_CEN;

    // Enable TIM2 interrupt in NVIC
    MNVIC_vEnableInterrupt(28);
}

// ===================== API Functions =====================
void SerMux_setAngle(u8 servo_index, u8 angle) {
    if (servo_index < g_num_servos) {
        if (angle > 180) angle = 180;
        servo_angles[servo_index] = angle;
    }
}

u8 SerMux_getAngle(u8 servo_index) {
    if (servo_index < g_num_servos) {
        return servo_angles[servo_index];
    }
    return 0;
}

void SerMux_init(u8 port_id, u8 first_pin, u8 num_servos) {
    if (num_servos > MAX_SERVOS) num_servos = MAX_SERVOS;
    if (first_pin > 15) first_pin = 15;
    if ((u32)first_pin + num_servos > 16U) {
        num_servos = (u8)(16U - first_pin);
    }
    if (num_servos == 0) return;

    g_port_id = port_id;
    g_first_pin = first_pin;
    g_num_servos = num_servos;

    // Each slot = 20ms / num_servos
    slot_time_us = FRAME_TIME_US / g_num_servos;

    for (u8 i = 0; i < g_num_servos; i++) {
        u8 pin = g_first_pin + i;

        MGPIO_vSetPinMode(g_port_id, pin, GPIO_MODE_OUTPUT);
        MGPIO_vSetPinOutputType(g_port_id, pin, GPIO_PUSHPULL);
        MGPIO_vSetPinOutputSpeed(g_port_id, pin, GPIO_MED_SPEED);
        MGPIO_vSetPinPull(g_port_id, pin, GPIO_NOPULL);

        MGPIO_vSetPinValue(g_port_id, pin, GPIO_LOW);
        servo_angles[i] = 90; // neutral
    }

    current_servo = 0;
    pulse_counter = 0;
    slot_counter  = 0;
    safety_counter = 0;

    Timer2_Init_10usTick();
}

void SerMux_stop(void) {
    for (u8 i = 0; i < g_num_servos; i++) {
        MGPIO_vSetPinValue(g_port_id, g_first_pin + i, GPIO_LOW);
    }
}

void SerMux_setMultipleAngles(u8* angles, u8 count) {
    u8 max_count = (count > g_num_servos) ? g_num_servos : count;
    for (u8 i = 0; i < max_count; i++) {
        SerMux_setAngle(i, angles[i]);
    }
}

u8 SerMux_getServoCount(void) {
    return g_num_servos;
}

// ===================== ISR =====================
void TIM2_IRQHandler(void) {
    if (TIM2_SR & TIM_UIF) {
        TIM2_SR &= ~TIM_UIF;

        safety_counter += TICK_US;
        if (safety_counter >= SAFETY_TIMEOUT_US) {
            SerMux_stop();
            safety_counter = 0;
            slot_counter = 0;
            pulse_counter = 0;
            current_servo = 0;
            return;
        }

        if (slot_counter == 0) {
            // Start pulse for current servo
            MGPIO_vSetPinValue(g_port_id, g_first_pin + current_servo, GPIO_HIGH);
            target_pulse = angle_to_us(servo_angles[current_servo]);
            pulse_counter = 0;
        }

        pulse_counter += TICK_US;
        slot_counter  += TICK_US;

        // End pulse
        if (pulse_counter >= target_pulse) {
            MGPIO_vSetPinValue(g_port_id, g_first_pin + current_servo, GPIO_LOW);
        }

        // End of slot → move to next servo
        if (slot_counter >= slot_time_us) {
            slot_counter = 0;
            pulse_counter = 0;
            current_servo = (current_servo + 1) % g_num_servos;

            if (current_servo == 0) {
                safety_counter = 0; // reset each frame
            }
        }
    }
}

// ===================== Cleanup =====================
void SerMux_deinit(void) {
    TIM2_CR1 &= ~TIM_CEN;
    TIM2_DIER &= ~TIM_UIE;
    MNVIC_vDisableInterrupt(28);

    SerMux_stop();

    g_num_servos = 0;
    current_servo = 0;
    pulse_counter = 0;
    slot_counter  = 0;
    safety_counter = 0;
}
