/*
 * PWM_prg.c
 *
 *  Created on: Sep 11, 2025
 *      Author: Koshok
 */
#include "PWM_int.h"
#include "PWM_prv.h"
#include "PWM_cfg.h"

void PWM_vInit(PWM_Config_t* Config){
	MRCC_vEnableClk(RCC_APB2, TIM1EN); // Enable clock for TIM1
    // Timer configuration
    TIM1->PSC = 24;  // Prescaler = 24 (assuming 25 MHz input)
    TIM1->ARR = 999; // ARR = 999 (1 kHz PWM frequency)

    // Scale initial duty cycle (0-100%) to ARR (0-999)
    u16 scaledDuty = ((u32)Config->PWM_InitialDuty * 999) / 100;

    // Set initial duty cycle for all channels
    TIM1->CCR1 = scaledDuty;
    TIM1->CCR2 = scaledDuty;
    TIM1->CCR3 = scaledDuty;
    TIM1->CCR4 = scaledDuty;

    // Configure PWM mode - Clear mode bits first
    TIM1->CCMR1 &= ~((0x7 << 4) | (0x7 << 12));  // Clear OC1M and OC2M
    TIM1->CCMR2 &= ~((0x7 << 4) | (0x7 << 12));  // Clear OC3M and OC4M

    // Enable preload for compare registers
    TIM1->CCMR1 |= (1 << 3) | (1 << 11);  // OC1PE and OC2PE
    TIM1->CCMR2 |= (1 << 3) | (1 << 11);  // OC3PE and OC4PE

    switch (Config->PWM_Mode)
    {
    case PWM_FAST_PWM:
        // PWM mode 1: upcounting, active when TIMx_CNT < TIMx_CCRx
        TIM1->CCMR1 |= (0x6 << 4) | (0x6 << 12);   // OC1M and OC2M = 110
        TIM1->CCMR2 |= (0x6 << 4) | (0x6 << 12);   // OC3M and OC4M = 110
        break;
    case PWM_PHASE_CORRECT:
        // PWM mode 2: upcounting, active when TIMx_CNT > TIMx_CCRx
        TIM1->CCMR1 |= (0x7 << 4) | (0x7 << 12);   // OC1M and OC2M = 111
        TIM1->CCMR2 |= (0x7 << 4) | (0x7 << 12);   // OC3M and OC4M = 111
        break;
    default:
        // Default to PWM mode 1
        TIM1->CCMR1 |= (0x6 << 4) | (0x6 << 12);
        TIM1->CCMR2 |= (0x6 << 4) | (0x6 << 12);
        break;
    }

    // Enable capture/compare outputs
    TIM1->CCER |= (1 << 0);   // CC1E - Channel 1 enable
    TIM1->CCER |= (1 << 4);   // CC2E - Channel 2 enable
    TIM1->CCER |= (1 << 8);   // CC3E - Channel 3 enable
    TIM1->CCER |= (1 << 12);  // CC4E - Channel 4 enable

    // TIM1 is an advanced timer - need to enable main output
    TIM1->BDTR |= (1 << 15);  // MOE - Main Output Enable (critical for TIM1!)

    // Enable auto-reload preload
    TIM1->CR1 |= (1 << 7);    // ARPE

    // Generate update event to load preload values
    TIM1->EGR |= (1 << 0);    // UG - Update generation

    // Finally, enable the timer
    TIM1->CR1 |= (1 << 0);    // CEN - Counter enable
}

void PWM_vSetDutyCycle_Channel(u8 Channel, u8 DutyCycle){
    // Input validation
    if(DutyCycle > 100) DutyCycle = 100; // Cap duty cycle to 100%

    // Scale duty cycle to timer's ARR value
    u16 scaledDuty = ((u32)DutyCycle * 999) / 100;

    switch (Channel)
    {
    case 1:
        TIM1->CCR1 = scaledDuty;
        break;
    case 2:
        TIM1->CCR2 = scaledDuty;
        break;
    case 3:
        TIM1->CCR3 = scaledDuty;
        break;
    case 4:
        TIM1->CCR4 = scaledDuty;
        break;
    default:
        // Invalid channel - could add error handling here
        break;
    }
}

// Additional helper function to stop PWM
void PWM_vStop(void){
    TIM1->CR1 &= ~(1 << 0);  // Disable timer
    TIM1->BDTR &= ~(1 << 15); // Disable main output
}

// Additional helper function to start PWM
void PWM_vStart(void){
    TIM1->BDTR |= (1 << 15);  // Enable main output
    TIM1->CR1 |= (1 << 0);    // Enable timer
}
