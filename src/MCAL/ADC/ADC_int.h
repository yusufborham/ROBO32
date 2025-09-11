/*
 * ADC_int.h
 *
 *  Created on: Sep 9, 2025
 *      Author: Koshok
 */

#ifndef MCAL_ADC_ADC_INT_H_
#define MCAL_ADC_ADC_INT_H_

#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"
#include "ADC_prv.h"



typedef struct {
    u8 resolution;      // ADC resolution: 8 10 or 12 bits
    u8 conversion_mode; // Single or continuous conversion
    u8 trigger_source;  // Trigger source for conversion
    u8 voltage_ref;    // Voltage reference selection
    u8 data_align;     // Data alignment: left or right
} ADC_Config_t;

typedef struct {
    u8 channel;        // ADC channel number (0-15)
    u8 sample_time;    // Sampling time
    u8 rank;           // Rank in the regular group (1-16)
} ADC_ChannelConfig_t;


void HADC_vInit(ADC_Config_t* config);
void HADC_vConfigureChannel(ADC_ChannelConfig_t* channelConfig);
u16 HADC_u16GetConversion(ADC_ChannelConfig_t* channelConfig);




#endif /* MCAL_ADC_ADC_INT_H_ */
