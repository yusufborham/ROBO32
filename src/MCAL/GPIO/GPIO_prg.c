/*
 * GPIO_prg.c
 *
 *  Created on: Aug 17, 2025
 *      Author: Mohamed Ismail
 */

#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "GPIO_int.h"
#include "GPIO_cfg.h"
#include "GPIO_prv.h"

void MGPIO_vSetPinMode(u8 A_u8PortId, u8 A_u8PinNo, u8 A_u8Mode)
{
    GPIOx_MemMap_t* GPIOx = NULL;
    if((A_u8PortId == GPIO_PORTA) && ((A_u8PinNo == 13 ) || (A_u8PinNo ==14 ) ||(A_u8PinNo == 15 )) ){

    }
    else if((A_u8PortId == GPIO_PORTB) && ((A_u8PinNo == 3 ) || (A_u8PinNo == 4 ))){

        }
    else{
    switch(A_u8PortId)
    {
    case GPIO_PORTA: GPIOx = ((GPIOx_MemMap_t*)GPIOA_BASE_ADDR); break;
    case GPIO_PORTB: GPIOx = ((GPIOx_MemMap_t*)GPIOB_BASE_ADDR); break;
    case GPIO_PORTC: GPIOx = ((GPIOx_MemMap_t*)GPIOC_BASE_ADDR); break;
    case GPIO_PORTD: GPIOx = ((GPIOx_MemMap_t*)GPIOD_BASE_ADDR); break;
    case GPIO_PORTE: GPIOx = ((GPIOx_MemMap_t*)GPIOE_BASE_ADDR); break;
    case GPIO_PORTH: GPIOx = ((GPIOx_MemMap_t*)GPIOH_BASE_ADDR); break;
    default: return;
    }

    GPIOx->MODER &= ~(0b11 << (A_u8PinNo * 2));

    GPIOx->MODER |= ((A_u8Mode) << (A_u8PinNo * 2));
    }
}

void MGPIO_vSetPinOutputType(u8 A_u8PortId, u8 A_u8PinNo, u8 A_u8OutputType){
    GPIOx_MemMap_t* GPIOx = NULL;
    if((A_u8PortId == GPIO_PORTA) && ((A_u8PinNo == 13 ) || (A_u8PinNo == 14 ) ||(A_u8PinNo == 15 )) ){

        }
        else if((A_u8PortId == GPIO_PORTB) && ((A_u8PinNo == 3 ) || (A_u8PinNo == 4 ))){

            }
        else{
    switch(A_u8PortId){
        case GPIO_PORTA: GPIOx = ((GPIOx_MemMap_t*)GPIOA_BASE_ADDR); break;
        case GPIO_PORTB: GPIOx = ((GPIOx_MemMap_t*)GPIOB_BASE_ADDR); break;
        case GPIO_PORTC: GPIOx = ((GPIOx_MemMap_t*)GPIOC_BASE_ADDR); break;
        case GPIO_PORTD: GPIOx = ((GPIOx_MemMap_t*)GPIOD_BASE_ADDR); break;
        case GPIO_PORTE: GPIOx = ((GPIOx_MemMap_t*)GPIOE_BASE_ADDR); break;
        case GPIO_PORTH: GPIOx = ((GPIOx_MemMap_t*)GPIOH_BASE_ADDR); break;
        default: return; // invalid port
    }

    if (A_u8OutputType == GPIO_PUSHPULL) {
        CLR_BIT(GPIOx->OTYPER, A_u8PinNo);
    } else if (A_u8OutputType == GPIO_OPENDRAIN) {
        SET_BIT(GPIOx->OTYPER, A_u8PinNo);
    }
        }
}




void MGPIO_vSetPinOutputSpeed(u8 A_u8PortId, u8 A_u8PinNo, u8 A_u8OutputSpeed){
	GPIOx_MemMap_t* GPIOx = NULL;
	if((A_u8PortId == GPIO_PORTA) && ((A_u8PinNo == 13 ) || (A_u8PinNo == 14 ) ||(A_u8PinNo == 15 )) ){

	    }
	    else if((A_u8PortId == GPIO_PORTB) && ((A_u8PinNo == 3 ) || (A_u8PinNo == 4 ))){

	        }
	    else{
	    switch(A_u8PortId)
	    {
	    case GPIO_PORTA: GPIOx = ((GPIOx_MemMap_t*)GPIOA_BASE_ADDR); break;
	    case GPIO_PORTB: GPIOx = ((GPIOx_MemMap_t*)GPIOB_BASE_ADDR); break;
	    case GPIO_PORTC: GPIOx = ((GPIOx_MemMap_t*)GPIOC_BASE_ADDR); break;
	    case GPIO_PORTD: GPIOx = ((GPIOx_MemMap_t*)GPIOD_BASE_ADDR); break;
	    case GPIO_PORTE: GPIOx = ((GPIOx_MemMap_t*)GPIOE_BASE_ADDR); break;
	    case GPIO_PORTH: GPIOx = ((GPIOx_MemMap_t*)GPIOH_BASE_ADDR); break;
	    default: return;
	    }

	    GPIOx->OSPEEDR &= ~(0b11 << (A_u8PinNo * 2));

	    GPIOx->OSPEEDR |= ((A_u8OutputSpeed) << (A_u8PinNo * 2));
	    }
}


