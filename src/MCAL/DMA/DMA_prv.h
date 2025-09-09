/*
 * DMA_prv.h
 *
 *  Created on: Sep 4, 2025
 *      Author: drago
 */

#ifndef MCAL_DMA_DMA_PRV_H_
#define MCAL_DMA_DMA_PRV_H_

#define DMA1_BASE_ADDR 0x40026000u
#define DMA2_BASE_ADDR 0x40026400u

typedef struct {
    u32 LISR;
    u32 HISR;
    u32 LIFCR;
    u32 HIFCR;

    struct {
        u32 CR;
        u32 NDTR;
        u32 PAR;
        u32 M0AR;
        u32 M1AR;
        u32 FCR;
    } STREAM[8];
} DMA_MemMap_t;

#define DMA1 ((volatile DMA_MemMap_t *)DMA1_BASE_ADDR)
#define DMA2 ((volatile DMA_MemMap_t *)DMA2_BASE_ADDR)

#endif /* MCAL_DMA_DMA_PRV_H_ */
