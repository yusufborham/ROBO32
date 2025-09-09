/*
 * S2P_cfg.h
 *
 *  Created on: Aug 28, 2025
 *      Author: Koshok
 */

#ifndef HAL_S2P_S2P_CFG_H_
#define HAL_S2P_S2P_CFG_H_


#define S2P_NO_OF_SHIFT_GEG  2

/* Default Pins Configuration */
#define S2P_DEFAULT_DATA_PORT       GPIO_PORTA
#define S2P_DEFAULT_DATA_PIN        PIN0

#define S2P_DEFAULT_SHIFTCLK_PORT   GPIO_PORTA
#define S2P_DEFAULT_SHIFTCLK_PIN    PIN1

#define S2P_DEFAULT_LATCHCLK_PORT   GPIO_PORTA
#define S2P_DEFAULT_LATCHCLK_PIN    PIN2

/* Optional: Delay timing (ms) */
#define S2P_CLOCK_PULSE_DELAY_MS    1



#endif /* HAL_S2P_S2P_CFG_H_ */
