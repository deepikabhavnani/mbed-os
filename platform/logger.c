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
#ifdef MBED_CONF_RTOS_PRESENT

#include "platform/logger_api.h"
#include "platform/logger.h"
#include "events/equeue/equeue.h"
#include "platform/mbed_error.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined (MBED_ID_BASED_TRACING)
#define LOG_ID_SINGLE_DATA_SIZE            (((MBED_CONFIG_MAX_LOG_ID_ARGS+2)*4)+1)
#define LOG_ID_EVENT_COUNT                 (MBED_CONFIG_LOG_MAX_BUFFER_SIZE/LOG_ID_SINGLE_DATA_SIZE)
#define LOG_SINGLE_EVENT_DATA_SIZE         LOG_ID_SINGLE_DATA_SIZE
#define LOG_TOTAL_EVENT_COUNT              LOG_ID_EVENT_COUNT
#else
#define LOG_STR_SINGLE_DATA_SIZE           (MBED_CONFIG_MAX_LOG_STR_SIZE)
#define LOG_STR_EVENT_COUNT                (MBED_CONFIG_LOG_MAX_BUFFER_SIZE/LOG_STR_SINGLE_DATA_SIZE)
#define LOG_SINGLE_EVENT_DATA_SIZE         LOG_STR_SINGLE_DATA_SIZE
#define LOG_TOTAL_EVENT_COUNT              LOG_STR_EVENT_COUNT
#endif

#define LOG_TOTAL_BUFFER_SIZE              (LOG_TOTAL_EVENT_COUNT * (LOG_SINGLE_EVENT_DATA_SIZE + EQUEUE_EVENT_SIZE))

static equeue_t log_queue;
static char log_buffer[LOG_TOTAL_BUFFER_SIZE];

void log_init(void)
{
    // creates a queue with space for 32 basic events
    equeue_create_inplace(&log_queue, LOG_TOTAL_BUFFER_SIZE, (void *)&log_buffer[0]);
}

void log_thread(void)
{
    equeue_dispatch(&log_queue, -1);
}

#if defined (MBED_ID_BASED_TRACING)
void log_buffer_id_data(uint8_t argCount, ...)
{   
    va_list args;
    va_start(args, argCount);
    int32_t *data_args;

    uint8_t *data = equeue_alloc(&log_queue, LOG_SINGLE_EVENT_DATA_SIZE);
    if (NULL == data) {
        printf("Logging queue out of buffer\n");
        return;
    }

    data[0] = argCount;
    data_args = (int32_t *)&data[1];
    
    for (uint8_t i = 0; i < argCount; i++) {
        data_args[i] = va_arg(args, int);
    }
    equeue_post(&log_queue, log_dump_id_data, data);
    va_end(args);
}

void log_dump_id_data(void *data)
{
#if DEVICE_STDIO_MESSAGES && !defined(NDEBUG)
    // Get arg size
    uint8_t *argCount = data;
    int32_t *args = (int32_t *)&argCount[1];
    printf("%d ", *argCount);
    for (uint8_t i = 0; i < *argCount; i++) 
    {
        printf("%d ", args[i]);
    }
    printf("\n");
#endif
}

#else 
void log_buffer_string_data(const char *format, ...)
{
    va_list args;
    va_start(args, format);    

    void *data = equeue_alloc(&log_queue, LOG_SINGLE_EVENT_DATA_SIZE);
    if (NULL == data) {
        printf("Logging queue out of buffer\n");
        return;
    }
    vsnprintf(data, LOG_SINGLE_EVENT_DATA_SIZE, format, args);
    equeue_post(&log_queue, log_dump_str_data, data);

    va_end(args);
}

void log_dump_str_data(void *data)
{
#if DEVICE_STDIO_MESSAGES && !defined(NDEBUG)
    printf("%s \n", (char *)data);
#endif
}

#endif

#ifdef __cplusplus
}
#endif

#endif
