/*
 * IR_prg.c
 *
 *  Created on: Aug 30, 2025
 *      Author: drago
 */
#include "IR_cfg.h"
#include "IR_prv.h"
#include "IR_int.h"

#include "../../MCAL/RCC/RCC_int.h"
#include "../../MCAL/GPIO/GPIO_int.h"
#include "../../MCAL/EXTI/EXTI_int.h"
#include "../../MCAL/NVIC/NVIC_int.h"
#include "../../MCAL/SYSTICK/SYSTICK_int.h"
#include "../../LIB/BIT_MATH.h"

IR_State_t g_irState = {0};
static IR_Callback_t s_callback = 0;

/* ===== Public API ===== */
void IR_Init(IR_Callback_t cb)
{
    /* Enable clocks */
    IR_ENABLE_GPIO_CLK();
    IR_ENABLE_SYSCFG_CLK();

    /* Configure IR input pin */
    GPIOx_PinConfig_t pinCfg = {
        .port = IR_GPIO_PORT,
        .pin  = IR_GPIO_PIN,
        .mode = GPIO_MODE_INPUT,
        .pull = GPIO_NOPULL
    };
    MGPIO_vPinInit(&pinCfg);

    /* Configure EXTI */
    EXTI_Cng_t extiCfg = {0,0};
    MEXTI_vInit(&extiCfg);
    MEXTI_vEnableINT(IR_EXTI_LINE, EXTI_FALLING_EDGE);
    MEXTI_vSetCallBack(IR_EXTI_Handler, IR_EXTI_LINE);

    /* Enable NVIC */
    MNVIC_vEnableInterrupt(IR_NVIC_IRQ_NUM);

    /* Save callback */
    s_callback = cb;

    /* Reset state */
    g_irState.startFlag = 0;
    g_irState.edgeCount = 0;
    g_irState.lastCode  = 0;
}

u8 IR_GetLastCode(u8* outCode)
{
    if (!outCode) return 0;
    *outCode = g_irState.lastCode;
    return 1;
}

void IR_ClearLastCode(void)
{
    g_irState.lastCode = 0;
}

/* ===== Internals ===== */
static void IR_EXTI_Handler(void)
{
    if (g_irState.startFlag == 0)
    {
        g_irState.startFlag = 1;
        g_irState.edgeCount = 0;
        MSYSTICK_vSetInterval_Single(15, IR_DecodeFrame);
    }
    else
    {
        u32 elapsed = MSYSTICK_u32GetElapsedTime_SingleShot();
        u32 us = IR_TICK_TO_US(elapsed);

        if (g_irState.edgeCount < IR_BUFFER_LEN)
        {
            g_irState.edgesUs[g_irState.edgeCount++] = us;
        }

        MSYSTICK_vSetInterval_Single(4, IR_DecodeFrame);
    }
}

static void IR_DecodeFrame(void)
{
    u8 value = 0;

    for (u8 i=0; i<IR_DATA_BITS; i++)
    {
        u32 t = g_irState.edgesUs[17+i];
        if (t >= IR_ZERO_MIN_US && t < IR_ZERO_MAX_US)
        {
            CLR_BIT(value, i);
        }
        else if (t >= IR_ONE_MIN_US && t < IR_ONE_MAX_US)
        {
            SET_BIT(value, i);
        }
    }

    g_irState.lastCode = value;
    g_irState.startFlag = 0;
    g_irState.edgeCount = 0;

    for (u8 i=0; i<IR_BUFFER_LEN; i++) g_irState.edgesUs[i] = 0;

    if (s_callback) s_callback(value);
}
