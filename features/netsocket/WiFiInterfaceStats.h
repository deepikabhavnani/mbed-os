
#ifndef WIFI_INTERFACE_STATS_H
#define WIFI_INTERFACE_STATS_H

#include "platform/StatsList.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

namespace mbed {

/** WiFiInterfaceStats class
 *
 *  Interface to get Wifi statistics
 *  @addtogroup netsocket
 */
class WiFiInterfaceStats: public StatsList
{
public:
    
    WiFiInterfaceStats() : StatsList(STATS_NW_WIFI) {
        memset(&_stats, 0, sizeof(interface_info_t));
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
        _stats.is_connected = connected;
    }

    inline void log_data_sent(uint32_t count)
    {
        _stats.data_sent += count;
    }
    
    inline void log_data_recv(uint32_t count)
    {
        _stats.data_recv += count;
    }
    
protected:
    void read_stats(void* stats) {
        MBED_ASSERT(stats != NULL);
        memcpy((void *)stats, (void *)&_stats, sizeof(interface_info_t));
    }

private:
    interface_info_t _stats;
};

} // namespace mbed

#endif

/** @}*/