void MGPIO_vSetPinPull(u8 A_u8PortId, u8 A_u8PinNo, u8 A_u8PullType){
	GPIOx_MemMap_t* GPIOx = NULL;
	if((A_u8PortId == GPIO_PORTA) && ((A_u8PinNo == 13 ) || (A_u8PinNo == 14 ) ||(A_u8PinNo == 15 )) ){

	    }
	    else if((A_u8PortId == GPIO_PORTB) && ((A_u8PinNo == 3 ) || (A_u8PinNo == 4 ))){

	        }
	    else{
		    switch(A_u8PortId)
		    {
		    case GPIO_PORTA: GPIOx = ((GPIOx_MemMap_t*)GPIOA_BASE_ADDR); break;
		    case GPIO_PORTB: GPIOx = ((GPIOx_MemMap_t*)GPIOB_BASE_ADDR); break;
		    case GPIO_PORTC: GPIOx = ((GPIOx_MemMap_t*)GPIOC_BASE_ADDR); break;
		    case GPIO_PORTD: GPIOx = ((GPIOx_MemMap_t*)GPIOD_BASE_ADDR); break;
		    case GPIO_PORTE: GPIOx = ((GPIOx_MemMap_t*)GPIOE_BASE_ADDR); break;
		    case GPIO_PORTH: GPIOx = ((GPIOx_MemMap_t*)GPIOH_BASE_ADDR); break;
		    default: return;
		    }
		    GPIOx->PUPDR &= ~(0b11 << (A_u8PinNo * 2));

		    GPIOx->PUPDR|= ((A_u8PullType) << (A_u8PinNo * 2));
	    }
}

void MGPIO_vSetPinValue(u8 A_u8PortId, u8 A_u8PinNo, u8 A_u8Value){
	GPIOx_MemMap_t* GPIOx = NULL;
	if((A_u8PortId == GPIO_PORTA) && ((A_u8PinNo == 13 ) || (A_u8PinNo == 14 ) ||(A_u8PinNo == 15 )) ){

	    }
	    else if((A_u8PortId == GPIO_PORTB) && ((A_u8PinNo == 3 ) || (A_u8PinNo == 4 ))){

	        }
	    else{
		    switch(A_u8PortId)
		    {
		    case GPIO_PORTA: GPIOx = ((GPIOx_MemMap_t*)GPIOA_BASE_ADDR); break;
		    case GPIO_PORTB: GPIOx = ((GPIOx_MemMap_t*)GPIOB_BASE_ADDR); break;
		    case GPIO_PORTC: GPIOx = ((GPIOx_MemMap_t*)GPIOC_BASE_ADDR); break;
		    case GPIO_PORTD: GPIOx = ((GPIOx_MemMap_t*)GPIOD_BASE_ADDR); break;
		    case GPIO_PORTE: GPIOx = ((GPIOx_MemMap_t*)GPIOE_BASE_ADDR); break;
		    case GPIO_PORTH: GPIOx = ((GPIOx_MemMap_t*)GPIOH_BASE_ADDR); break;
		    default: return;
		    }
		    if (A_u8Value == GPIO_HIGH) {
		          SET_BIT(GPIOx->ODR, A_u8PinNo);
		    } else {
		          CLR_BIT(GPIOx->ODR, A_u8PinNo);
		    }
	    }

}
void MGPIO_vTogPinValue(u8 A_u8PortID, u8 A_u8PinNum)
{
	switch(A_u8PortID)
	{
	case GPIO_PORTA:
		TOG_BIT(GPIOA -> ODR, A_u8PinNum);
		break;
	case GPIO_PORTB:
		TOG_BIT(GPIOB -> ODR, A_u8PinNum);
		break;
	case GPIO_PORTC:
		TOG_BIT(GPIOC -> ODR, A_u8PinNum);
		break;
	}
}


u8 MGPIO_u8GetPinValue(u8 A_u8PortId, u8 A_u8PinNo) {
    GPIOx_MemMap_t* GPIOx = NULL;

    if ((A_u8PortId == GPIO_PORTA) &&
        ((A_u8PinNo == 13) || (A_u8PinNo == 14) || (A_u8PinNo == 15)))
    {
        return 0;
    }
    else if ((A_u8PortId == GPIO_PORTB) &&
             ((A_u8PinNo == 3) || (A_u8PinNo == 4)))
    {
        return 0;
    }
    else
    {
        switch (A_u8PortId) {
            case GPIO_PORTA: GPIOx = (GPIOx_MemMap_t*)GPIOA_BASE_ADDR; break;
            case GPIO_PORTB: GPIOx = (GPIOx_MemMap_t*)GPIOB_BASE_ADDR; break;
            case GPIO_PORTC: GPIOx = (GPIOx_MemMap_t*)GPIOC_BASE_ADDR; break;
            case GPIO_PORTD: GPIOx = (GPIOx_MemMap_t*)GPIOD_BASE_ADDR; break;
            case GPIO_PORTE: GPIOx = (GPIOx_MemMap_t*)GPIOE_BASE_ADDR; break;
            case GPIO_PORTH: GPIOx = (GPIOx_MemMap_t*)GPIOH_BASE_ADDR; break;
            default: return 0; // Invalid port
        }

        return GET_BIT(GPIOx->IDR, A_u8PinNo);
    }
}



