/**
 ******************************************************************************
 * @file    pinmap_hal.c
 * @authors Satish Nair, Brett Walach, Matthew McGowan
 * @version V1.0.0
 * @date    12-Sept-2014
 * @brief
 ******************************************************************************
  Copyright (c) 2013-2015 Particle Industries, Inc.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "pinmap_hal.h"
#include "pinmap_impl.h"
#include <stddef.h>

#define NONE CHANNEL_NONE

/* Private typedef -----------------------------------------------------------*/

STM32_Pin_Info __PIN_MAP[TOTAL_PINS] =
{
/*
 * gpio_peripheral (GPIOA, GPIOB, GPIOC or GPIOD)
 * gpio_pin (0-15)
 * gpio_pin_source (GPIO_PinSource0 - GPIO_PinSource15)
 * adc_channel (ADC_Channel_xx or NONE. Note we don't define the peripheral because our chip only has one)
 * dac_channel (DAC_Channel_xx or NONE)
 * timer_peripheral (TIM1 - TIM5, or NONE)
 * timer_ch (1-3, or NONE)
 * pin_mode (NONE by default, can be set to OUTPUT, INPUT, or other types)
 * timer_ccr (0 by default, store the CCR value for TIM interrupt use)
 * user_property (0 by default, user variable storage)
 */

/* COMMON TO PHOTON, P1 and ELECTRON */
/* D0            - 00 */ { GPIOB, GPIO_Pin_7, GPIO_PinSource7, NONE, NONE, TIM4, TIM_Channel_2, PIN_MODE_NONE, 0, 0 },
/* D1            - 01 */ { GPIOB, GPIO_Pin_6, GPIO_PinSource6, NONE, NONE, TIM4, TIM_Channel_1, PIN_MODE_NONE, 0, 0 },
/* D2            - 02 */ { GPIOB, GPIO_Pin_5, GPIO_PinSource5, NONE, NONE, TIM3, TIM_Channel_2, PIN_MODE_NONE, 0, 0 },
/* D3            - 03 */ { GPIOB, GPIO_Pin_4, GPIO_PinSource4, NONE, NONE, TIM3, TIM_Channel_1, PIN_MODE_NONE, 0, 0 },
/* D4            - 04 */ { GPIOB, GPIO_Pin_3, GPIO_PinSource3, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* D5            - 05 */ { GPIOA, GPIO_Pin_15, GPIO_PinSource15, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* D6            - 06 */ { GPIOA, GPIO_Pin_14, GPIO_PinSource14, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* D7            - 07 */ { GPIOA, GPIO_Pin_13, GPIO_PinSource13, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* NOT USED      - 08 */ { NULL, NONE, NONE, NONE, NONE, NULL, NONE, NONE, NONE, NONE },
/* NOT USED      - 09 */ { NULL, NONE, NONE, NONE, NONE, NULL, NONE, NONE, NONE, NONE },
/* A0            - 10 */ { GPIOC, GPIO_Pin_5, GPIO_PinSource5, ADC_Channel_15, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* A1            - 11 */ { GPIOC, GPIO_Pin_3, GPIO_PinSource3, ADC_Channel_13, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* A2            - 12 */ { GPIOC, GPIO_Pin_2, GPIO_PinSource2, ADC_Channel_12, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* DAC2, A3      - 13 */ { GPIOA, GPIO_Pin_5, GPIO_PinSource5, ADC_Channel_5, DAC_Channel_2, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* A4            - 14 */ { GPIOA, GPIO_Pin_6, GPIO_PinSource6, ADC_Channel_6, NONE, TIM3, TIM_Channel_1, PIN_MODE_NONE, 0, 0 },
/* A5            - 15 */ { GPIOA, GPIO_Pin_7, GPIO_PinSource7, ADC_Channel_7, NONE, TIM3, TIM_Channel_2, PIN_MODE_NONE, 0, 0 },
/* DAC, DAC1, A6 - 16 */ { GPIOA, GPIO_Pin_4, GPIO_PinSource4, ADC_Channel_4, DAC_Channel_1, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* WKP, A7       - 17 */ { GPIOA, GPIO_Pin_0, GPIO_PinSource0, ADC_Channel_0, NONE, TIM5, TIM_Channel_1, PIN_MODE_NONE, 0, 0 },
/* RX            - 18 */ { GPIOA, GPIO_Pin_10, GPIO_PinSource10, NONE, NONE, TIM1, TIM_Channel_3, PIN_MODE_NONE, 0, 0 },
/* TX            - 19 */ { GPIOA, GPIO_Pin_9, GPIO_PinSource9, NONE, NONE, TIM1, TIM_Channel_2, PIN_MODE_NONE, 0, 0 },
/* SETUP BUTTON  - 20 */ { GPIOC, GPIO_Pin_7, GPIO_PinSource7, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* RGBR          - 21 */ { GPIOA, GPIO_Pin_1, GPIO_PinSource1, NONE, NONE, TIM2, TIM_Channel_2, PIN_MODE_NONE, 0, 0 },
/* RGBG          - 22 */ { GPIOA, GPIO_Pin_2, GPIO_PinSource2, NONE, NONE, TIM2, TIM_Channel_3, PIN_MODE_NONE, 0, 0 },
/* RGBB          - 23 */ { GPIOA, GPIO_Pin_3, GPIO_PinSource3, NONE, NONE, TIM2, TIM_Channel_4, PIN_MODE_NONE, 0, 0 }
#if PLATFORM_ID == 8 // P1
/* P1S0          - 24 */,{ GPIOB, GPIO_Pin_0, GPIO_PinSource0, ADC_Channel_8, NONE, TIM3, TIM_Channel_3, PIN_MODE_NONE, 0, 0 },
/* P1S1          - 25 */ { GPIOB, GPIO_Pin_1, GPIO_PinSource1, ADC_Channel_9, NONE, TIM3, TIM_Channel_4, PIN_MODE_NONE, 0, 0 },
/* P1S2          - 26 */ { GPIOC, GPIO_Pin_0, GPIO_PinSource0, ADC_Channel_10, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* P1S3          - 27 */ { GPIOC, GPIO_Pin_4, GPIO_PinSource4, ADC_Channel_14, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* P1S4          - 28 */ { GPIOC, GPIO_Pin_13, GPIO_PinSource13, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* P1S5          - 29 */ { GPIOC, GPIO_Pin_1, GPIO_PinSource1, ADC_Channel_11, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 }
#endif
#if PLATFORM_ID == 10 // Electron (((NOTE: Update TOTAL_ANALOG_PINS after adding all ADCs later)))
/* B0            - 24 */,{ GPIOC, GPIO_Pin_8, GPIO_PinSource8, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* B1            - 25 */ { GPIOC, GPIO_Pin_6, GPIO_PinSource6, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* B2            - 26 */ { GPIOB, GPIO_Pin_0, GPIO_PinSource0, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* B3            - 27 */ { GPIOB, GPIO_Pin_1, GPIO_PinSource1, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* B4            - 28 */ { GPIOC, GPIO_Pin_0, GPIO_PinSource0, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* B5            - 29 */ { GPIOC, GPIO_Pin_1, GPIO_PinSource1, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* C0            - 30 */ { GPIOD, GPIO_Pin_2, GPIO_PinSource2, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* C1            - 31 */ { GPIOC, GPIO_Pin_12, GPIO_PinSource12, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* C2            - 32 */ { GPIOC, GPIO_Pin_11, GPIO_PinSource11, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* C3            - 33 */ { GPIOC, GPIO_Pin_10, GPIO_PinSource10, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* C4            - 34 */ { GPIOB, GPIO_Pin_9, GPIO_PinSource9, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* C5            - 35 */ { GPIOB, GPIO_Pin_8, GPIO_PinSource8, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* TXD_UC        - 36 */ { GPIOB, GPIO_Pin_10, GPIO_PinSource10, NONE, NONE, TIM2, TIM_Channel_3, PIN_MODE_NONE, 0, 0 },
/* RXD_UC        - 37 */ { GPIOB, GPIO_Pin_11, GPIO_PinSource11, NONE, NONE, TIM2, TIM_Channel_4, PIN_MODE_NONE, 0, 0 },
/* RI_UC         - 38 */ { GPIOB, GPIO_Pin_12, GPIO_PinSource12, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* CTS_UC        - 39 */ { GPIOB, GPIO_Pin_13, GPIO_PinSource13, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* RTS_UC        - 40 */ { GPIOB, GPIO_Pin_14, GPIO_PinSource14, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* PWR_UC        - 41 */ { GPIOB, GPIO_Pin_2, GPIO_PinSource2, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* RESET_UC      - 42 */ { GPIOC, GPIO_Pin_4, GPIO_PinSource4, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* LVLOE_UC      - 43 */ { GPIOB, GPIO_Pin_15, GPIO_PinSource15, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* PM_SDA_UC     - 44 */ { GPIOC, GPIO_Pin_9, GPIO_PinSource9, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* PM_SCL_UC     - 45 */ { GPIOA, GPIO_Pin_8, GPIO_PinSource8, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
/* LOW_BAT_UC    - 46 */ { GPIOC, GPIO_Pin_13, GPIO_PinSource13, NONE, NONE, NULL, NONE, PIN_MODE_NONE, 0, 0 },
#endif
};


STM32_Pin_Info* HAL_Pin_Map() {
    return __PIN_MAP;
}