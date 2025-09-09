/*
 * DAC_prg.c
 *
 *  Created on: Aug 27, 2025
 *      Author: Koshok
 */

#include "DAC_int.h"
#include "../../MCAL/GPIO/GPIO_int.h"
#include "../../MCAL/SYSTICK/SYSTICK_int.h"

/*=====================================================
 * DAC Initialization
 *=====================================================*/
void HDAC_vInit(DAC_Config_t* DAC_cfg) {
    for (int i = 0; i < 8; i++) {
        GPIOx_PinConfig_t DAC_Pins = {
            .port       = DAC_cfg->Port,
            .pin        = DAC_cfg->pin[i],
            .mode       = GPIO_MODE_OUTPUT,
            .outputType = GPIO_PUSHPULL,
            .speed      = GPIO_LOW_SPEED,
            .pull       = GPIO_NOPULL,
            .altFunc    = 0
        };
        MGPIO_vPinInit(&DAC_Pins);
    }

    MSYSTICK_Config_t stk_cfg = {
        .InterruptEnable = INT_ENABLE,
        .CLK_SRC        = CLK_SRC_AHB_8
    };
    MSYSTICK_vInit(&stk_cfg);
}

/*=====================================================
 * DAC Send one sample (8-bit value on 8 GPIO pins)
 *=====================================================*/
void HDAC_vSend(DAC_Config_t* DAC_cfg, u8* A_u8ptr, u32 index) {
    for (u8 i = 0; i < 8; i++) {
        u8 bitVal = GET_BIT(A_u8ptr[index], i);
        MGPIO_vSetPinValue(DAC_cfg->Port, DAC_cfg->pin[i], bitVal);
    }
}



