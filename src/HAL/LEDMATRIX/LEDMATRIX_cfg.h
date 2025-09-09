/*
 * LEDMATRIX_cfg.h
 *
 *  Created on: Aug 21, 2025
 *      Author: drago
 *      Version: 0.2
 */

#ifndef SRC_HAL_LEDMATRIX_CFG_H_
#define SRC_HAL_LEDMATRIX_CFG_H_



#define SCAN_TIME  2.5  // in ms

#include "../../HAL/S2P/S2P_int.h"

extern const S2P_Config_t LEDMATRIX_S2P;


/* Options:
 * DIRECT_LEDMATRIX
 * S2P_CONNECTION
 */
#define METHOD  S2P_CONNECTION


#define NO_ROWS  8
#define NO_COLS  8

#endif /* SRC_HAL_LEDMATRIX_CFG_H_ */

