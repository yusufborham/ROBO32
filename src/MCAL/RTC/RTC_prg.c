/*
 * RTC_prg.c
 * Implementation for STM32F401CC RTC with LSE
 */

#include "RTC_cfg.h"
#include "RTC_int.h"
#include "RTC_prv.h"
#include "../RCC/RCC_int.h"
#include "../RCC/RCC_prv.h"
#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

/* Helper functions */
static u8 dec_to_bcd(u8 val) { return ((val / 10) << 4) | (val % 10); }
static u8 bcd_to_dec(u8 val) { return ((val >> 4) * 10) + (val & 0x0F); }
void HRTC_vInit(void)
{
    /* Enable PWR clock */
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;

    /* Allow access to backup domain */
    PWR->CR |= PWR_CR_DBP;

    /* Reset backup domain once (only if you want fresh config) */
    RCC->BDCR |= RCC_BDCR_BDRST;
    RCC->BDCR &= ~RCC_BDCR_BDRST;

    /* Enable LSE */
    RCC->BDCR |= RCC_BDCR_LSEON;

    /* Wait for LSE ready with timeout */
    u32 timeout = 1000000;
    while (!(RCC->BDCR & RCC_BDCR_LSERDY) && timeout--)
        ;

    if (!(RCC->BDCR & RCC_BDCR_LSERDY)) {
        //  LSE failed – fall back to LSI
        RCC->CSR |= RCC_CSR_LSION;
        while (!(RCC->CSR & RCC_CSR_LSIRDY));
        RCC->BDCR &= ~(0x3 << 8);
        RCC->BDCR |= (0x2 << 8); // RTCSEL = LSI
    } else {
        // Use LSE
        RCC->BDCR &= ~(0x3 << 8);
        RCC->BDCR |= (0x1 << 8); // RTCSEL = LSE
    }

    /* Enable RTC */
    RCC->BDCR |= RCC_BDCR_RTCEN;

    /* Unlock RTC write protection */
    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;

    /* Enter init mode */
    RTC->ISR |= RTC_ISR_INIT;
    while (!(RTC->ISR & RTC_ISR_INITF));

    /* Set prescalers:
       - For 32.768 kHz LSE  (127, 255) = 1 Hz
       - For ~32 kHz LSI  (127, 249) ~ 1 Hz
    */
    if (RCC->BDCR & RCC_BDCR_LSERDY) {
        RTC->PRER = (127 << 16) | (255 << 0); // for LSE
    } else {
        RTC->PRER = (127 << 16) | (249 << 0); // for LSI fallback
    }

    /* Exit init mode */
    RTC->ISR &= ~RTC_ISR_INIT;

    /* Lock write protection */
    RTC->WPR = 0xFF;

    /* Sync registers */
    RTC->ISR &= ~RTC_ISR_RSF;
    while (!(RTC->ISR & RTC_ISR_RSF));
}


/* ========================== SET TIME ========================== */
void HRTC_vSetTime(RTC_Time_t *time)
{
    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;

    SET_BIT(RTC->ISR, 7);
    while(!(RTC->ISR & (1 << 6)));

    RTC->TR = (dec_to_bcd(time->hours)   << 16) |
              (dec_to_bcd(time->minutes) << 8)  |
              (dec_to_bcd(time->seconds));

    CLR_BIT(RTC->ISR, 7);
    RTC->WPR = 0xFF;
}

/* ========================== GET TIME ========================== */
void HRTC_vGetTime(RTC_Time_t *time)
{
    u32 tr = RTC->TR;

    time->hours   = bcd_to_dec((tr >> 16) & 0x3F);
    time->minutes = bcd_to_dec((tr >> 8)  & 0x7F);
    time->seconds = bcd_to_dec(tr & 0x7F);
}

/* ========================== SET DATE ========================== */
void HRTC_vSetDate(RTC_Date_t *date)
{
    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;

    SET_BIT(RTC->ISR, 7);
    while(!(RTC->ISR & (1 << 6)));

    RTC->DR = (dec_to_bcd(date->year)  << 16) |
              (dec_to_bcd(date->month) << 8)  |
              (dec_to_bcd(date->day)) |
              ((date->weekday & 0x7) << 13);

    CLR_BIT(RTC->ISR, 7);
    RTC->WPR = 0xFF;
}

/* ========================== GET DATE ========================== */
void HRTC_vGetDate(RTC_Date_t *date)
{
    u32 dr = RTC->DR;

    date->year    = bcd_to_dec((dr >> 16) & 0xFF);
    date->month   = bcd_to_dec((dr >> 8)  & 0x1F);
    date->day     = bcd_to_dec(dr & 0x3F);
    date->weekday = (dr >> 13) & 0x07; // binary
}

/* ========================== GET NOW ========================== */
RTC_DateTime_t HRTC_vGetNow(void)
{
    RTC_DateTime_t now;

    u32 tr = RTC->TR;
    u32 dr = RTC->DR;

    now.time.hours   = bcd_to_dec((tr >> 16) & 0x3F);
    now.time.minutes = bcd_to_dec((tr >> 8) & 0x7F);
    now.time.seconds = bcd_to_dec(tr & 0x7F);

    now.date.year    = bcd_to_dec((dr >> 16) & 0xFF);
    now.date.month   = bcd_to_dec((dr >> 8) & 0x1F);
    now.date.day     = bcd_to_dec(dr & 0x3F);
    now.date.weekday = (dr >> 13) & 0x07;

    return now;
}

/* ========================== GET NOW + OFFSET ========================== */
RTC_DateTime_t HRTC_vGetNowWithOffset(int offsetHours)
{
    RTC_DateTime_t now = HRTC_vGetNow();

    int h = now.time.hours + offsetHours;
    if (h >= 24) h -= 24;
    if (h < 0)   h += 24;

    now.time.hours = h;
    return now;
}
