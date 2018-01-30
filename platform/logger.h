
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

#ifndef MBED_CONFIG_MAX_LOG_LEVEL
#define MBED_CONFIG_MAX_LOG_LEVEL           5
#endif

#ifndef MBED_CONFIG_MAX_LOG_ID_ARGS
#define MBED_CONFIG_MAX_LOG_ID_ARGS         10
#endif

#ifndef MBED_CONFIG_MAX_LOG_STR_SIZE
#define MBED_CONFIG_MAX_LOG_STR_SIZE        64
#endif

#ifndef MBED_CONFIG_LOG_MAX_BUFFER_SIZE
#define MBED_CONFIG_LOG_MAX_BUFFER_SIZE     1024
#endif
    
#define SEPARATOR                 " "
#define FMT_SEP                   SEPARATOR ":" SEPARATOR

#define GET_STRING_LEN_(x)        (sizeof(x) - 1)
#define GET_STRING_LEN(x)         GET_STRING_LEN_(x)
#define GET_STRING_2(x)           #x
#define GET_STRING(x)             GET_STRING_2(x)
#if defined(__ARMCC_VERSION)
#define FILE_INFO                 __MODULE__ SEPARATOR GET_STRING(__LINE__)
#else
#define FILE_INFO                 __BASE_FILE__ SEPARATOR GET_STRING(__LINE__)
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
#define LOG_ERR_CRITICAL        SEPARATOR "C" SEPARATOR
#define LOG_ERR                 SEPARATOR "E" SEPARATOR
#define LOG_WARN                SEPARATOR "W" SEPARATOR
#define LOG_DEBUG               SEPARATOR "D" SEPARATOR
#define LOG_INFO                SEPARATOR "I" SEPARATOR
#define LOG_TRACE               SEPARATOR "T" SEPARATOR

#define SET_LEVEL(x)     (((x) << 0) & 0xF)
#define SET_COUNTER(y)   (((y) << 8) & 0xFF)
#define TRACE_ID(x,y)    (SET_LEVEL(x) | SET_COUNTER(y))

// Count the arguments in macro
#define GET_NTH_ARG(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, N, ...)   N
#define COUNT_VARARGS(...)      GET_NTH_ARG(__VA_ARGS__, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)

#define TERMINATING_STR        "END;"
#define APPEND_END_STR(x)      (x TERMINATING_STR)

typedef struct trace_id_data {
    uint8_t argCount;
    int32_t *args;
}trace_id_data_t;

// Macros to log ID based data
#define MBED_LOG_ID_3(cond, ...)                       do { if(cond) {log_buffer_id_data(__VA_ARGS__);}} while(0);
#define MBED_LOG_ID_2(cond, counter, args, fmt, ...)   { volatile static const __attribute__((section(".keep.log_data"))) char str ## counter[] = APPEND_END_STR(fmt); \
                                                         volatile static const __attribute__((section(".keep.log_data"))) uint32_t len ## counter = GET_STRING_LEN(fmt); \
                                                         volatile static const __attribute__((section(".keep.log_data"))) uint32_t id ## counter = counter; \
                                                         MBED_LOG_ID_3(cond, args, counter, ##__VA_ARGS__); \
                                                       }
#define MBED_LOG_ID_1(cond, counter, ...)              MBED_LOG_ID_2(cond, counter, COUNT_VARARGS(__VA_ARGS__), ##__VA_ARGS__)
                                                  
// Macros to log string data
#define MBED_LOG_STR(cond, ...)                        do { if(cond) {log_buffer_string_data(__VA_ARGS__);}} while(0);
#define MBED_LOG_STR_1(cond, fmt, ...)                 MBED_LOG_STR(cond, fmt "\n", ##__VA_ARGS__)

void log_dump_str_data(void *data);
void log_dump_id_data(void *data);
void log_buffer_id_data(uint8_t argCount, ...);
void log_buffer_string_data(const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif  // MBED_LOGGER_H
/**@}*/

/**@}*/
