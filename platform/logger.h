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

/** \addtogroup platform */
/** @{*/
/**
 * \defgroup platform_log Logger functions
 * @{
 */

#ifndef LOGGER_INTERNAL_H
#define LOGGER_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include "platform/mbed_preprocessor.h"

#ifndef MBED_CONF_MAX_LOG_STR_SIZE
#define MBED_CONF_MAX_LOG_STR_SIZE          128
#endif

#ifndef MBED_CONF_LOG_MAX_BUFFER_SIZE
#define MBED_CONF_LOG_MAX_BUFFER_SIZE       1024
#endif

#define LOG_TOTAL_BUFFER_SIZE    MBED_CONF_LOG_MAX_BUFFER_SIZE
#define LOG_SINGLE_STR_SIZE      MBED_CONF_MAX_LOG_STR_SIZE

#if defined(__ARMCC_VERSION)
#define FILE_NAME_               __MODULE__
#else
#define FILE_NAME_               __BASE_FILE__
#endif

#define LOG_LEVEL_ERR_CRITICAL   0x0
#define LOG_LEVEL_GEN            0x1
#define LOG_LEVEL_ERR            0x2
#define LOG_LEVEL_WARN           0x4
#define LOG_LEVEL_INFO           0x8
#define LOG_LEVEL_DEBUG          0x10
#define LOG_LEVEL_TRACE          0x20

// Log-Level Strings
#define LOG_GEN_                 "GEN "
#define LOG_ERR_CRITICAL_        "CRT "
#define LOG_ERR_                 "ERR "
#define LOG_WARN_                "WARN"
#define LOG_DEBUG_               "DBG "
#define LOG_INFO_                "INF0"
#define LOG_TRACE_               "TRAC"

#define GET_LOG_STRING(ll)      ((ll > LOG_LEVEL_DEBUG) ? LOG_TRACE_ : \
                                ((ll > LOG_LEVEL_INFO)  ? LOG_DEBUG_ : \
                                ((ll > LOG_LEVEL_WARN)  ? LOG_INFO_  : \
                                ((ll > LOG_LEVEL_ERR)   ? LOG_WARN_  : \
                                ((ll > LOG_LEVEL_GEN)   ? LOG_ERR_   : LOG_GEN_)))))

#define GET_MODULE_ID_(a, b, c, d)   (375 + (a << 1) + (b << 2) + (c >> 2) + (d >> 1))
#define GET_MODULE_ID(x)        (GET_MODULE_ID_(x[0], x[1], x[2], x[3]))

#define SET_MODULE(x)           (GET_MODULE_ID(x) & 0xFF)
#define SET_COUNTER(y)          ((y & 0xFF) << 8)
#define SET_LINE_NUM(z)         ((z & 0xFFFF) << 16)
#define TRACE_ID_(x,y,z)        (SET_MODULE(x) | SET_COUNTER(y) | SET_LINE_NUM(z))

// Macros to log ID based data
#define MBED_LOG_ID_4(...)                            log_buffer_id_data(__VA_ARGS__)
// Data dumped in special section is : Unique ID, format string length and format string
#define MBED_LOG_ID_3(counter, id, args, fmt, ...)    ({volatile static const __attribute__((section(".keep.log_data"))) char str##counter[] = fmt; \
                                                       volatile static const __attribute__((section(".keep.log_data"))) uint32_t len##counter = MBED_STRLEN(fmt); \
                                                       volatile static const __attribute__((section(".keep.log_data"))) uint32_t c##counter = id; \
                                                       MBED_LOG_ID_4(args, id, ##__VA_ARGS__); \
                                                      })
#define MBED_LOG_ID_2(counter, id, ...)               MBED_LOG_ID_3(counter, id, MBED_COUNT_VA_ARGS(__VA_ARGS__), ##__VA_ARGS__)
#define MBED_LOG_ID_1(mod, fmt, ll, f, l, c, ...)     MBED_LOG_ID_2(c, TRACE_ID_(mod,c,l), "[" mod "][" f "][" MBED_STRINGIFY(l) "]: " fmt, ##__VA_ARGS__)

// Macros to log string data
#define MBED_LOG_STR(...)                             log_buffer_string_data(__VA_ARGS__)
#define MBED_LOG_STR_1(mod, fmt, ll, ...)             MBED_LOG_STR("[%-4.4s][%-4.4s]: " fmt, ll, mod, ##__VA_ARGS__)

// Assert string
#define MBED_LOG_ASSERT(...)                          log_assert(__VA_ARGS__)
#define MBED_LOG_ASSERT_1(mod, fmt, ll, f, l, ...)    MBED_LOG_ASSERT("[%-4.4s][%-4.4s][%-15s][%5d]: " fmt, ll, mod, f, l, ##__VA_ARGS__)

#define MBED_LOG_VSTR_2(fmt1, ap)                     log_buffer_string_vdata(fmt1, ap)
#define MBED_LOG_VSTR_1(ll, mod, fmt, ap)             MBED_LOG_VSTR_2("[" ll "][" mod "]: " fmt, ap)

void log_buffer_id_data(uint8_t argCount, ...);
void log_buffer_string_data(const char *format, ...) __attribute__ ((__format__(__printf__, 1, 2)));
void log_buffer_string_vdata(const char *format, va_list args);
void log_assert(const char *format, ...) __attribute__ ((__format__(__printf__, 1, 2)));

#ifdef __cplusplus
}
#endif

#endif  // LOGGER_INTERNAL_H
/**@}*/

/**@}*/
