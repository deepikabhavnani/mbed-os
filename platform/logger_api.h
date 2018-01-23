
/** \addtogroup platform */
/** @{*/
/**
 * \defgroup platform_log_api Logging API functions
 * @{
 */
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
 
#ifndef MBED_LOGGER_API_H
#define MBED_LOGGER_API_H

#include <stdint.h>
#include "platform/logger.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MBED_ID_BASED_TRACING               1

/** Information level log
 *
 * @param  mod  Module name
 * @param  format  printf-style format string, followed by variables
 **/
#if MBED_CONFIG_MAX_LOG_LEVEL >= LOG_LEVEL_INFO
#if MBED_ID_BASED_TRACING
#define info(mod, fmt, ...)   MBED_LOG_ID_1(1, __COUNTER__, mod LOG_INFO FILE_INFO FMT_SEP fmt, ##__VA_ARGS__)
#else
#define info(mod, fmt, ...)   MBED_LOG_STR_1(1, "%-4.4s" LOG_INFO FILE_INFO FMT_SEP fmt, mod, ##__VA_ARGS__)
#endif
#else
#define info(mod, ...)
#endif

/** Information level log
 *
 * @param  mod  Module name
 * @param  condition output only if condition is true (!= 0)
 * @param  format  printf-style format string, followed by variables
 **/
#if MBED_CONFIG_MAX_LOG_LEVEL >= LOG_LEVEL_INFO
#if MBED_ID_BASED_TRACING
#define info_if(mod, condition, fmt, ...)   MBED_LOG_ID_1(condition, __COUNTER__, mod LOG_INFO FILE_INFO FMT_SEP fmt, ##__VA_ARGS__)
#else
#define info_if(mod, condition, fmt, ...)   MBED_LOG_STR_1(condition, "%-4.4s" LOG_INFO FILE_INFO FMT_SEP fmt, mod, ##__VA_ARGS__)
#endif
#else
#define info_if(mod, condition, ...)
#endif

    
#ifdef __cplusplus
}
#endif

#endif

/**@}*/
/** @}*/
