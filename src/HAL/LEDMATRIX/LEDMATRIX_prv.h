/*
 * LEDMATRIX_prv.h
 *
 *  Created on: Aug 28, 2025
 *      Author: drago
 *      Version: 0.2
 */

#ifndef SRC_HAL_LEDMATRIX_LEDMATRIX_PRV_H_
#define SRC_HAL_LEDMATRIX_LEDMATRIX_PRV_H_

#include "../../LIB/STD_TYPES.h"
#include "../../HAL/S2P/S2P_int.h"

#define DIRECT_LEDMATRIX	1
#define S2P_CONNECTION		2





#define LEDMATRIX_NUM_CHIPS 2
#define LEDMATRIX_NUM_COLS (8*LEDMATRIX_NUM_CHIPS)


static const u8 LEDMATRIX_RowPins[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};



#endif /* SRC_HAL_LEDMATRIX_LEDMATRIX_PRV_H_ */
