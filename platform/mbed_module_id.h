
/** \addtogroup platform */
/** @{*/
/**
 * \defgroup platform_ID Module ID's
 * @{
 */

/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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
 
#ifndef MBED_MODULE_ID_H
#define MBED_MODULE_ID_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum module_id {
    MOD_MBED_OS = 0,

    // All HAL Modules
    MOD_HAL_GEN = 1,
    
    // All Platfrom Modules
    MOD_PLATFORM_GEN = 101,
    
    // All RTOS Modules
    MOD_RTOS_GEN = 201,
    
    // All Driver Modules
    MOD_DRIVER_GEN = 301,
    
    
    // Special Modules,
    MOD_SPECIAL_GEN = 1001,
    
    
    // All features
    MOD_FEATURE_GEN = 1101,
    MOD_FEATURE_FAT = 1102,
    MOD_FEATURE_CHANFS = 1103,


   // Others
   MOD_OTHER_GEN   = 2001,
   
   
   MOD_ID_END
    
}module_id_t;

#ifdef __cplusplus
}
#endif

#endif  // MBED_LOGGER_H
/**@}*/

/**@}*/
