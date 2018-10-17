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

#ifndef EMAC_INTERFACE_STATS_H
#define EMAC_INTERFACE_STATS_H

#ifdef MBED_NW_STATS_ENABLED

#include "NetworkInterface.h"
#include "platform/StatsList.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    uint32_t stack_addr;
    uint8_t ip[NSAPI_IPv6_SIZE];
    uint8_t gateway[NSAPI_IPv4_SIZE];
    uint8_t netmask[NSAPI_IPv4_SIZE];
    uint8_t mac[NSAPI_MAC_SIZE];
    uint8_t connect_status;
    bool dhcp;
}nw_emac_info_t;

/** EMACInterfaceStats class
 *
 *  Interface to get EMAC statistics
 *  @addtogroup netsocket
 */
class EMACInterfaceStats: public mbed::StatsList
{
public:

    EMACInterfaceStats() : mbed::StatsList() {
        memset(&_emac_stats, 0, sizeof(nw_emac_info_t));
    }

    inline void log_ip(const char *ip)
    {
        MBED_ASSERT(ip != NULL);
        memcpy((void *)&_emac_stats.ip, (const void *)ip, NSAPI_IPv6_SIZE);
    }

    inline void log_gateway(const char *gateway)
    {
        MBED_ASSERT(gateway != NULL);
        memcpy((void *)&_emac_stats.gateway, (const void *)gateway, NSAPI_IPv4_SIZE);
    }

    inline void log_netmask(const char *netmask)
    {
        MBED_ASSERT(netmask != NULL);
        memcpy((void *)&_emac_stats.netmask, (const void *)netmask, NSAPI_IPv4_SIZE);
    }
    
    inline void log_mac(const char *mac)
    {
        MBED_ASSERT(mac != NULL);
        memcpy((void *)&_emac_stats.mac, (const void *)mac, NSAPI_MAC_SIZE);
    }

    inline void log_dhcp(bool dhcp)
    {
        _emac_stats.dhcp = dhcp;
    }

    inline void log_status(uint8_t connect_status)
    {
        _emac_stats.connect_status = connect_status;
    }

    inline void log_stack_addr(uint32_t addr)
    {
        _emac_stats.stack_addr = addr;
    }
    
private:
    nw_emac_info_t _emac_stats;

protected:
    void read_stats(mbed::stats_info_t* stats)
    {
        MBED_ASSERT(stats != NULL);
        stats->stats_type = mbed::STATS_NW_INTERFACE_EMAC;
        stats->buf_size = sizeof(nw_emac_info_t);
        stats->buf = (void *)&_emac_stats;
    }
};

#endif
#endif

/** @}*/
