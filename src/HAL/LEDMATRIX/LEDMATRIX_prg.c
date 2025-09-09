/*
 * LEDMATRIX_prg.c
 *
 *  Created on: Aug 30, 2025
 *  Author: Mohamed Ismail
 *  Version: 1.1
 */

#include "LEDMATRIX_int.h"
#include "LEDMATRIX_cfg.h"
#include "LEDMATRIX_prv.h"

#include "../../MCAL/SYSTICK/SYSTICK_int.h"
#include "../../HAL/S2P/S2P_int.h"


const S2P_Config_t LEDMATRIX_S2P = {
    .DataPort     = GPIO_PORTA,
    .DataPin      = PIN0,
    .ShiftCLKPort = GPIO_PORTA,
    .ShiftCLKPin  = PIN1,
    .LatchCLKPort = GPIO_PORTA,
    .LatchCLKPin  = PIN2
};


void LEDMATRIX_vInit(void)
{

    S2P_vInit((S2P_Config_t *)&LEDMATRIX_S2P);

    u8 clearFrame[8] = {0};
    LEDMATRIX_vDisplayFrame(clearFrame, 10);
}

static void LEDMATRIX_vSendRowData(u8 rowMask, u8 colData)
{
    u16 combinedData = ((u16)colData << 8) | (~rowMask & 0xFF);

    S2P_SendData((S2P_Config_t *)&LEDMATRIX_S2P, combinedData);
}

void LEDMATRIX_vDisplayColumn(u8 *frame)
{
    for (u8 row = 0; row < 8; row++)
    {
        LEDMATRIX_vSendRowData(LEDMATRIX_RowPins[row], frame[row]);

        MSYSTICK_vSetDelay_ms(1);
    }
}

void LEDMATRIX_vDisplayFrame(u8 *frame, u16 delay_ms)
{
    for (u16 elapsed = 0; elapsed < delay_ms; elapsed++)
    {
        LEDMATRIX_vDisplayColumn(frame);
    }
}
