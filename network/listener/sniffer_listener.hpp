#ifndef __SNIFFER_LISTENER_HPP__
#define __SNIFFER_LISTENER_HPP__

#include <stdint.h>
#include <string>

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif

class CSnifferListener
{
public:
    CSnifferListener();
    virtual ~CSnifferListener();

    virtual void packet(int64_t time_stamp_ms, uint32_t caplen, uint32_t len);
    virtual void ethernet_frame(std::string src_mac, std::string dest_mac);
    virtual void arp_request(std::string src_ip, std::string dest_ip);
    virtual void arp_response(std::string src_ip, std::string dest_ip);
    virtual void ip_packet(std::string src_ip, std::string dest_ip);
    virtual void tcp_packet(uint32_t src_port, uint32_t dest_port);
};


#endif
