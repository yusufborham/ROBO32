/*
 * RTC_cfg.h
 * Configuration for STM32F401CC RTC
 */

#ifndef MCAL_RTC_RTC_CFG_H_
#define MCAL_RTC_RTC_CFG_H_

/* -------- Select RTC Clock Source -------- */
#define RTC_CLOCK_SOURCE_LSE   0
#define RTC_CLOCK_SOURCE_LSI   1
#define RTC_CLOCK_SOURCE_HSE   2

/* Use HSE for F401CC (25 MHz external crystal) */
#define RTC_CLOCK_SOURCE   RTC_CLOCK_SOURCE_HSE

/* -------- Default Time & Date -------- */
#define RTC_DEFAULT_HOUR      12
#define RTC_DEFAULT_MINUTE    0
#define RTC_DEFAULT_SECOND    0

#define RTC_DEFAULT_YEAR      24   /* two digits: 00-99 */
#define RTC_DEFAULT_MONTH     9
#define RTC_DEFAULT_DAY       5
#define RTC_DEFAULT_WEEKDAY   4    /* 1=Mon ... 7=Sun */

#endif /* MCAL_RTC_RTC_CFG_H_ */
