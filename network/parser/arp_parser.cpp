#include "arp_parser.hpp"


CARPParser::CARPParser() : CARPParser (ARP_SNIFFER_DEFAULT_LOG_NAME)
{
    // empty on purpose
}


CARPParser::CARPParser(std::string log_name)
{
    this->log_name = log_name;
    this->logger = CLogger::get_instance();
}


CARPParser:: ~CARPParser()
{
    // empty on purpose
}


void CARPParser::register_listener(CSnifferListener* listener)
{
    listeners.push_back(listener);
}


int32_t CARPParser::parse(const unsigned char* p_arp_header)
{
    const arp_header_t* arp_header = reinterpret_cast<const struct arp_header*> (p_arp_header);

    log(arp_header);
    inform_listeners(arp_header);

    return 0;
}


int32_t CARPParser::log(const arp_header_t* arp_header)
{
    std::string hw_type = is_ethernet(arp_header) ? "Ethernet" : "Unknown";
    std::string protocol_type = is_ipv4(arp_header) ? "IPV4" : "Unknown";
    std::string operation_type = is_arp_request(arp_header) ? "ARP Request" : "ARP Reply";

    std::string log_string = "Hardware Type: "+ hw_type+", Protocol type: "+ protocol_type+ ", Operation type: " + operation_type
                                + ", Sender MAC: "+ get_sender_mac(arp_header) +", Sender IP: "+ get_sender_ipv4(arp_header)
                                + ", Receiver MAC: "+ get_receiver_mac(arp_header) + ", Receiver IP: "+ get_receiver_ipv4(arp_header);

    logger->log_debug(log_name, log_string);
    return 0;
}


int32_t CARPParser::inform_listeners(const arp_header_t* arp_header )
{
    for(auto it = listeners.begin(); it < listeners.end(); it++)
    {
        if( is_arp_request(arp_header) )
        {
            (*it)->arp_request(get_sender_ipv4(arp_header), get_receiver_ipv4(arp_header));
        }
        else
        {
            (*it)->arp_response(get_sender_ipv4(arp_header), get_receiver_ipv4(arp_header));
        }
    }

    return 0;
}


bool CARPParser::is_ethernet(const arp_header_t* arp_header)
{
    return ntohs(arp_header->htype) == 1;
}


bool CARPParser::is_ipv4(const arp_header_t* arp_header)
{
    return ntohs(arp_header->ptype) == 0x0800;
}


bool CARPParser::is_arp_request(const arp_header_t* arp_header)
{
    return  ntohs(arp_header->oper) == ARP_REQUEST;
}


bool CARPParser::is_arp_reply(const arp_header_t* arp_header)
{
    return  ntohs(arp_header->oper) == ARP_REPLY;
}


std::string CARPParser::get_sender_mac(const arp_header_t* arp_header)
{
    return CUtil::parse_mac(arp_header->sha);
}


std::string CARPParser::get_sender_ipv4(const arp_header* arp_header)
{
    return CUtil::parse_ipv4(arp_header->spa);
}


std::string CARPParser::get_receiver_mac(const arp_header_t* arp_header)
{
    return CUtil::parse_mac(arp_header->tha);
}


std::string CARPParser::get_receiver_ipv4(const arp_header_t* arp_header)
{
    return CUtil::parse_ipv4( arp_header->tpa );
}