void MGPIO_vSetAlt(u8 A_u8PortId, u8 A_u8PinNo, u8 A_u8AFx)
{
    GPIOx_MemMap_t* GPIOx = NULL;

    // Invalid pins (debug/program pins)
    if ((A_u8PortId == GPIO_PORTA) &&
        ((A_u8PinNo == 13) || (A_u8PinNo == 14) || (A_u8PinNo == 15)))
    {
        return;
    }
    else if ((A_u8PortId == GPIO_PORTB) &&
             ((A_u8PinNo == 3) || (A_u8PinNo == 4)))
    {
        return;
    }
    else
    {
        switch (A_u8PortId) {
            case GPIO_PORTA: GPIOx = (GPIOx_MemMap_t*)GPIOA_BASE_ADDR; break;
            case GPIO_PORTB: GPIOx = (GPIOx_MemMap_t*)GPIOB_BASE_ADDR; break;
            case GPIO_PORTC: GPIOx = (GPIOx_MemMap_t*)GPIOC_BASE_ADDR; break;
            case GPIO_PORTD: GPIOx = (GPIOx_MemMap_t*)GPIOD_BASE_ADDR; break;
            case GPIO_PORTE: GPIOx = (GPIOx_MemMap_t*)GPIOE_BASE_ADDR; break;
            case GPIO_PORTH: GPIOx = (GPIOx_MemMap_t*)GPIOH_BASE_ADDR; break;
            default: return;
        }

        if (A_u8PinNo <= 7) {
            GPIOx->AFRL &= ~(0xF << (A_u8PinNo * 4));      // Clear
            GPIOx->AFRL |=  ((A_u8AFx & 0xF) << (A_u8PinNo * 4)); // Set
        } else if(A_u8PinNo >= 8 && A_u8PinNo < 16){
            GPIOx->AFRH &= ~(0xF << ((A_u8PinNo - 8) * 4));
            GPIOx->AFRH |=  ((A_u8AFx & 0xF) << ((A_u8PinNo - 8) * 4));
        }
    }
}

void MGPIO_vSetPinAtomic(u8 A_u8PortId, u8 A_u8PinNo, u8 A_u8Value)
{
    GPIOx_MemMap_t* GPIOx = NULL;

    // Protect debug pins (PA13, PA14, PA15) and PB3/PB4
    if ((A_u8PortId == GPIO_PORTA) &&
        ((A_u8PinNo == 13) || (A_u8PinNo == 14) || (A_u8PinNo == 15)))
    {
        return;
    }
    else if ((A_u8PortId == GPIO_PORTB) &&
             ((A_u8PinNo == 3) || (A_u8PinNo == 4)))
    {
        return;
    }

    switch (A_u8PortId) {
        case GPIO_PORTA: GPIOx = (GPIOx_MemMap_t*)GPIOA_BASE_ADDR; break;
        case GPIO_PORTB: GPIOx = (GPIOx_MemMap_t*)GPIOB_BASE_ADDR; break;
        case GPIO_PORTC: GPIOx = (GPIOx_MemMap_t*)GPIOC_BASE_ADDR; break;
        case GPIO_PORTD: GPIOx = (GPIOx_MemMap_t*)GPIOD_BASE_ADDR; break;
        case GPIO_PORTE: GPIOx = (GPIOx_MemMap_t*)GPIOE_BASE_ADDR; break;
        case GPIO_PORTH: GPIOx = (GPIOx_MemMap_t*)GPIOH_BASE_ADDR; break;
        default: return; // Invalid port
    }

    if (A_u8Value == GPIO_HIGH) {
        GPIOx->BSRR = (1U << A_u8PinNo);       // Set bit
    } else {
        GPIOx->BSRR = (1U << (A_u8PinNo + 16)); // Reset bit
    }
}
void MGPIO_vPinInit(GPIOx_PinConfig_t* A_xPinCfg) {
    MGPIO_vSetPinMode(A_xPinCfg->port, A_xPinCfg->pin, A_xPinCfg->mode);
    MGPIO_vSetPinOutputType(A_xPinCfg->port, A_xPinCfg->pin, A_xPinCfg->outputType);
    MGPIO_vSetPinOutputSpeed(A_xPinCfg->port, A_xPinCfg->pin, A_xPinCfg->speed);
    MGPIO_vSetPinPull(A_xPinCfg->port, A_xPinCfg->pin, A_xPinCfg->pull);
    if(A_xPinCfg->mode == GPIO_MODE_ALTFUNC) {
        MGPIO_vSetAlt(A_xPinCfg->port, A_xPinCfg->pin, A_xPinCfg->altFunc);
    }
}

