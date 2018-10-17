
#ifndef EMAC_INTERFACE_STATS_H
#define EMAC_INTERFACE_STATS_H

#ifdef MBED_NW_STATS_ENABLED

#include "NetworkInterface.h"
#include "platform/StatsList.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    uint32_t connect_status : 8;
    uint32_t dhcp : 1;
    uint32_t resv : 23;
    uint8_t ip[NSAPI_IPv6_SIZE];
    uint8_t gateway[NSAPI_IPv4_SIZE];
    uint8_t netmask[NSAPI_IPv4_SIZE];
    uint8_t mac[NSAPI_MAC_SIZE];
    nw_common_stats_t tcp;
    nw_common_stats_t udp;
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

    static void emac_log_add(uint32_t *member, uint32_t count)
    {
        (*member) += count;
    }
    
    static void emac_log_add(uint16_t *member, uint16_t count)
    {
        (*member) += count;
    }

    static void emac_log_assign(uint32_t *member, uint32_t count)
    {
        (*member) = count;
    }

    static void emac_log_sub(uint32_t *member, uint32_t count)
    {
        (*member) -= count;
    }

    static void emac_log_sub(uint16_t *member, uint16_t count)
    {
        (*member) -= count;
    }

public:
    static nw_emac_info_t _emac_stats;
protected:
    void read_stats(mbed::stats_info_t* stats)
    {
        MBED_ASSERT(stats != NULL);
        stats->stats_type = mbed::STATS_NW_EMAC;
        stats->buf_size = sizeof(nw_emac_info_t);
        stats->buf = (void *)&_emac_stats;
    }
};

#define EMAC_STAT_INC(x)         EMACInterfaceStats::emac_log_add(&(EMACInterfaceStats::_emac_stats.x), 1)
#define EMAC_STAT_DEC(x)         EMACInterfaceStats::emac_log_sub(&(EMACInterfaceStats::_emac_stats.x), 1)
#define EMAC_STAT_ADD(x, n)      EMACInterfaceStats::emac_log_add(&(EMACInterfaceStats::_emac_stats.x), n)
#define EMAC_STAT_ASSIGN(x, n)   EMACInterfaceStats::emac_log_assign(&(EMACInterfaceStats::_emac_stats.x), n)
#else
#define EMAC_STAT_INC(x)
#define EMAC_STAT_DEC(x)
#define EMAC_STAT_ADD(x, n)
#define EMAC_STAT_ASSIGN(x, n) 
#endif

#endif

/** @}*/
