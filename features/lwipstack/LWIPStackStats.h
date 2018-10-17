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

#ifndef LWP_STACK_STATS_H
#define LWIP_STACK_STATS_H

#ifdef MBED_NW_STATS_ENABLED

#include "platform/StatsList.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

typedef struct {    
    uint32_t pkt_xmit;         /* Transmitted packets. */
    uint32_t pkt_recv;         /* Received packets. */
    uint16_t sock_open;        /* Number of sockets opened */
}stack_stats_t;

typedef struct {
    uint32_t stack_addr;
    uint16_t connect_err;       /* Connection error. */
    uint16_t param_err;         /* Invalid parameter error - NSAPI_ERROR_PARAMETER */
    uint16_t mem_err;           /* Out of memory error - NSAPI_ERROR_NO_MEMORY */
    uint16_t socket_err;        /* Socket error - NSAPI_ERROR_NO_SOCKET */
    uint16_t addr_err;          /* Address error - NSAPI_ERROR_ADDRESS_IN_USE / NSAPI_ERROR_NO_ADDRESS */
    uint16_t err;               /* Misc error. */
    stack_stats_t udp;
    stack_stats_t tcp;
}nw_stack_stats_t;


/** LWIPStackStats class
 *
 *  Interface to get LWIP Stack statistics
 */
class LWIPStackStats: public mbed::StatsList
{
public:

    LWIPStackStats() : mbed::StatsList() {
        memset(&_lwip_stats, 0, sizeof(nw_stack_stats_t));
    }

    inline void log_stack_addr(uint32_t addr)
    {
        _lwip_stats.stack_addr = addr;
    }

    inline void lwip_log_add(uint32_t *member, uint32_t count)
    {
        (*member) += count;
    }
    
    inline void lwip_log_add(uint16_t *member, uint16_t count)
    {
        (*member) += count;
    }

    inline void lwip_log_sub(uint32_t *member, uint32_t count)
    {
        (*member) -= count;
    }

    inline void lwip_log_sub(uint16_t *member, uint16_t count)
    {
        (*member) -= count;
    }

    nw_stack_stats_t _lwip_stats;

protected:
    void read_stats(mbed::stats_info_t* stats)
    {
        MBED_ASSERT(stats != NULL);
        stats->stats_type = mbed::STATS_NW_STACK_LWIP;
        stats->buf_size = sizeof(nw_stack_stats_t);
        stats->buf = (void *)&_lwip_stats;
    }
};

#define LWIP_STAT_INC(obj, mem)      obj.lwip_log_add(&(obj._lwip_stats.mem), 1)
#define LWIP_STAT_DEC(obj, mem)      obj.lwip_log_sub(&(obj._lwip_stats.mem), 1)
#define LWIP_STAT_ADD(obj, mem, n)   obj.lwip_log_add(&(obj._lwip_stats.mem), n)
#else
#define LWIP_STAT_INC(obj, mem)
#define LWIP_STAT_DEC(obj, mem)
#define LWIP_STAT_ADD(obj, mem, n)
#endif

#endif

/** @}*/
