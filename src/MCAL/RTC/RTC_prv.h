/*
 * RTC_prv.h
 *
 *  Created on: Sep 5, 2025
 *      Author: drago
 */

#ifndef MCAL_RTC_RTC_PRV_H_
#define MCAL_RTC_RTC_PRV_H_


#include "../../LIB/STD_TYPES.h"

#ifndef __IO
#define __IO volatile
#endif


/* ================= Base Addresses ================= */
#define PERIPH_BASE        0x40000000UL
#define AHB1PERIPH_BASE    (PERIPH_BASE + 0x00020000UL)
#define APB1PERIPH_BASE    (PERIPH_BASE + 0x00000000UL)
#define BACKUP_BASE        0x40002800UL

#define RCC_BASE           (AHB1PERIPH_BASE + 0x3800UL)
#define PWR_BASE           (APB1PERIPH_BASE + 0x7000UL)
#define RTC_BASE           BACKUP_BASE

/* ================= RCC Registers ================= */



/* ================= PWR Registers ================= */
typedef struct {
    __IO u32 CR;
    __IO u32 CSR;
} PWR_TypeDef;

#define PWR     ((PWR_TypeDef *) PWR_BASE)
#define PWR_CR_DBP     (1 << 8)

/* ================= RTC Registers ================= */
typedef struct {
    __IO u32 TR;     // time register
    __IO u32 DR;     // date register
    __IO u32 CR;     // control register
    __IO u32 ISR;    // init/status register
    __IO u32 PRER;   // prescaler
    __IO u32 WUTR;
    __IO u32 CALIBR;
    __IO u32 ALRMAR;
    __IO u32 ALRMBR;
    __IO u32 WPR;
    __IO u32 SSR;
    __IO u32 SHIFTR;
    __IO u32 TSTR;
    __IO u32 TSDR;
    __IO u32 TSSSR;
    __IO u32 CALR;
    __IO u32 TAFCR;
    __IO u32 ALRMASSR;
    __IO u32 ALRMBSSR;
    u32 RESERVED7[2];
    __IO u32 BKP0R;
} RTC_TypeDef;


#define RTC     ((RTC_TypeDef *) RTC_BASE)

/* RTC_ISR bits */
#define RTC_ISR_RSF    (1 << 5)
#define RTC_ISR_INITF  (1 << 6)
#define RTC_ISR_INIT   (1 << 7)

/* ================= Helpers ================= */
#define ENTER_INIT_MODE()   do { RTC->ISR |= RTC_ISR_INIT; while(!(RTC->ISR & RTC_ISR_INITF)); } while(0)
#define EXIT_INIT_MODE()    do { RTC->ISR &= ~RTC_ISR_INIT; } while(0)
#endif /* MCAL_RTC_RTC_PRV_H_ */
