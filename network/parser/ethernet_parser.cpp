#include "ethernet_parser.hpp"


CEthernetParser::CEthernetParser() : CEthernetParser(ETHERNET_SNIFFER_DEFAULT_LOG_NAME, new CARPParser(), new CIPParser() )
{
    // empty on purpose
}


CEthernetParser::CEthernetParser(std::string log_name, CARPParser *arp_parser, CIPParser* ip_sniffer)
{
    this->arp_parser = arp_parser;
    this->ip_sniffer = ip_sniffer;
    this->log_name = log_name;
    this->logger = CLogger::get_instance();
}


CEthernetParser::~CEthernetParser()
{
    // empty on purpose
}


void CEthernetParser::register_listener(CSnifferListener* listener)
{
    listeners.push_back(listener);

    this->arp_parser->register_listener(listener);
    this->ip_sniffer->register_listener(listener);
}


int32_t CEthernetParser::parse(const unsigned char *packet)
{
    const ethernet_header_t* ethernet_header = reinterpret_cast<const ethernet_header_t*> (packet);

    log(ethernet_header);
    inform_listeners(ethernet_header);

    if (is_ip(ethernet_header))
    {
        ip_sniffer->parse( packet + 14 );
    }
    else if (is_arp(ethernet_header))
    {
        arp_parser->parse(packet + 14);
    }
    else
    {
        logger->log_warning(log_name, "Unknown type");
    }

    return 0;
}


bool CEthernetParser::is_ip(const ethernet_header_t* ethernet_header)
{
    return ntohs(ethernet_header->ether_type) == ETHERTYPE_IP;
}


bool CEthernetParser::is_arp(const ethernet_header_t* ethernet_header)
{
    return ntohs(ethernet_header->ether_type) == ETHERTYPE_ARP;
}


int32_t CEthernetParser::log(const ethernet_header_t* ethernet_header)
{
    std::string type = "";
    if( is_ip(ethernet_header))
    {
        type = "IPV4";
    }
    else if( is_arp(ethernet_header))
    {
        type = "ARP";
    }
    else
    {
        type = "Unknown";
    }

    std::string log_string = "Source: " + CUtil::parse_mac(ethernet_header->ether_shost) + ", Destination: " + CUtil::parse_mac(ethernet_header->ether_dhost)
                            + ", Type: " + type;

    logger->log_debug(log_name, log_string);

    return 0;
}


int32_t CEthernetParser::inform_listeners(const ethernet_header_t* ethernet_header )
{
    for(auto it = listeners.begin(); it < listeners.end(); it++)
    {
        (*it)->ethernet_frame(CUtil::parse_mac(ethernet_header->ether_shost), CUtil::parse_mac(ethernet_header->ether_dhost));
    }

    return 0;

}
