#include "ip_parser.hpp"


CIPParser::CIPParser() : CIPParser(IP_PARSER_DEFAULT_LOG_NAME, new CTCPParser())
{
    // empty on purpose
}


CIPParser::CIPParser(std::string log_name, CTCPParser* tcp_parser)
{
    this->log_name = log_name;
    this->logger = CLogger::get_instance();
    this->tcp_parser = tcp_parser;
}


CIPParser::~CIPParser()
{
    // empty on purpose
}


int32_t CIPParser::parse(const unsigned char* p_ip_header)
{
    const ip_header_t* ip_header = reinterpret_cast<const ip_header_t*> (p_ip_header);

    log(ip_header);
    inform_listeners(ip_header);

    int size_ip = IP_HL(ip_header)*4;

    if ( is_tcp( ip_header ))
    {
        tcp_parser->parse(p_ip_header + size_ip);
    }
    else if( is_udp( ip_header))
    {
        // empty on purpose
    }
    else if( is_icmp( ip_header))
    {
        // empty on purpose
    }
    else if( is_ip( ip_header))
    {
        // empty on purpose
    }

    return 0;
}


int32_t CIPParser::log(const ip_header_t* ip_header)
{
    std::string type = "";
    if ( is_tcp( ip_header ))
    {
        type = "TCP";
    }
    else if( is_udp( ip_header))
    {
        type = "UDP";
    }
    else if( is_icmp( ip_header))
    {
        type = "IP";
    }
    else if( is_ip( ip_header))
    {
        type = "UNKNOWN";
    }

    std::string log_string = "Source: " + CUtil::parse_ipv4(ip_header->ip_src) + ", Destination: " + CUtil::parse_ipv4(ip_header->ip_dst) + ", Type: "+type;
    logger->log_debug(log_name, log_string);

    return 0;
}


void CIPParser::register_listener(CSnifferListener* listener)
{
    listeners.push_back(listener);
    this->tcp_parser->register_listener(listener);
}


int32_t CIPParser::inform_listeners(const ip_header_t* ip_header )
{
    for(auto it = listeners.begin(); it < listeners.end(); it++)
    {
        (*it)->ip_packet(get_sender_ipv4(ip_header), get_receiver_ipv4(ip_header));
    }

    return 0;
}


bool CIPParser::is_tcp(const ip_header_t* ip_header )
{
    return ip_header->ip_p == IPPROTO_TCP;
}


bool CIPParser::is_udp(const ip_header_t* ip_header)
{
    return ip_header->ip_p == IPPROTO_UDP;
}


bool CIPParser::is_icmp(const ip_header_t* ip_header)
{
    return ip_header->ip_p == IPPROTO_ICMP;
}


bool CIPParser::is_ip(const ip_header_t* ip_header)
{
    return ip_header->ip_p == IPPROTO_IP;
}


std::string CIPParser::get_sender_ipv4(const ip_header_t* ip_header)
{
    return CUtil::parse_ipv4( ip_header->ip_src );
}


std::string CIPParser::get_receiver_ipv4(const ip_header_t* ip_header)
{
    return CUtil::parse_ipv4( ip_header->ip_dst );
}
