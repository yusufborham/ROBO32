/*
 * RCC_prv.h
 *
 *  Created on: Aug 14, 2025
 *      Author: Mohamed Ismail
 */

#ifndef MCAL_RCC_RCC_PRV_H_
#define MCAL_RCC_RCC_PRV_H_


/* Base address for RCC */
#define RCC_BASE_ADDR    0x40023800U


typedef struct
{
    volatile u32 CR;
    volatile u32 PLLCFGR;
    volatile u32 CFGR;
    volatile u32 CIR;
    volatile u32 AHB1RSTR;
    volatile u32 AHB2RSTR;
    u32 RESERVED0[2];
    volatile u32 APB1RSTR;
    volatile u32 APB2RSTR;
    u32 RESERVED1[2];
    volatile u32 AHB1ENR;
    volatile u32 AHB2ENR;
    u32 RESERVED2[2];
    volatile u32 APB1ENR;
    volatile u32 APB2ENR;
    u32 RESERVED3[2];
    volatile u32 AHB1LPENR;
    volatile u32 AHB2LPENR;
    u32 RESERVED4[2];
    volatile u32 APB1LPENR;
    volatile u32 APB2LPENR;
    u32 RESERVED5[2];
    volatile u32 BDCR;
    volatile u32 CSR;
    u32 RESERVED6[2];
    volatile u32 SSCGR;
    volatile u32 PLLI2SCFGR;
    volatile u32 PLLSAICFGR;
    volatile u32 DCKCFGR;
} RCC_t;

#define RCC ((RCC_t*)RCC_BASE_ADDR)

/* ECC_SYS_CLK ----OPTIONS------ */
#define HSE_CLK 0
#define HSI_CLK 1
#define PLL_CLK 2

/* HSE_BYPASS ----OPTIONS------ */
#define RC_CLK 0
#define MECHANICAL_CLK 1

/*CR->BITS*/
#define HSEBYP 18
#define HSEON 16
#define HSERDY 17
#define HSION 0
#define HSIRDY 1

// RCC_CSR register bits
#define RCC_CSR_LSION      (1U << 0)
#define RCC_CSR_LSIRDY     (1U << 1)


/* AHB1ENR Bits */
#define GPIOAEN 0
#define GPIOBEN 1
#define GPIOCEN 2
#define GPIODEN 3
#define GPIOEEN 4
#define GPIOFEN 5
#define GPIOGEN 6
#define GPIOHEN 7
#define CRCEN_RCC   12
#define DMA1EN  21
#define DMA2EN  22

/* AHB2ENR Bits */
#define OTGFSEN 7

/* APB1ENR Bits */
#define TIM2EN   0
#define TIM3EN   1
#define TIM4EN   2
#define TIM5EN   3
#define WWDGEN   11
#define SPI2EN   14
#define SPI3EN   15
#define USART2EN 17
#define I2C1EN   21
#define I2C2EN   22
#define I2C3EN   23
#define PWREN    28

/* APB2ENR Bits */
#define TIM1EN    0
#define USART1EN  4
#define USART6EN  5
#define ADC1EN    8
#define SDIOEN    11
#define SPI1EN    12
#define SPI4EN    13
#define SYSCFGEN  14
#define TIM9EN    16
#define TIM10EN   17
#define TIM11EN   18

/* --- RCC_APB1ENR bits --- */
#define RCC_APB1ENR_PWREN      (1 << 28)   // Power interface clock enable

/* --- RCC_BDCR bits --- */
#define RCC_BDCR_BDRST         (1 << 16)   // Backup domain software reset

#define RCC_BDCR_RTCSEL_LSE    (0x1 << 8)  // LSE selected as RTC clock
#define RCC_BDCR_RTCSEL_LSI    (0x2 << 8)  // LSI selected as RTC clock
#define RCC_BDCR_RTCSEL_HSE    (0x3 << 8)  // HSE selected as RTC clock


/************ Backup Domain Control Register (RCC_BDCR) ************/
#define RCC_BDCR_LSEON_Pos     0U
#define RCC_BDCR_LSEON_Msk     (1U << RCC_BDCR_LSEON_Pos)
#define RCC_BDCR_LSEON         RCC_BDCR_LSEON_Msk   // Enable LSE oscillator

#define RCC_BDCR_LSERDY_Pos    1U
#define RCC_BDCR_LSERDY_Msk    (1U << RCC_BDCR_LSERDY_Pos)
#define RCC_BDCR_LSERDY        RCC_BDCR_LSERDY_Msk  // LSE Ready flag

#define RCC_BDCR_LSEBYP_Pos    2U
#define RCC_BDCR_LSEBYP_Msk    (1U << RCC_BDCR_LSEBYP_Pos)
#define RCC_BDCR_LSEBYP        RCC_BDCR_LSEBYP_Msk  // LSE Bypass

#define RCC_BDCR_RTCSEL_Pos    8U
#define RCC_BDCR_RTCSEL_Msk    (3U << RCC_BDCR_RTCSEL_Pos)
#define RCC_BDCR_RTCSEL        RCC_BDCR_RTCSEL_Msk  // RTC clock source selection

#define RCC_BDCR_RTCEN_Pos     15U
#define RCC_BDCR_RTCEN_Msk     (1U << RCC_BDCR_RTCEN_Pos)
#define RCC_BDCR_RTCEN         RCC_BDCR_RTCEN_Msk   // RTC clock enable

#define RCC_BDCR_BDRST_Pos     16U
#define RCC_BDCR_BDRST_Msk     (1U << RCC_BDCR_BDRST_Pos)
#define RCC_BDCR_




#endif /* MCAL_RCC_RCC_PRV_H_ */
