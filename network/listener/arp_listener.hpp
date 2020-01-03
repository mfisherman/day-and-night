#ifndef __ARP_LISTENER_HPP__
#define __ARP_LISTENER_HPP__

#include <list>

#include "../../util/logger.hpp"
#include "../../util/util.hpp"
#include "sniffer_listener.hpp"
#include "../wol.hpp"

#define ARP_LISTENER_DEFAULT_LOG_NAME "sniffer"
#define ARP_LISTENER_DEFAULT_INTERVAL_MSEC (60*1000)
#define ARP_LISTENER_DEFAULT_FREQUENCY 8
#define ARP_LISTENER_DEFAULT_HOST_IP "192.168.178.99"

class CARPListener : public CSnifferListener
{
public:
    CARPListener(CWOL *p_wol);
    CARPListener(CWOL *p_wol, std::string p_log_name);
    CARPListener(CWOL *p_wol, std::string p_log_name, int64_t p_interval_msec, std::vector<uint32_t> p_frequency, std::string host_ip);
    virtual ~CARPListener();
    virtual void arp_request( std::string src_ip, std::string dest_ip );
    virtual void arp_response( std::string src_ip, std::string dest_ip );

private:
    bool has_frequency(std::list<int64_t>* arps_msec, int64_t interval_msec, uint32_t frequency);
    void add_frequency(std::list<int64_t>* arps_msec, uint32_t frequency);

    CLogger* logger;
    std::string log_name;
    CWOL* wol;
    std::list<int64_t> arps_msec;
    int64_t interval_msec;
    std::vector<uint32_t> frequency;
    uint32_t current_frequency;
    std::string host_ip;
};

#endif
