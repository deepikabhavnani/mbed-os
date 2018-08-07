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

namespace mbed {

/** Stats type */
typedef enum {
    STATS_RTOS_GEN = 0x0,
    
    STATS_PLATFORM_GEN = 0x20,
    
    STATS_STORAGE_GEN = 0x40,
    
    STATS_NW_GEN = 0x60,
    STATS_NW_ETH,
    STATS_NW_WIFI,
    STATS_NW_MESH,
    STATS_NW_CELLULAR,    
}mbed_stats_type_t;

typedef struct  {
}eth_info_t;

typedef struct  {
    uint8_t ssid[33];
    uint8_t sec_type;
    int8_t rssi;
    uint8_t channel;
    bool firmware_ok;
}wifi_info_t;

typedef struct  {
}mesh_info_t;

typedef struct  {
    const uint8_t *apn;
    const uint8_t *uname;
}cellular_info_t;

typedef union {
    eth_info_t eth_info;
    wifi_info_t wifi_info;
    mesh_info_t mesh_info;
    cellular_info_t cellular_info;
}iface_info_t;

typedef struct  {
    uint8_t ip[16];
    uint8_t gateway[16];
    uint8_t netmask[16];
    uint8_t mac[18];
    bool is_connected;
    uint32_t data_sent;
    uint32_t data_recv;
    iface_info_t info;
}interface_info_t; 

class StatsList
{
public:
    
    StatsList();
    StatsList(mbed_stats_type_t type);

    ~StatsList();
    /**
     *  Get all the information related to stats recorded in class
     *
     *  @param stats    A pointer to the data structure to fill
     */
    static int get_each(void *stats, int count);
    
    virtual void read_stats(void* stats) = 0;

private:
    static StatsList *_head;
    static SingletonPtr<PlatformMutex> _mutex;

    StatsList   *_next;
    mbed_stats_type_t _type;
};

} // namespace mbed

#endif

/** @}*/

/** @}*/
