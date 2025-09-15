/*
 * Battery_level.c
 *
 *  Created on: Sep 14, 2025
 *      Author: Koshok
 */

 //volage step = Vref/4096 = 3.3/4096 = 0.0008056640625 V = 0.8056640625 mV
 //battery level = ADC_value * voltage step * (R1 + R2) / R2
 //level from 0 to 100% = battery level / max battery voltage * 100% is 12.6V for 11.1V Li-ion battery
 //with R1 = 30K and 7.5k which makes (12.6 to 11.1V) to (2.52V to 2.22V) on ADC pin
 //then battery level adc reading is from 3124 to 2756 (100 to 0%  respectively)

#include "../APP/Includes_int.h"
#include "Battery_level.h"

void Battery_Level_Init(void) {
    MRCC_vEnableClk(RCC_AHB1, RCC_GPIOA);
    MRCC_vEnableClk(RCC_APB2, RCC_ADC1);

    // Configure PA0 as analog input for ADC
    GPIOx_PinConfig_t adcPin = {
        .port = GPIO_PORTA,
        .pin = PIN0,
        .mode = GPIO_MODE_ANALOG,
        .speed = GPIO_LOW_SPEED,
        .outputType = GPIO_PUSHPULL,
        .pull = GPIO_NOPULL,
        .altFunc = 0
    };
    MGPIO_vPinInit(&adcPin);

    // ADC initialization
    ADC_Config_t adcConfig = {
        .resolution = ADC_RESOLUTION_12BIT,
        .conversion_mode = ADC_CONVERSION_SINGLE,
        .trigger_source = ADC_TRIGGER_SOURCE_SW,
        .data_align = 0 // Right alignment
    };
    HADC_vInit(&adcConfig);

    ADC_ChannelConfig_t adcChannel = {
        .channel = 0,        // ADC channel 0 (PA0)
        .sample_time = 3,    // Sample time
        .rank = 1
    };
    HADC_vConfigureChannel(&adcChannel);
}

u8 Battery_Level_Read(void) {
    ADC_ChannelConfig_t adcChannel = {
        .channel = 0,        // ADC channel 0 (PA0)
        .sample_time = 3,    // Sample time
        .rank = 1
    };
    u16 adcValue = HADC_u16GetConversion(&adcChannel);

    // Clamp adcValue to valid range
    if (adcValue > 3124) adcValue = 3124;
    if (adcValue < 2756) adcValue = 2756;

    // Map 3124 (100%) to 2756 (0%)
    u32 percent = ((u32)(adcValue - 2756) * 100) / (3124 - 2756);

    return (u8)percent;
}


