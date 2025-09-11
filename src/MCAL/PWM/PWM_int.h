/*
 * PWM_int.h
 *
 *  Created on: Sep 11, 2025
 *      Author: Koshok
 */

#ifndef MCAL_PWM_PWM_INT_H_
#define MCAL_PWM_PWM_INT_H_
#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"
#include "PWM_cfg.h"
#include "PWM_prv.h"
#include "../RCC/RCC_int.h"
#include "../RCC/RCC_prv.h"
#include "../NVIC/NVIC_int.h"
#include "../GPIO/GPIO_int.h"

typedef struct
{
    u8 PWM_Mode;          // PWM Mode: Fast PWM or Phase Correct PWM
    u8 PWM_Prescaler;     // Clock Prescaler
    u8 PWM_InitialDuty;   // Initial Duty Cycle (0-100)
}PWM_Config_t;



void PWM_vInit(PWM_Config_t* Config);
void PWM_vSetDutyCycle_Channel(u8 Channel, u8 DutyCycle);


#define PWM_FAST_PWM        0
#define PWM_PHASE_CORRECT   1



#endif /* MCAL_PWM_PWM_INT_H_ */
