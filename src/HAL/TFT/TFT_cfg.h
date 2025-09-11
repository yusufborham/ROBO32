/*
 * TFT_cfg.h
 * Configuration file for TFT driver
 *  Created on: Sep 2, 2025
 *      Author: drago
 */

#ifndef HAL_TFT_TFT_CFG_H_
#define HAL_TFT_TFT_CFG_H_

#include "TFT_int.h"
#include "../../LIB/STD_TYPES.h"

// SPI Communication Configuration
#define TFT_SPI_TIMEOUT_MS          100
#define TFT_TX_CHUNK_PIXELS         64
#define TFT_SWAP_BYTES_IN_TX        0
#define TFT_USE_HAL_SPI_HANDLE      1

// Display Configuration
#define TFT_DEFAULT_ROTATION        1  // 0=Portrait, 1=Landscape, 2=Portrait flip, 3=Landscape flip

// Performance Configuration
#define TFT_ENABLE_FAST_FILL        1  // Enable optimized fill operations
#define TFT_ENABLE_BOUNDS_CHECK     1  // Enable pixel bounds checking

// Debug Configuration
#define TFT_ENABLE_ERROR_CHECKING   1  // Enable error return codes

#endif /* HAL_TFT_TFT_CFG_H_ */
