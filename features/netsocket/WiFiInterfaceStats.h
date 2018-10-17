
#ifndef WIFI_INTERFACE_STATS_H
#define WIFI_INTERFACE_STATS_H

#ifdef  NOT_NOW //MBED_NW_STATS_ENABLED

#include "platform/StatsList.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

namespace mbed {

typedef struct {
    uint32_t type : 3;
    uint32_t connected : 1;
    uint32_t firmware_ok : 1;
    uint32_t resv : 3;
    uint32_t sec_type : 8;
    uint32_t channel : 8;
    uint32_t rssi : 8;
    uint8_t ip[16];
    uint8_t gateway[16];
    uint8_t netmask[16];
    uint8_t mac[18];
    uint8_t ssid[33];
    nw_common_stats_t nw_stats;
}nw_wifi_info_t;

/** WiFiInterfaceStats class
 *
 *  Interface to get Wifi statistics
 *  @addtogroup netsocket
 */
class WiFiInterfaceStats: public StatsList
{
public:
    
    WiFiInterfaceStats() : StatsList() {
        memset(&_stats, 0, sizeof(nw_wifi_info_t));
    }

    inline void log_ip(uint8_t *ip)
    {
        MBED_ASSERT(ip != NULL);
        memcpy(&_stats.ip, ip, sizeof(_stats.ip));
    }

    inline void log_gateway(uint8_t *gateway)
    {
        MBED_ASSERT(gateway != NULL);
        memcpy(&_stats.gateway, gateway, sizeof(_stats.gateway));
    }

    inline void log_netmask(uint8_t *netmask)
    {
        MBED_ASSERT(netmask != NULL);
        memcpy(&_stats.netmask, netmask, sizeof(_stats.netmask));
    }
    
    inline void log_mac(uint8_t *mac)
    {
        MBED_ASSERT(mac != NULL);
        memcpy(&_stats.mac, mac, sizeof(_stats.mac));
    }
    
    inline void log_status(bool connected)
    {
        _stats.connected = connected;
    }
    
    inline void log_xmit(uint32_t count)
    {
//        _stats.nw_stats.xmit += count;
    }
    
    inline void log_recv(uint32_t count)
    {
  //      _stats.nw_stats.recv += count;
    }
    
    inline void log_fwd(uint32_t count)
    {
//        _stats.nw_stats.fwd += count;
    }

    inline void log_drop(uint32_t count)
    {
//        _stats.nw_stats.drop += count;
    }

    inline void log_chkerr()
    {
//        ++_stats.nw_stats.chkerr;
    }

    inline void log_lenerr()
    {
//        ++_stats.nw_stats.lenerr;
    }

    inline void log_memerr()
    {
 //       ++_stats.nw_stats.memerr;
    }

    inline void log_proterr()
    {
  //      ++_stats.nw_stats.proterr;
    }

    inline void log_opterr()
    {
//        ++_stats.nw_stats.opterr;
    }

    inline void log_err()
    {
//        ++_stats.nw_stats.err;
    }
    
protected:
    void read_stats(stats_info_t* stats)
    {
        MBED_ASSERT(stats != NULL);
        stats->stats_type = STATS_NW_WIFI;
        stats->buf_size = sizeof(nw_wifi_info_t);
        memcpy((void *)&stats->buf, (void *)&_stats, sizeof(nw_wifi_info_t));
    }

private:
    nw_wifi_info_t _stats;
};

} // namespace mbed

#endif
#endif

/** @}*/
