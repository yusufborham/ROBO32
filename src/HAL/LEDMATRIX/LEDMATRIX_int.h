/*
 * LEDMATRIX_int.h
 *
 *  Created on: Aug 21, 2025
 *      Author: drago
 *      Version: 0.2
 */

#ifndef SRC_HAL_LEDMATRIX_INT_H_
#define SRC_HAL_LEDMATRIX_INT_H_


#include "../../LIB/STD_TYPES.h"

void LEDMATRIX_vInit(void);

void LEDMATRIX_vDisplayColumn(u8* frame);

void LEDMATRIX_vDisplayFrame(u8* frame, u16 delay_ms);

#endif /* SRC_HAL_LEDMATRIX_INT_H_ */
