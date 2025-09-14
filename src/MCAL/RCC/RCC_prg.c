/*
 * RCC_prg.c
 *
 *  Created on: Aug 14, 2025
 *      Author: Mohamed Ismail
 */

#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "RCC_int.h"
#include "RCC_cfg.h"
#include "RCC_prv.h"

void MRCC_vInit(void){

#if RCC_SYS_CLK == HSE_CLK
	// CHOOSE Bypass
#if HSE_BYPASS == RC_CLK
	SET_BIT(RCC->CR, HSEBYP);
#elif HSE_BYPASS == MECHANICAL_CLK
	CLR_BIT(RCC->CR, HSEBYP);
#endif

	//HSE ON
	SET_BIT(RCC->CR,HSEON);
	// WAIT TILL FLAG IS SET
	while(GET_BIT(RCC->CR,HSERDY) == 0);
	// SWITCH TO HSE
	CLR_BIT(RCC->CFGR, 1);
	SET_BIT(RCC->CFGR, 0);

#elif RCC_SYS_CLK == HSI_CLK
	//HSI ON
		SET_BIT(RCC->CR,HSION);
		// WAIT TILL FLAG IS SET
		while(GET_BIT(RCC->CR,HSIRDY) == 0)
			;
		// SWITCH TO HSI
		CLR_BIT(RCC->CFGR, 1);
		CLR_BIT(RCC->CFGR, 0);

#elif RCC_SYS_CLK == PLL_CLK

#if PLL_SRC = HSE_CLK

#elif PLL_SRC = HSI_CLK

#endif

#else
#error "INVALID OPTION"


#endif
}

void MRCC_vEnableClk(u8 A_u8BusID, u8 A_u8PeriphID) {

    switch (A_u8BusID) {
    case RCC_AHB1:
        switch (A_u8PeriphID) {
        case RCC_GPIOA: SET_BIT(RCC->AHB1ENR, GPIOAEN); break;
        case RCC_GPIOB: SET_BIT(RCC->AHB1ENR, GPIOBEN); break;
        case RCC_GPIOC: SET_BIT(RCC->AHB1ENR, GPIOCEN); break;
        case RCC_GPIOD: SET_BIT(RCC->AHB1ENR, GPIODEN); break;
        case RCC_GPIOE: SET_BIT(RCC->AHB1ENR, GPIOEEN); break;
        case RCC_GPIOF: SET_BIT(RCC->AHB1ENR, GPIOFEN); break;
        case RCC_GPIOG: SET_BIT(RCC->AHB1ENR, GPIOGEN); break;
        case RCC_GPIOH: SET_BIT(RCC->AHB1ENR, GPIOHEN); break;
        case RCC_CRC:   SET_BIT(RCC->AHB1ENR, CRCEN_RCC);   break;
        case RCC_DMA1:  SET_BIT(RCC->AHB1ENR, DMA1EN);  break;
        case RCC_DMA2:  SET_BIT(RCC->AHB1ENR, DMA2EN);  break;
        }
        SET_BIT(RCC -> AHB1ENR ,A_u8PeriphID);
        break;

    case RCC_AHB2:
        switch (A_u8PeriphID) {
        case RCC_OTGFS: SET_BIT(RCC->AHB2ENR, OTGFSEN); break;
        }
        break;

    case RCC_APB1:
        switch (A_u8PeriphID) {
        case RCC_TIM2:   SET_BIT(RCC->APB1ENR, TIM2EN);   break;
        case RCC_TIM3:   SET_BIT(RCC->APB1ENR, TIM3EN);   break;
        case RCC_TIM4:   SET_BIT(RCC->APB1ENR, TIM4EN);   break;
        case RCC_TIM5:   SET_BIT(RCC->APB1ENR, TIM5EN);   break;
        case RCC_WWDG:   SET_BIT(RCC->APB1ENR, WWDGEN);   break;
        case RCC_SPI2:   SET_BIT(RCC->APB1ENR, SPI2EN);   break;
        case RCC_SPI3:   SET_BIT(RCC->APB1ENR, SPI3EN);   break;
        case RCC_USART2: SET_BIT(RCC->APB1ENR, USART2EN); break;
        case RCC_I2C1:   SET_BIT(RCC->APB1ENR, I2C1EN);   break;
        case RCC_I2C2:   SET_BIT(RCC->APB1ENR, I2C2EN);   break;
        case RCC_I2C3:   SET_BIT(RCC->APB1ENR, I2C3EN);   break;
        case RCC_PWR:    SET_BIT(RCC->APB1ENR, PWREN);    break;
        }
        break;

    case RCC_APB2:
        switch (A_u8PeriphID) {
        case RCC_TIM1:   SET_BIT(RCC->APB2ENR, TIM1EN);   break;
        case RCC_USART1: SET_BIT(RCC->APB2ENR, USART1EN); break;
        case RCC_USART6: SET_BIT(RCC->APB2ENR, USART6EN); break;
        case RCC_ADC1:   SET_BIT(RCC->APB2ENR, ADC1EN);   break;
        case RCC_SDIO:   SET_BIT(RCC->APB2ENR, SDIOEN);   break;
        case RCC_SPI1:   SET_BIT(RCC->APB2ENR, SPI1EN);   break;
        case RCC_SPI4:   SET_BIT(RCC->APB2ENR, SPI4EN);   break;
        case RCC_SYSCFG: SET_BIT(RCC->APB2ENR, SYSCFGEN); break;
        case RCC_TIM9:   SET_BIT(RCC->APB2ENR, TIM9EN);   break;
        case RCC_TIM10:  SET_BIT(RCC->APB2ENR, TIM10EN);  break;
        case RCC_TIM11:  SET_BIT(RCC->APB2ENR, TIM11EN);  break;
        }
        break;
    }
}

