/*
 * GPIO_prv.h
 *
 *  Created on: Aug 17, 2025
 *      Author: Mohamed Ismail
 */

#ifndef MCAL_GPIO_PRV_H_
#define MCAL_GPIO_PRV_H_

#include "../../LIB/STD_TYPES.h"

/* -------------------- Base Addresses -------------------- */
#define GPIOA_BASE_ADDR   0x40020000U
#define GPIOB_BASE_ADDR   0x40020400U
#define GPIOC_BASE_ADDR   0x40020800U
#define GPIOD_BASE_ADDR   0x40020C00U
#define GPIOE_BASE_ADDR   0x40021000U
#define GPIOH_BASE_ADDR   0x40021C00U

/* -------------------- GPIO Registers -------------------- */
typedef struct{
	u32 MODER;
	u32 OTYPER;
	u32 OSPEEDR;
	u32 PUPDR;
	u32 IDR;
	u32 ODR;
	u32 BSRR;
	u32 LCKR;
	u32 AFRL;
	u32 AFRH;
}GPIOx_MemMap_t;

/* -------------------- GPIO Instances -------------------- */
#define GPIOA   ((volatile GPIOx_MemMap_t*) GPIOA_BASE_ADDR)
#define GPIOB   ((volatile GPIOx_MemMap_t*) GPIOB_BASE_ADDR)
#define GPIOC   ((volatile GPIOx_MemMap_t*) GPIOC_BASE_ADDR)
#define GPIOD   ((volatile GPIOx_MemMap_t*) GPIOD_BASE_ADDR)
#define GPIOE   ((volatile GPIOx_MemMap_t*) GPIOE_BASE_ADDR)
#define GPIOH   ((volatile GPIOx_MemMap_t*) GPIOH_BASE_ADDR)

#endif /* MCAL_GPIO_PRV_H_ */
