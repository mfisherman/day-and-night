#ifndef __ETHERNET_SNIFFER_HPP__
#define __ETHERNET_SNIFFER_HPP__

#include <stdlib.h>
#include <string.h>
#include <vector>

#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>

#include "../../util/logger.hpp"
#include "../listener/sniffer_listener.hpp"
#include "arp_parser.hpp"
#include "ip_parser.hpp"

#define ETHERNET_SNIFFER_DEFAULT_LOG_NAME "ethernet_sniffer"

typedef struct ethernet_header {
        u_char  ether_dhost[6];    /* destination host address */
        u_char  ether_shost[6];    /* source host address */
        u_short ether_type;                     /* IP? ARP? RARP? etc */
} ethernet_header_t;


class CEthernetParser
{

public:
    CEthernetParser();
    CEthernetParser(std::string log_name, CARPParser *arp_parser, CIPParser* ip_sniffer);
    ~CEthernetParser();

    void register_listener(CSnifferListener* listener);
    int32_t parse(const unsigned char *packet);

private:
    int32_t log(const ethernet_header_t* ethernet_header);
    int32_t inform_listeners(const ethernet_header_t* ethernet_header );
    bool is_ip(const ethernet_header_t* ethernet_header);
    bool is_arp(const ethernet_header_t* ethernet_header);

    CLogger* logger;
    std::vector<CSnifferListener*> listeners;
    std::string log_name;
    CARPParser* arp_parser;
    CIPParser* ip_sniffer;

};

#endif
