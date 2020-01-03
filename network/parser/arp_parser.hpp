#ifndef __ARP_SNIFFER__
#define __ARP_SNIFFER__

#define ARP_SNIFFER_DEFAULT_LOG_NAME "arp_parser"

#define ARP_REQUEST 1
#define ARP_REPLY 2

#include <string>

#include <pcap.h>
#include <arpa/inet.h>

#include "../../util/logger.hpp"
#include "../listener/sniffer_listener.hpp"
#include "../../util/util.hpp"

typedef struct arp_header {
    u_int16_t htype; // hardware type
    u_int16_t ptype; // protocol type
    u_char hlen; // hardware address length
    u_char plen; // protocol address length
    u_int16_t oper; // operation code
    u_char sha[6]; // sender hardware address (mac)
    u_char spa[4]; // sender ip address (ipv4)
    u_char tha[6]; // target hardware address (mac)
    u_char tpa[4]; // target ip address (ipv4)

} arp_header_t;

class CARPParser
{
public:
    CARPParser();
    CARPParser(std::string log_name);
    ~CARPParser();

    int parse(const unsigned char* p_arp_header);
    void register_listener(CSnifferListener* listener);

private:
    int log(const arp_header_t* arp_header);
    int inform_listeners(const arp_header_t* arp_header );

    bool is_ethernet(const arp_header_t* arp_header);
    bool is_ipv4(const arp_header_t* arp_header);
    bool is_arp_request(const arp_header_t* arp_header);
    bool is_arp_reply(const arp_header_t* arp_header);
    std::string get_sender_mac(const arp_header_t* arp_header);
    std::string get_sender_ipv4(const arp_header* arp_header);
    std::string get_receiver_mac(const arp_header_t* arp_header);
    std::string get_receiver_ipv4(const arp_header_t* arp_header);

    CLogger* logger;
    std::string log_name;
    std::vector<CSnifferListener*> listeners;
};


#endif
