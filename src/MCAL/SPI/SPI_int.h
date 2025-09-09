/*
 * SPI_int.h
 *
 *  Created on: Sep 1, 2025
 *      Author: drago
 */

#ifndef MCAL_SPI_SPI_INT_H_
#define MCAL_SPI_SPI_INT_H_

#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"
#include "SPI_prv.h"

// Init SPI peripheral (generic)
void MSPI_vInit(volatile SPIx_MemMap_t* SPIx);

// Transceive single byte
u8 MSPI_u8Transceive(volatile SPIx_MemMap_t* SPIx, u8 data);

// Transmit multiple bytes (buffer)
void MSPI_vTransmitBuffer(volatile SPIx_MemMap_t* SPIx, const u8* buffer, u32 length);

#endif /* MCAL_SPI_SPI_INT_H_ */
