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
#include "gpio_api.h"
#include "clock_config.h"
#include "fsl_power.h"
#if defined (__ARM_FEATURE_CMSE) &&  (__ARM_FEATURE_CMSE == 3U)
#include "partition_ARMCM33.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

// called before main
void mbed_sdk_init()
{
    BOARD_BootClockFROHF96M();
}

void SystemInitHook(void)
{
/* The TrustZone should be configured as early as possible after RESET.
 * Therefore it is called from SystemInit() during startup. The SystemInitHook() weak function
 * overloading is used for this purpose.
*/
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
    TZ_SAU_Setup();
#endif

}

// Enable the RTC oscillator if available on the board
void rtc_setup_oscillator(void)
{

}

uint32_t us_ticker_get_clock()
{
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
    /* Use 12 MHz clock us ticker timer */
    CLOCK_AttachClk(kFRO_HF_to_CTIMER0);
    return CLOCK_GetFreq(kCLOCK_CTmier0);;
#else
    /* Use 12 MHz clock us ticker timer */
    CLOCK_AttachClk(kFRO_HF_to_CTIMER1);
    return CLOCK_GetFreq(kCLOCK_CTmier1);;
#endif
}

