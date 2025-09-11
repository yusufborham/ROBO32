/*
 * ADC_prg.c
 *
 *  Created on: Sep 9, 2025
 *      Author: Koshok
 */

#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h" 
#include "../../MCAL/RCC/RCC_int.h"
#include "ADC_int.h"
#include "ADC_prv.h"
#include "ADC_cfg.h"
#include "../RCC/RCC_int.h"
#include "../RCC/RCC_prv.h"


static void ADC_SetSampleTime(u8 channel, u8 sample_time);
static void ADC_SetSequence(u8 rank, u8 channel);

void HADC_vInit(ADC_Config_t* config) 
{
    // Enable ADC clock (Assuming RCC is already configured)
    MRCC_vInit();
    MRCC_vEnableClk(RCC_APB2, ADC1EN); // Enable ADC1 clock

    // set resolution
    ADC1->ADC_CR1 &= ~(0b11U << 24); // Clear resolution bits
    ADC1->ADC_CR1 |= (config->resolution << 24);

    // SET conversion mode
    if (config->conversion_mode == ADC_CONVERSION_CONTINUOUS) {
        SET_BIT(ADC1->ADC_CR2, 1); // Set CONT bit for continuous conversion
    } else {
        CLR_BIT(ADC1->ADC_CR2, 1); // Clear CONT bit for single conversion
    }

    // Set trigger source
    ADC1->ADC_CR2 &= ~(0b1111U << 24); // Clear EXTSEL bits
    ADC1->ADC_CR2 |= (config->trigger_source << 24);

    // Set data alignment
    if (config->data_align == 0) { // Right alignment
        CLR_BIT(ADC1->ADC_CR2, 11); // Clear ALIGN bit
    } else { // Left alignment
        SET_BIT(ADC1->ADC_CR2, 11); // Set ALIGN bit
    }

 
    // Enable ADC
    SET_BIT(ADC1->ADC_CR2, 0); // Set ADON bit
 
}

static void ADC_SetSampleTime(u8 channel, u8 sample_time) {
    if (channel <= 9) {
        ADC1->ADC_SMPR2 &= ~(0b111U << (channel * 3));
        ADC1->ADC_SMPR2 |= (sample_time << (channel * 3));
    } else if (channel <= 15) {
        ADC1->ADC_SMPR1 &= ~(0b111U << ((channel - 10) * 3));
        ADC1->ADC_SMPR1 |= (sample_time << ((channel - 10) * 3));
    }
}

static void ADC_SetSequence(u8 rank, u8 channel) {
    if (rank >= 1 && rank <= 6) {
        ADC1->ADC_SQR3 &= ~(0b11111U << ((rank - 1) * 5));
        ADC1->ADC_SQR3 |= (channel << ((rank - 1) * 5));
    } else if (rank >= 7 && rank <= 12) {
        ADC1->ADC_SQR2 &= ~(0b11111U << ((rank - 7) * 5));
        ADC1->ADC_SQR2 |= (channel << ((rank - 7) * 5));
    } else if (rank >= 13 && rank <= 16) {
        ADC1->ADC_SQR1 &= ~(0b11111U << ((rank - 13) * 5));
        ADC1->ADC_SQR1 |= (channel << ((rank - 13) * 5));
    }
}

void HADC_vConfigureChannel(ADC_ChannelConfig_t* channelConfig) 
{
    ADC_SetSampleTime(channelConfig->channel, channelConfig->sample_time);
    ADC_SetSequence(channelConfig->rank, channelConfig->channel);

    // Set sequence length (L bits in SQR1)
    ADC1->ADC_SQR1 &= ~(0xF << 20);
    ADC1->ADC_SQR1 |= ((channelConfig->rank - 1) << 20); // L = N conversions - 1
}

u16 HADC_u16GetConversion(ADC_ChannelConfig_t* channelConfig) 
{
    u8 channel = channelConfig->channel;
    
    //select the channel by configuring its sample time and rank
    ADC_SetSampleTime(channel, channelConfig->sample_time);
    ADC_SetSequence(channelConfig->rank, channel);

    // Start conversion
    SET_BIT(ADC1->ADC_CR2, 30); // Set SWSTART bit to start conversion

    // Wait for conversion to complete
    while (GET_BIT(ADC1->ADC_SR, 1) == 0); // Wait until EOC (End of Conversion) flag is set

    // Read conversion result
    u16 result = (u16)(ADC1->ADC_DR & 0xFFFF); // Read the data register

    // Clear EOC flag by reading ADC_SR (it is cleared automatically when ADC_DR is read)
    volatile u32 temp = ADC1->ADC_SR; // Dummy read to clear EOC flag

    return result;
}



