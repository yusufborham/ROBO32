/*
 * RCC_cfg.h
 *
 *  Created on: Aug 14, 2025
 *      Author: Mohamed Ismail
 */

#ifndef MCAL_RCC_RCC_CFG_H_
#define MCAL_RCC_RCC_CFG_H_

/* RCC_SYSCLK
 * 	OPTIONS
 * 		HSE_CLK
 * 		HSI_CLK
 * 		PLL_CLK
 */
#define RCC_SYSCLK_SRC  HSE_CLK


/* HSE_BYPASS
 * 	OPTIONS
 * 		RC_CLK
 * 		MECHANICAL_CLK
 */
#define HSE_BYPASS MECHANICAL_CL

#endif /* MCAL_RCC_RCC_CFG_H_ */
