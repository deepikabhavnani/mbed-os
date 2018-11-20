/* mbed Microcontroller Library - CMSIS
 * Copyright (C) 2009-2017 ARM Limited. All rights reserved.
 *
 * A generic CMSIS include header, pulling in LPC54608 specifics
 */

#ifndef MBED_CMSIS_H
#define MBED_CMSIS_H

#include "fsl_device_registers.h"
#include "cmsis_nvic.h"
#include "partition_ARMCM33.h"

/* TZ_START_NS: Start address of non-secure application */
#ifndef TZ_START_NS
#define TZ_START_NS (SECURE_ROM_SIZE)
#endif

#endif
