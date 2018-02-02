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
#include "platform/logger_api.h"
#include "platform/logger.h"


#ifdef __cplusplus
extern "C" {
#endif

void log_string_data(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
}

void log_id_data(uint8_t argCount, ...)
{
    printf("%d ", argCount);
    va_list args;
    va_start(args, argCount);
    for (uint8_t i = 0; i < argCount; i++) {
        printf("0x%lx ", va_arg(args, uint32_t));
    }
    printf("\n");
    va_end(args);
}


#ifdef __cplusplus
}
#endif
