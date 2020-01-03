#include "sniffer_listener.hpp"


CSnifferListener::CSnifferListener()
{
    // empty on purpose
}

CSnifferListener::~CSnifferListener()
{
    // empty on purpose
}


void CSnifferListener::packet( int64_t time_stamp_ms, uint32_t caplen, uint32_t len )
{
    UNUSED(time_stamp_ms);
    UNUSED(caplen);
    UNUSED(len);
}


void CSnifferListener::ethernet_frame(std::string src_mac, std::string dest_mac)
{
    UNUSED(src_mac);
    UNUSED(dest_mac);
}


void CSnifferListener::arp_request( std::string src_ip, std::string dest_ip )
{
    UNUSED(src_ip);
    UNUSED(dest_ip);
}

void CSnifferListener::arp_response( std::string src_ip, std::string dest_ip )
{
    UNUSED(src_ip);
    UNUSED(dest_ip);
}


void CSnifferListener::ip_packet(std::string src_ip, std::string dest_ip)
{
    UNUSED(src_ip);
    UNUSED(dest_ip);
}


void CSnifferListener::tcp_packet(uint32_t src_port, uint32_t dest_port)
{
    UNUSED(src_port);
    UNUSED(dest_port);
}
