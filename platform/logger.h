
/** \addtogroup platform */
/** @{*/
/**
 * \defgroup platform_log Logger functions
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

#ifndef MBED_LOGGER_H
#define MBED_LOGGER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdarg.h>
#include "platform/mbed_preprocessor.h"

#ifndef MBED_CONFIG_MAX_LOG_LEVEL
#define MBED_CONFIG_MAX_LOG_LEVEL           5
#endif
#ifndef MBED_CONFIG_LOG_BUF_SIZE
#define MBED_CONFIG_LOG_BUF_SIZE            512
#endif

#define SEPARATOR                 " "
#define FMT_SEP                   ":" SEPARATOR

#if defined(__ARMCC_VERSION)
#define FILE_NAME_               __MODULE__
#define FILE_INFO_               __MODULE__ SEPARATOR MBED_STRINGIFY(__LINE__) SEPARATOR
#else
#define FILE_NAME_               __BASE_FILE__
#define FILE_INFO_               __BASE_FILE__ SEPARATOR MBED_STRINGIFY(__LINE__) SEPARATOR
#endif

typedef enum log_level {
    LOG_LEVEL_ERR_CRITICAL = 0,
    LOG_LEVEL_ERR = 1,
    LOG_LEVEL_WARN = 2,
    LOG_LEVEL_DEBUG = 3,
    LOG_LEVEL_INFO = 4,
    LOG_LEVEL_TRACE = 5,
}log_level_t;

// Log-Level Strings
#define LOG_ERR_CRITICAL_        "C" SEPARATOR
#define LOG_ERR_                 "E" SEPARATOR
#define LOG_WARN_                "W" SEPARATOR
#define LOG_DEBUG_               "D" SEPARATOR
#define LOG_INFO_                "I" SEPARATOR
#define LOG_TRACE_               "T" SEPARATOR

#define SET_MODULE(x)           (x & 0xFF)
#define SET_COUNTER(y)          ((y & 0xFF) << 8)
#define SET_LINE_NUM(z)         ((z & 0xFFFF) << 16)
#define TRACE_ID(x,y,z)         (SET_MODULE(x) | SET_COUNTER(y) | SET_LINE_NUM(z))

typedef struct trace_id {
    uint32_t t_id;
    uint32_t strLen;
    char *s;
}trace_id_t;

// Macros to log ID based data

#define MBED_LOG_ID_4(cond, ...)                       do { if(cond) {log_id_data(__VA_ARGS__);}} while(0);
#define MBED_LOG_ID_3(cond, counter, id, args, fmt, ...)   { volatile static const __attribute__((section(".keep.log_data"))) char str##counter[] = fmt; \
                                                             volatile static const __attribute__((section(".keep.log_data"))) uint32_t len##counter = MBED_STRLEN(fmt); \
                                                             volatile static const __attribute__((section(".keep.log_data"))) uint32_t c##counter = id; \
                                                             MBED_LOG_ID_4(cond, args, id, ##__VA_ARGS__); \
                                                           }
#define MBED_LOG_ID_2(cond, counter, id, ...)     MBED_LOG_ID_3(cond, counter, id, MBED_COUNT_VA_ARGS(__VA_ARGS__), ##__VA_ARGS__)
#define MBED_LOG_ID_1(cond, counter, id, ...)     MBED_LOG_ID_2(cond, counter, id, __VA_ARGS__)

// Macros to log string data
#define MBED_LOG_STR(cond, ...)             do { if(cond) {log_string_data(__VA_ARGS__);}} while(0);
#define MBED_LOG_STR_1(cond, fmt, ...)      MBED_LOG_STR(cond, fmt "\n", ##__VA_ARGS__)

void log_id_data(uint8_t argCount, ...);
void log_string_data(const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif  // MBED_LOGGER_H
/**@}*/

/**@}*/
