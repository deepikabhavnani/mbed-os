/** \addtogroup platform */
/** @{*/
/**
 * \defgroup platform_stats stats functions
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
#ifndef STATS_LIST_H
#define STATS_LIST_H

#include "platform/platform.h"
#include "platform/SingletonPtr.h"
#include "platform/PlatformMutex.h"
#include "platform/NonCopyable.h"
#include "platform/mbed_stats.h"

namespace mbed {

/** Stats type */
typedef enum {
    STATS_RTOS_GEN = 0x0,
    
    STATS_PLATFORM_GEN = 0x20,
    
    STATS_STORAGE_GEN = 0x40,
    
    STATS_NW_GEN = 0x60,
    STATS_NW_EMAC,
    STATS_NW_WIFI,
    STATS_NW_MESH,
    STATS_NW_CELLULAR,    
}mbed_stats_type_t;

typedef struct  {
    mbed_stats_type_t stats_type;
    uint32_t buf_size;
    void *buf;
}stats_info_t;

class StatsList
{
public:
    
    StatsList();

    ~StatsList();
    /**
     *  Get all the information related to stats recorded in class
     *
     *  @param stats    A pointer to the data structure to fill
     */
    static int get_each(stats_info_t *stats, int count);
    static int get_each(stats_info_t *stats, mbed_stats_type_t type, int count);

    virtual void read_stats(stats_info_t* stats) = 0;

private:
    static StatsList *_head;
    static SingletonPtr<PlatformMutex> _mutex;
    StatsList   *_next;
};

} // namespace mbed

#endif

/** @}*/

/** @}*/
