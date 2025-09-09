/*
2 * RCC_int.h
 *
 *  Created on: Aug 14, 2025
 *      Author: Mohamed Ismail
 */

#ifndef MCAL_RCC_RCC_INT_H_
#define MCAL_RCC_RCC_INT_H_

/* Buses */
#define RCC_AHB1 0
#define RCC_AHB2 1
#define RCC_APB1 2
#define RCC_APB2 3

// AHB1 peripherals
#define RCC_GPIOA   0
#define RCC_GPIOB   1
#define RCC_GPIOC   2
#define RCC_GPIOD   3
#define RCC_GPIOE   4
#define RCC_GPIOF   5
#define RCC_GPIOG   6
#define RCC_GPIOH   7
#define RCC_CRC     12
#define RCC_DMA1    21
#define RCC_DMA2    22

// AHB2 peripherals
#define RCC_OTGFS   7

// APB1 peripherals
#define RCC_TIM2    0
#define RCC_TIM3    1
#define RCC_TIM4    2
#define RCC_TIM5    3
#define RCC_WWDG    11
#define RCC_SPI2    14
#define RCC_SPI3    15
#define RCC_USART2  17
#define RCC_I2C1    21
#define RCC_I2C2    22
#define RCC_I2C3    23
#define RCC_PWR     28

// APB2 peripherals
#define RCC_TIM1    0
#define RCC_USART1  4
#define RCC_USART6  5
#define RCC_ADC1    8
#define RCC_SDIO    11
#define RCC_SPI1    12
#define RCC_SPI4    13
#define RCC_SYSCFG  14
#define RCC_TIM9    16
#define RCC_TIM10   17
#define RCC_TIM11   18

/**
 * @brief Prescaler options for APB1 bus.
 */
typedef enum {
    APB1_PRESCALER_DIVIDE_1   = 0, /**< HCLK not divided */
    APB1_PRESCALER_DIVIDE_2   = 4, /**< HCLK divided by 2 */
    APB1_PRESCALER_DIVIDE_4   = 5, /**< HCLK divided by 4 */
    APB1_PRESCALER_DIVIDE_8   = 6, /**< HCLK divided by 8 */
    APB1_PRESCALER_DIVIDE_16  = 7  /**< HCLK divided by 16 */
} APB1_Prescaler_t;

/**
 * @brief Prescaler options for APB2 bus.
 */
typedef enum {
    APB2_PRESCALER_DIVIDE_1   = 0, /**< HCLK not divided */
    APB2_PRESCALER_DIVIDE_2   = 4, /**< HCLK divided by 2 */
    APB2_PRESCALER_DIVIDE_4   = 5, /**< HCLK divided by 4 */
    APB2_PRESCALER_DIVIDE_8   = 6, /**< HCLK divided by 8 */
    APB2_PRESCALER_DIVIDE_16  = 7  /**< HCLK divided by 16 */
} APB2_Prescaler_t;

/**
 * @brief Prescaler options for AHB bus.
 */
typedef enum {
    AHB_PRESCALER_DIVIDE_1    = 0,  /**< SYSCLK not divided */
    AHB_PRESCALER_DIVIDE_2    = 8,  /**< SYSCLK divided by 2 */
    AHB_PRESCALER_DIVIDE_4    = 9,  /**< SYSCLK divided by 4 */
    AHB_PRESCALER_DIVIDE_8    = 10, /**< SYSCLK divided by 8 */
    AHB_PRESCALER_DIVIDE_16   = 11, /**< SYSCLK divided by 16 */
    AHB_PRESCALER_DIVIDE_64   = 12, /**< SYSCLK divided by 64 */
    AHB_PRESCALER_DIVIDE_128  = 13, /**< SYSCLK divided by 128 */
    AHB_PRESCALER_DIVIDE_256  = 14, /**< SYSCLK divided by 256 */
    AHB_PRESCALER_DIVIDE_512  = 15  /**< SYSCLK divided by 512 */
} AHB_Prescaler_t;

/*
 * MCO1 Clock Source Options
 */
typedef enum {
    MCO1_SOURCE_HSI   = 0,
    MCO1_SOURCE_LSE   = 1,
    MCO1_SOURCE_HSE   = 2,
    MCO1_SOURCE_PLL   = 3
} MCO1_Source_t;

typedef enum {
    MCO1_PRESCALER_DIVIDE_1   = 0,
    MCO1_PRESCALER_DIVIDE_2   = 4,
    MCO1_PRESCALER_DIVIDE_4   = 5,
    MCO1_PRESCALER_DIVIDE_8   = 6,
    MCO1_PRESCALER_DIVIDE_16  = 7
} MCO1_Prescaler_t;

void MRCC_vInit(void);
void MRCC_vEnableClk(u8 A_u8BusID, u8 A_u8PrepheralID);
void MRCC_vDisableClk(u8 A_u8BusID, u8 A_u8PrepheralID);

void MRCC_vDisableClk(u8 A_u8BusID, u8 A_u8PeripheralID);

void MRCC_vSetAPB1Prescaler(u8 A_u8Prescaler);

void MRCC_vSetAPB2Prescaler(u8 A_u8Prescaler);

void MRCC_vSetAHBPrescaler(u8 A_u8Prescaler);

void MRCC_vOutputClockOnHardwarePin(u8 A_u8MCO1_PRESCALER , u8 A_u8MCO1_SOURCE);

#endif /* MCAL_RCC_RCC_INT_H_ */
