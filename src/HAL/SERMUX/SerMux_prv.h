#ifndef SERMUX_PRV_H
#define SERMUX_PRV_H

// Configuration constants
#define MAX_SERVOS 8
#define FRAME_TIME_US 20000U     // 20ms frame period (50Hz)
#define SAFETY_TIMEOUT_US  (FRAME_TIME_US / MAX_SERVOS) // 50ms safety timeout

// Timer register definitions for STM32F4xx
#define TIM2_PSC    (*((volatile u32*)0x40000028))
#define TIM2_ARR    (*((volatile u32*)0x4000002C))
#define TIM2_CNT    (*((volatile u32*)0x40000024))
#define TIM2_CR1    (*((volatile u32*)0x40000000))
#define TIM2_DIER   (*((volatile u32*)0x4000000C))
#define TIM2_SR     (*((volatile u32*)0x40000010))

// Timer control bits
#define TIM_CEN     (1 << 0)   // Counter enable
#define TIM_UIE     (1 << 0)   // Update interrupt enable
#define TIM_UIF     (1 << 0)   // Update interrupt flag

#endif /* SERMUX_PRV_H */