void MRCC_vDisableClk(u8 A_u8BusID, u8 A_u8PeriphID) {

    switch (A_u8BusID) {
    case RCC_AHB1:
        switch (A_u8PeriphID) {
        case RCC_GPIOA: CLR_BIT(RCC->AHB1ENR, GPIOAEN); break;
        case RCC_GPIOB: CLR_BIT(RCC->AHB1ENR, GPIOBEN); break;
        case RCC_GPIOC: CLR_BIT(RCC->AHB1ENR, GPIOCEN); break;
        case RCC_GPIOD: CLR_BIT(RCC->AHB1ENR, GPIODEN); break;
        case RCC_GPIOE: CLR_BIT(RCC->AHB1ENR, GPIOEEN); break;
        case RCC_GPIOF: CLR_BIT(RCC->AHB1ENR, GPIOFEN); break;
        case RCC_GPIOG: CLR_BIT(RCC->AHB1ENR, GPIOGEN); break;
        case RCC_GPIOH: CLR_BIT(RCC->AHB1ENR, GPIOHEN); break;
        case RCC_CRC:   CLR_BIT(RCC->AHB1ENR, CRCEN_RCC);   break;
        case RCC_DMA1:  CLR_BIT(RCC->AHB1ENR, DMA1EN);  break;
        case RCC_DMA2:  CLR_BIT(RCC->AHB1ENR, DMA2EN);  break;
        }
        break;

    case RCC_AHB2:
        switch (A_u8PeriphID) {
        case RCC_OTGFS: CLR_BIT(RCC->AHB2ENR, OTGFSEN); break;
        }
        break;

    case RCC_APB1:
        switch (A_u8PeriphID) {
        case RCC_TIM2:   CLR_BIT(RCC->APB1ENR, TIM2EN);   break;
        case RCC_TIM3:   CLR_BIT(RCC->APB1ENR, TIM3EN);   break;
        case RCC_TIM4:   CLR_BIT(RCC->APB1ENR, TIM4EN);   break;
        case RCC_TIM5:   CLR_BIT(RCC->APB1ENR, TIM5EN);   break;
        case RCC_WWDG:   CLR_BIT(RCC->APB1ENR, WWDGEN);   break;
        case RCC_SPI2:   CLR_BIT(RCC->APB1ENR, SPI2EN);   break;
        case RCC_SPI3:   CLR_BIT(RCC->APB1ENR, SPI3EN);   break;
        case RCC_USART2: CLR_BIT(RCC->APB1ENR, USART2EN); break;
        case RCC_I2C1:   CLR_BIT(RCC->APB1ENR, I2C1EN);   break;
        case RCC_I2C2:   CLR_BIT(RCC->APB1ENR, I2C2EN);   break;
        case RCC_I2C3:   CLR_BIT(RCC->APB1ENR, I2C3EN);   break;
        case RCC_PWR:    CLR_BIT(RCC->APB1ENR, PWREN);    break;
        }
        break;

    case RCC_APB2:
        switch (A_u8PeriphID) {
        case RCC_TIM1:   CLR_BIT(RCC->APB2ENR, TIM1EN);   break;
        case RCC_USART1: CLR_BIT(RCC->APB2ENR, USART1EN); break;
        case RCC_USART6: CLR_BIT(RCC->APB2ENR, USART6EN); break;
        case RCC_ADC1:   CLR_BIT(RCC->APB2ENR, ADC1EN);   break;
        case RCC_SDIO:   CLR_BIT(RCC->APB2ENR, SDIOEN);   break;
        case RCC_SPI1:   CLR_BIT(RCC->APB2ENR, SPI1EN);   break;
        case RCC_SPI4:   CLR_BIT(RCC->APB2ENR, SPI4EN);   break;
        case RCC_SYSCFG: CLR_BIT(RCC->APB2ENR, SYSCFGEN); break;
        case RCC_TIM9:   CLR_BIT(RCC->APB2ENR, TIM9EN);   break;
        case RCC_TIM10:  CLR_BIT(RCC->APB2ENR, TIM10EN);  break;
        case RCC_TIM11:  CLR_BIT(RCC->APB2ENR, TIM11EN);  break;
        }
        break;
    }
}

void MRCC_vSetAPB1Prescaler(u8 A_u8Prescaler){
	RCC->CFGR &= ~(0b111 << 10);			// clear the 3 bit field
	RCC->CFGR |= (A_u8Prescaler << 10);		// set the new prescaler value
}

void MRCC_vSetAPB2Prescaler(u8 A_u8Prescaler){
	RCC->CFGR &= ~(0b111 << 13);			// clear the 3 bit field
	RCC->CFGR |= (A_u8Prescaler << 13);		// set the new prescaler value
}

void MRCC_vSetAHBPrescaler(u8 A_u8Prescaler){
	RCC->CFGR &= ~(0b1111 << 4);			// clear the 4 bit field
	RCC->CFGR |= (A_u8Prescaler << 4);		// set the new prescaler value
}

void MRCC_vOutputClockOnHardwarePin(u8 A_u8MCO1_PRESCALER , u8 A_u8MCO1_SOURCE){
	// Set the MCO1 source and prescaler
	RCC->CFGR &= ~(0b111 << 24);				// Clear MCO1 bits
	RCC->CFGR |= (A_u8MCO1_PRESCALER << 24); 	// Set MCO1 PRESCALER

	RCC->CFGR &= ~(0b11 << 21); 				// Clear MCO1 source bits
	RCC->CFGR |= ( A_u8MCO1_SOURCE << 21); 		// Set MCO1 source
}
