/*
 * SPI_prv.h
 *
 *  Created on: Sep 1, 2025
 *      Author: drago
 */
#ifndef MCAL_SPI_SPI_PRV_H_
#define MCAL_SPI_SPI_PRV_H_

#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

// SPI base addresses
#define SPI1_BASE_ADDR 0x40013000U
#define SPI2_BASE_ADDR 0x40003800U
#define SPI3_BASE_ADDR 0x40003C00U
#define SPI4_BASE_ADDR 0x40013400U

typedef struct
{
    u32 CR1;
    u32 CR2;
    u32 SR;
    u32 DR;
    u32 CRCPR;
    u32 RXCRCR;
    u32 TXCRCR;
    u32 I2SCFGR;
    u32 I2SPR;
} SPIx_MemMap_t;

#define SPI1 ((volatile SPIx_MemMap_t*) SPI1_BASE_ADDR)
#define SPI2 ((volatile SPIx_MemMap_t*) SPI2_BASE_ADDR)
#define SPI3 ((volatile SPIx_MemMap_t*) SPI3_BASE_ADDR)
#define SPI4 ((volatile SPIx_MemMap_t*) SPI4_BASE_ADDR)

/* CR1 Bits */
#define SPI_CR1_CPHA    0
#define SPI_CR1_CPOL    1
#define MSTR            2
#define BR0             3
#define BR1             4
#define BR2             5
#define SPE             6
#define LSBFIRST        7
#define SSI             8
#define SSM             9
#define RXONLY          10
#define DFF             11
#define CRCNEXT         12
#define CRCEN           13
#define BIDIOE          14
#define BIDIMODE        15

/* CR2 Bits */
#define RXDMAEN 0
#define TXDMAEN 1
#define SSOE    2
#define FRF     4
#define ERRIE   5
#define RXNEIE  6
#define TXEIE   7

/* SR Bits */
#define RXNE 0
#define TXE  1
#define CHSIDE 2
#define UDR  3
#define CRCERR 4
#define MODF 5
#define OVR  6
#define BSY  7
#define FRE  8

#endif /* MCAL_SPI_SPI_PRV_H_ */
