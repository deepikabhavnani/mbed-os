/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PIN_INPUT,
    PIN_OUTPUT
} PinDirection;

#define PORT_SHIFT  5

typedef enum {
    P0_0 = (0 << PORT_SHIFT | 0),
    P0_1 = (0 << PORT_SHIFT | 1),
    P0_2 = (0 << PORT_SHIFT | 2),
    P0_3 = (0 << PORT_SHIFT | 3),
    P0_4 = (0 << PORT_SHIFT | 4),
    P0_5 = (0 << PORT_SHIFT | 5),
    P0_6 = (0 << PORT_SHIFT | 6),
    P0_7 = (0 << PORT_SHIFT | 7),
    P0_8 = (0 << PORT_SHIFT | 8),
    P0_9 = (0 << PORT_SHIFT | 9),
    P0_10 = (0 << PORT_SHIFT | 10),
    P0_11 = (0 << PORT_SHIFT | 11),
    P0_12 = (0 << PORT_SHIFT | 12),
    P0_13 = (0 << PORT_SHIFT | 13),
    P0_14 = (0 << PORT_SHIFT | 14),
    P0_15 = (0 << PORT_SHIFT | 15),
    P0_16 = (0 << PORT_SHIFT | 16),
    P0_17 = (0 << PORT_SHIFT | 17),
    P0_18 = (0 << PORT_SHIFT | 18),
    P0_19 = (0 << PORT_SHIFT | 19),
    P0_20 = (0 << PORT_SHIFT | 20),
    P0_21 = (0 << PORT_SHIFT | 21),
    P0_22 = (0 << PORT_SHIFT | 22),
    P0_23 = (0 << PORT_SHIFT | 23),
    P0_24 = (0 << PORT_SHIFT | 24),
    P0_25 = (0 << PORT_SHIFT | 25),
    P0_26 = (0 << PORT_SHIFT | 26),
    P0_27 = (0 << PORT_SHIFT | 27),
    P0_28 = (0 << PORT_SHIFT | 28),
    P0_29 = (0 << PORT_SHIFT | 29),
    P0_30 = (0 << PORT_SHIFT | 30),
    P0_31 = (0 << PORT_SHIFT | 31),

    P1_0 = (1 << PORT_SHIFT | 0),
    P1_1 = (1 << PORT_SHIFT | 1),
    P1_2 = (1 << PORT_SHIFT | 2),
    P1_3 = (1 << PORT_SHIFT | 3),
    P1_4 = (1 << PORT_SHIFT | 4),
    P1_5 = (1 << PORT_SHIFT | 5),
    P1_6 = (1 << PORT_SHIFT | 6),
    P1_7 = (1 << PORT_SHIFT | 7),
    P1_8 = (1 << PORT_SHIFT | 8),
    P1_9 = (1 << PORT_SHIFT | 9),
    P1_10 = (1 << PORT_SHIFT | 10),
    P1_11 = (1 << PORT_SHIFT | 11),
    P1_12 = (1 << PORT_SHIFT | 12),
    P1_13 = (1 << PORT_SHIFT | 13),
    P1_14 = (1 << PORT_SHIFT | 14),
    P1_15 = (1 << PORT_SHIFT | 15),
    P1_16 = (1 << PORT_SHIFT | 16),
    P1_17 = (1 << PORT_SHIFT | 17),
    P1_18 = (1 << PORT_SHIFT | 18),
    P1_19 = (1 << PORT_SHIFT | 19),
    P1_20 = (1 << PORT_SHIFT | 20),
    P1_21 = (1 << PORT_SHIFT | 21),
    P1_22 = (1 << PORT_SHIFT | 22),
    P1_23 = (1 << PORT_SHIFT | 23),
    P1_24 = (1 << PORT_SHIFT | 24),
    P1_25 = (1 << PORT_SHIFT | 25),
    P1_26 = (1 << PORT_SHIFT | 26),
    P1_27 = (1 << PORT_SHIFT | 27),
    P1_28 = (1 << PORT_SHIFT | 28),
    P1_29 = (1 << PORT_SHIFT | 29),
    P1_30 = (1 << PORT_SHIFT | 30),
    P1_31 = (1 << PORT_SHIFT | 31),

    LED_RED   = P1_4,

    // mbed original LED naming
    LED1 = P1_6,
    LED2 = P1_7,
    LED3 = LED_RED,
    LED4 = LED_RED,

    //Push buttons
    SW2 = P1_18,
    SW3 = P1_9,

    // USB Pins
    USBTX = P0_30,
    USBRX = P0_29,

    // Arduino Headers
    D0 = P1_24,
    D1 = P0_27,
    D2 = P0_15,
    D3 = P1_6,
    D4 = P1_7,
    D5 = P1_4,
    D6 = P1_10,
    D7 = P1_9,
    D8 = P1_8,
    D9 = P1_5,
    D10 = P1_1,
    D11 = P0_26,
    D12 = P1_3,
    D13 = P1_2,
    D14 = P1_21,
    D15 = P1_20,

    I2C_SCL = D15,
    I2C_SDA = D14,

    A0 = P0_16,
    A1 = P0_23,
    A2 = P0_0,
    A3 = P1_31,
    A4 = P0_14,
    A5 = P0_13,

    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;


typedef enum {
    PullNone = 0,
    PullDown = 1,
    PullUp   = 2,
    PullDefault = PullUp
} PinMode;

#ifdef __cplusplus
}
#endif

#endif
