#include "tcp_parser.hpp"


CTCPParser::CTCPParser() : CTCPParser(TCP_PARSER_DEFAULT_LOG_NAME )
{
    // empty on purpose
}


CTCPParser::CTCPParser(std::string log_name)
{
    this->log_name = log_name;
    this->logger = CLogger::get_instance();
}


CTCPParser::~CTCPParser()
{
    // empty on purpose
}


int32_t CTCPParser::parse(const unsigned char* p_tcp_header)
{
    const tcp_header_t* tcp_header = reinterpret_cast<const tcp_header_t*> (p_tcp_header);

    log(tcp_header);
    inform_listeners(tcp_header);

    return 0;
}


void CTCPParser::register_listener(CSnifferListener* listener)
{
    listeners.push_back(listener);
}


int32_t CTCPParser::log(const tcp_header* tcp_header)
{
    std::string log_string = "Src Port: "+ std::to_string(ntohs ( tcp_header->th_sport ) ) + ", Dst Port: "+ std::to_string( ntohs ( tcp_header->th_dport) ) ;
    logger->log_debug(log_name, log_string);

    return 0;
}


int32_t CTCPParser::inform_listeners(const tcp_header_t* tcp_header )
{
    for(auto it = listeners.begin(); it < listeners.end(); it++)
    {
        (*it)->tcp_packet( ntohs( tcp_header->th_sport ), ntohs( tcp_header->th_dport ) );
    }

    return 0;
}
