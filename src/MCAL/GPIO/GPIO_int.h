/*
 * GPIO_int.h
 *
 * Created on: Aug 16, 2025
 * Author: Mohamed Ismail
 */

#ifndef MCAL_GPIO_INT_H_
#define MCAL_GPIO_INT_H_

#include "../../LIB/STD_TYPES.h"

typedef struct {
    u8 port;
    u8 pin;
    u8 mode;
    u8 outputType;
    u8 speed;
    u8 pull;
    u16 altFunc;
} GPIOx_PinConfig_t;

/* ------------------- Port IDs ------------------- */
#define GPIO_PORTA   0
#define GPIO_PORTB   1
#define GPIO_PORTC   2
#define GPIO_PORTD   3
#define GPIO_PORTE   4
#define GPIO_PORTH   7

/* ------------------- Pin IDs ------------------- */
#define PIN0    0
#define PIN1    1
#define PIN2    2
#define PIN3    3
#define PIN4    4
#define PIN5    5
#define PIN6    6
#define PIN7    7
#define PIN8    8
#define PIN9    9
#define PIN10   10
#define PIN11   11
#define PIN12   12
#define PIN13   13
#define PIN14   14
#define PIN15   15

/* ------------------- Pin Modes ------------------- */
#define GPIO_MODE_INPUT          0b00
#define GPIO_MODE_OUTPUT         0b01
#define GPIO_MODE_ALTFUNC        0b10
#define GPIO_MODE_ANALOG         0b11

/* ------------------- Output Types ------------------- */
#define GPIO_PUSHPULL       0
#define GPIO_OPENDRAIN      1

/* ------------------- Output Speed ------------------- */
#define GPIO_LOW_SPEED      0b00
#define GPIO_MED_SPEED      0b01
#define GPIO_HIGH_SPEED     0b10
#define GPIO_VHIGH_SPEED    0b11

/* ------------------- Pull-up / Pull-down ------------------- */
#define GPIO_NOPULL         0b00
#define GPIO_PULLUP         0b01
#define GPIO_PULLDOWN       0b10

/* ------------------- Pin Values ------------------- */
#define GPIO_LOW            0
#define GPIO_HIGH           1

/*======================================================
 * Alternate Function (AF) Macros for STM32F401C
 *======================================================*/

/* AF0: System (SYS) functions */
#define GPIO_AF0_SYSTEM        0x0   // SYS_AF: MCO, RTC, etc.

/* AF1: TIM1 / TIM2 */
#define GPIO_AF1_TIM1_TIM2     0x1

/* AF2: TIM3 / TIM4 / TIM5 */
#define GPIO_AF2_TIM3_TIM4_TIM5 0x2

/* AF3: TIM9 / TIM10 / TIM11 */
#define GPIO_AF3_TIM9_TIM10_TIM11 0x3

/* AF4: I2C1 / I2C2 / I2C3 */
#define GPIO_AF4_I2C1_I2C2_I2C3 0x4

/* AF5: SPI1 / SPI2 */
#define GPIO_AF5_SPI1_SPI2     0x5

/* AF6: SPI3 */
#define GPIO_AF6_SPI3          0x6

/* AF7: USART1 / USART2 */
#define GPIO_AF7_USART1_USART2 0x7

/* AF8: USART6 */
#define GPIO_AF8_USART6        0x8

/* AF9: I2C2 / I2C3 / CAN1 */
#define GPIO_AF9_I2C2_I2C3_CAN1 0x9

/* AF10: OTG_FS (USB) */
#define GPIO_AF10_OTG_FS       0xA

/* AF11: (Not used in STM32F401C) */
#define GPIO_AF11_UNUSED       0xB

/* AF12: SDIO */
#define GPIO_AF12_SDIO         0xC

/* AF13: (Not used in STM32F401C) */
#define GPIO_AF13_UNUSED       0xD

/* AF14: (Not used in STM32F401C) */
#define GPIO_AF14_UNUSED       0xE

/* AF15: EVENTOUT */
#define GPIO_AF15_EVENTOUT     0xF



/* ------------------- Functions Prototypes ------------------- */
void MGPIO_vSetPinMode(u8 A_u8PortId, u8 A_u8PinNo, u8 A_u8Mode);
void MGPIO_vSetPinOutputType(u8 A_u8PortId, u8 A_u8PinNo, u8 A_u8OutputType);
void MGPIO_vSetPinOutputSpeed(u8 A_u8PortId, u8 A_u8PinNo, u8 A_u8Speed);
void MGPIO_vSetPinPull(u8 A_u8PortId, u8 A_u8PinNo, u8 A_u8PullType);
void MGPIO_vSetPinValue(u8 A_u8PortId, u8 A_u8PinNo, u8 A_u8Value);
u8   MGPIO_u8GetPinValue(u8 A_u8PortId, u8 A_u8PinNo);
void   MGPIO_vTogPinValue(u8 A_u8PortId, u8 A_u8PinNo);
void MGPIO_vSetAlt(u8 A_u8PortId, u8 A_u8PinNo, u8 A_u8AFx);
void MGPIO_vPinInit(GPIOx_PinConfig_t* A_xPinCfg);
void MGPIO_vSetPinAtomic(u8 A_u8PortId, u8 A_u8PinNo, u8 A_u8Value);

#endif /* MCAL_GPIO_INT_H_ */
