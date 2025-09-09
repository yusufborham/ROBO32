/*
 * RTC_int.h
 * Interface for STM32F401CC RTC
 */

#ifndef MCAL_RTC_RTC_INT_H_
#define MCAL_RTC_RTC_INT_H_

#include "../../LIB/STD_TYPES.h"

/* Time structure */
typedef struct {
    u8 hours;      /* 0–23 */
    u8 minutes;    /* 0–59 */
    u8 seconds;    /* 0–59 */
    u8 pm;         /* 0=AM/24h, 1=PM (optional) */
} RTC_Time_t;

/* Date structure */
typedef struct {
    u8 year;       /* 0–99 */
    u8 month;      /* 1–12 */
    u8 day;        /* 1–31 */
    u8 weekday;    /* 1=Mon ... 7=Sun */
} RTC_Date_t;

typedef struct {
    RTC_Time_t time;
    RTC_Date_t date;
} RTC_DateTime_t;

RTC_DateTime_t HRTC_vGetNow(void);
RTC_DateTime_t HRTC_vGetNowWithOffset(int offsetHours) ;


/* -------- API Prototypes -------- */
void HRTC_vInit(void);
void HRTC_vSetTime(RTC_Time_t *time);
void HRTC_vSetDate(RTC_Date_t *date);
void HRTC_vGetTime(RTC_Time_t *time);
void HRTC_vGetDate(RTC_Date_t *date);

#endif /* MCAL_RTC_RTC_INT_H_ */
