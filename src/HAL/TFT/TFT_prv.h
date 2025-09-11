/*
 * TFT_prv.h
 * Private definitions for TFT driver
 *  Created on: Sep 2, 2025
 *      Author: drago
 */

#ifndef HAL_TFT_TFT_PRV_H_
#define HAL_TFT_TFT_PRV_H_

#include "../../LIB/STD_TYPES.h"

// Private function prototypes
static u16 HTFT_SwapBytes(u16 color);
static u8 HTFT_SendDataWithTimeout(u8* data, u16 size);
static void HTFT_vSendColorBulk(u16* colors, u32 count);
static void DrawLineCore(u16 x0, u16 y0, u16 x1, u16 y1, u16 color);

// Private constants
#define TFT_RESET_DELAY_MS      100
#define TFT_INIT_DELAY_MS       150
#define TFT_READY_DELAY_MS      50

#endif /* HAL_TFT_TFT_PRV_H_ */
