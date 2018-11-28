/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
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

#include "flash_api.h"
#include "mbed_critical.h"

#if DEVICE_FLASH

#include "fsl_iap.h"

#define SECURE_FLASH_START_ADDR    0x10000000

static flash_config_t flash_config;

int32_t flash_init(flash_t *obj)
{
    status_t result;

    /* Clean up Flash driver Structure*/
    memset(&flash_config, 0, sizeof(flash_config_t));

    /* Setup flash driver structure for device and initialize variables. */
    result = FLASH_Init(&flash_config);
    if (kStatus_FLASH_Success != result) {
        return -1;
    } else {
        return 0;
    }
}

int32_t flash_free(flash_t *obj)
{
    return 0;
}

int32_t flash_erase_sector(flash_t *obj, uint32_t address)
{
    int status;

    /* We need to prevent flash accesses during erase operation */
    core_util_critical_section_enter();
    status = FLASH_Erase(&flash_config, address, flash_config.PFlashSectorSize, kFLASH_ApiEraseKey);

    if (status == kStatus_Success) {
        status = FLASH_VerifyErase(&flash_config, address, flash_config.PFlashSectorSize);
    }
    core_util_critical_section_exit();

    if (status == kStatus_Success) {
        return 0;
    } else {
        return -1;
    }
}

int32_t flash_program_page(flash_t *obj, uint32_t address, const uint8_t *data, uint32_t size)
{
    int status;
    uint32_t failedAddress, failedData;

    /* We need to prevent flash accesses during program operation */
    core_util_critical_section_enter();
    status = FLASH_Program(&flash_config, address, (uint8_t *)data, size);

    if (status == kStatus_Success) {
        status = FLASH_VerifyProgram(&flash_config, address, size, data,  &failedAddress, &failedData);
    }
    core_util_critical_section_exit();

    return status;

}

uint32_t flash_get_sector_size(const flash_t *obj, uint32_t address)
{
    uint32_t sectorsize = MBED_FLASH_INVALID_SIZE;
    uint32_t devicesize = 0;
    uint32_t startaddr = 0;

#if (__ARM_FEATURE_CMSE & 0x2)
    startaddr = SECURE_FLASH_START_ADDR;
#else
    FLASH_GetProperty(&flash_config, kFLASH_PropertyPflashBlockBaseAddr, &startaddr);
#endif

    FLASH_GetProperty(&flash_config, kFLASH_PropertyPflashTotalSize, &devicesize);

    if ((address >= startaddr) && (address < (startaddr + devicesize))) {
        FLASH_GetProperty(&flash_config, kFLASH_PropertyPflashSectorSize, &sectorsize);
    }

    return sectorsize;
}

uint32_t flash_get_page_size(const flash_t *obj)
{
    uint32_t flashPageSize = 0;

    FLASH_GetProperty(&flash_config, kFLASH_PropertyPflashPageSize, &flashPageSize);;

    return flashPageSize;
}

uint32_t flash_get_start_address(const flash_t *obj)
{
    uint32_t startaddr = 0;

#if (__ARM_FEATURE_CMSE & 0x2)
    startaddr += SECURE_FLASH_START_ADDR;
#else
    FLASH_GetProperty(&flash_config, kFLASH_PropertyPflashBlockBaseAddr, &startaddr);
#endif

    return startaddr;
}

uint32_t flash_get_size(const flash_t *obj)
{
    uint32_t devicesize = 0;

    FLASH_GetProperty(&flash_config, kFLASH_PropertyPflashTotalSize, &devicesize);

    return devicesize;
}

uint8_t flash_get_erase_value(const flash_t *obj)
{
    (void)obj;

    return 0xFF;
}

#endif
