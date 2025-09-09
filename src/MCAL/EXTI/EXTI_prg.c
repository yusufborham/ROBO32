/*
 * EXTI_prg.c
 *
 *  Created on: Aug 20, 2025
 *      Author: Nada Mamdouh
 *      Version: 0.0
 */
#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "EXTI_int.h"
#include "EXTI_prv.h"
#include "EXTI_cfg.h"

static void (*G_Fptr[16])(void) = {NULL};

void MEXTI_vInit(EXTI_Cng_t* A_xCfg)
{
	/*Enable all required EXTI */

	/* Clear flag */
	EXTI->PR = 0xFFFFFFFF;
}
void MEXTI_vEnableINT(u8 A_u8LineNo, u8 A_u8Trigger_t)
{
	if(A_u8LineNo < 16){
		/* Enable EXTI */
		SET_BIT(EXTI->IMR, A_u8LineNo);
		/* Set Trigger Type */
		switch(A_u8Trigger_t)
		{
		case EXTI_RISING_EDGE:
			SET_BIT(EXTI->RTSR, A_u8LineNo);
			CLR_BIT(EXTI->FTSR, A_u8LineNo);
			break;
		case EXTI_FALLING_EDGE:
			CLR_BIT(EXTI->RTSR, A_u8LineNo);
			SET_BIT(EXTI->FTSR, A_u8LineNo);
			break;
		case EXTI_ONCHANGE_EDGE:
			SET_BIT(EXTI->RTSR, A_u8LineNo);
			SET_BIT(EXTI->FTSR, A_u8LineNo);
			break;
		}
	}
}
void MEXTI_vDisableINT(u8 A_u8LineNo)
{
	if(A_u8LineNo < 16){
		/* Disable EXTI */
		CLR_BIT(EXTI->IMR, A_u8LineNo);
	}
}
void MEXTI_vSetTrigger(u8 A_u8LineNo, u8 A_u8Trigger_t)
{
	if(A_u8LineNo < 16){
		/* Set Trigger Type */
		switch(A_u8Trigger_t)
		{
		case EXTI_RISING_EDGE:
			SET_BIT(EXTI->RTSR, A_u8LineNo);
			CLR_BIT(EXTI->FTSR, A_u8LineNo);
			break;
		case EXTI_FALLING_EDGE:
			CLR_BIT(EXTI->RTSR, A_u8LineNo);
			SET_BIT(EXTI->FTSR, A_u8LineNo);
			break;
		case EXTI_ONCHANGE_EDGE:
			SET_BIT(EXTI->RTSR, A_u8LineNo);
			SET_BIT(EXTI->FTSR, A_u8LineNo);
			break;
		}
	}
}
void MEXTI_vSoftwareINT (u8 A_u8LineNo)
{
	if(A_u8LineNo < 16){
		SET_BIT(EXTI->SWIER, A_u8LineNo);
	}
}

void MEXTI_vSetCallBack(void(*A_xFptr)(void), u8 A_u8LineNo)
{
	if(A_u8LineNo < 16){
		G_Fptr[A_u8LineNo] = A_xFptr;
	}
}

void EXTI0_IRQHandler(void)
{
	if(G_Fptr[EXTI_LINE0] != NULL)
	{
		// action
		G_Fptr[EXTI_LINE0]();
	}
	// clear pending flag
	SET_BIT(EXTI->PR,EXTI_LINE0);
}
void EXTI1_IRQHandler(void)
{
	if(G_Fptr[EXTI_LINE1] != NULL)
	{
		G_Fptr[EXTI_LINE1]();
	}
	// clear pending flag
	SET_BIT(EXTI->PR,EXTI_LINE1);
}
void EXTI9_5IRQHandler(void)
{
	if(GET_BIT(EXTI->PR, EXTI_LINE5))
	{
		if(G_Fptr[EXTI_LINE5] != NULL)
		{
			G_Fptr[EXTI_LINE5]();
			// clear pending flag
			SET_BIT(EXTI->PR,EXTI_LINE5);
		}
	}
	if(GET_BIT(EXTI->PR, EXTI_LINE6))
	{
		if(G_Fptr[EXTI_LINE6] != NULL)
		{
			G_Fptr[EXTI_LINE6]();
			// clear pending flag
			SET_BIT(EXTI->PR,EXTI_LINE6);
		}
	}

}

void MEXTI_vClearPendingFlag(u8 A_u8LineNo){
	EXTI->PR |= (1 << A_u8LineNo);
}
