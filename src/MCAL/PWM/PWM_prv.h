/*
 * PWM_prv.h
 *
 *  Created on: Sep 11, 2025
 *      Author: Koshok
 */

#ifndef MCAL_PWM_PWM_PRV_H_
#define MCAL_PWM_PWM_PRV_H_
#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#define TIM1_BASE_ADDRESS 0x40010000

typedef struct
{
    u32 CR1;
    u32 CR2;
    u32 SMCR;
    u32 DIER;
    u32 SR;
    u32 EGR;
    u32 CCMR1;
    u32 CCMR2;
    u32 CCER;
    u32 CNT;
    u32 PSC;
    u32 ARR;
    u32 RCR;
    u32 CCR1;
    u32 CCR2;
    u32 CCR3;
    u32 CCR4;
    u32 BDTR;
    u32 DCR;
   // u32 DMAR ;
}TIM1_MemMap_t;

#define TIM1 ((TIM1_MemMap_t *)TIM1_BASE_ADDRESS)


#endif /* MCAL_PWM_PWM_PRV_H_ */
