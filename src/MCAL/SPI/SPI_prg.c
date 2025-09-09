/*
 * SPI_prg.c
 *  Created on: Sep 1, 2025
 *  Author: drago
 */

#include "SPI_int.h"
#include "SPI_cfg.h"
#include "../../MCAL/GPIO/GPIO_int.h"


// Initialize SPI peripheral
void MSPI_vInit(volatile SPIx_MemMap_t* SPIx)
{
    // Software slave management
    SET_BIT(SPIx->CR1, SSM);
    SET_BIT(SPIx->CR1, SSI);

    // 8-bit frame
    CLR_BIT(SPIx->CR1, DFF);

    // MSB first
    CLR_BIT(SPIx->CR1, LSBFIRST);

    // Baudrate
    SPIx->CR1 &= ~(0b111 << BR0);
    SPIx->CR1 |= (SPI1_BAUDRATE << BR0);

    // Master mode
    SET_BIT(SPIx->CR1, MSTR);

    // SPI mode 0
    CLR_BIT(SPIx->CR1, SPI_CR1_CPOL);
    CLR_BIT(SPIx->CR1, SPI_CR1_CPHA);

    // Enable SPI
    SET_BIT(SPIx->CR1, SPE);
}

// Transceive single byte with timeout
u8 MSPI_u8Transceive(volatile SPIx_MemMap_t* SPIx, u8 data)
{
    u32 timeout = 100000;

    while(GET_BIT(SPIx->SR, TXE) == 0 && timeout--) ;
    if(timeout == 0) return 0xFF; // error

    SPIx->DR = data;

    timeout = 100000;
    while(GET_BIT(SPIx->SR, RXNE) == 0 && timeout--) ;
    if(timeout == 0) return 0xFF; // error

    return SPIx->DR;
}

// Transmit buffer of bytes
void MSPI_vTransmitBuffer(volatile SPIx_MemMap_t* SPIx, const u8* buffer, u32 length)
{
    for(u32 i=0; i<length; i++)
    {
        MSPI_u8Transceive(SPIx, buffer[i]);
    }
}
