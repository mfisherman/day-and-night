#include "tcp_listener.hpp"


CTCPListener::CTCPListener() : CTCPListener(TCP_LISTENER_DEFAULT_LOG_NAME)
{
    // empty on purpose
}


CTCPListener::CTCPListener(std::string p_log_name) :
              CTCPListener(p_log_name, TCP_LISTENER_DEFAULT_TIMEOUT_MSEC,
                           TCP_LISTENER_DEFAULT_HOST_IP, TCP_LISTENER_DEFAULT_REQUEST_THRESHOLD,
                           std::vector<std::pair<uint32_t, uint32_t>>())
{
    // empty on purpose
}


CTCPListener::CTCPListener(std::string p_log_name, int64_t p_timeout_msec,
                           std::string host_ip, uint32_t request_threshold, std::vector<std::pair<uint32_t, uint32_t>> port_weight_list)
{
    this->log_name = p_log_name;
    this->logger = CLogger::get_instance();
    this->timeout_msec = p_timeout_msec;
    this->request_count = 0;
    this->host_ip = host_ip;
    this->is_for_host = false;
    this->request_threshold = request_threshold;

    if(port_weight_list.empty())
    {
        this->port_weight_list = std::vector<std::pair<uint32_t, uint32_t>> { std::make_pair( 443, 1), std::make_pair(137, 10),
                                                                            std::make_pair(138, 10), std::make_pair(139, 10),
                                                                            std::make_pair(22, 25), std::make_pair(21, 25)  };
    }
    else
    {
        this->port_weight_list = port_weight_list;
    }
}


CTCPListener::~CTCPListener()
{
    // empty on purpose
}


void CTCPListener::run_blocking()
{
    logger->log_info(log_name, "timeout: " + std::to_string(timeout_msec));
    while(true)
    {
        //sleep for timeout
        logger->log_info(log_name, "Sleeping for " + std::to_string( timeout_msec ) + "msec");
        usleep( static_cast<uint32_t> (timeout_msec * 1000));

        //check if some request came in in this period
        logger->log_info(log_name, "Request count: " + std::to_string(request_count) +
                         ", request threshold: " + std::to_string(this->request_threshold));
        if( request_count < this->request_threshold)
        {
            shutdown_linux();
            break;
        }
        logger->log_info(log_name, "Reseting the request counter");
        request_count = 0;
    }
}


std::thread* CTCPListener::run_non_blocking()
{
    auto thread = new std::thread(&CTCPListener::run_blocking, this);
    return thread;
}


void CTCPListener::shutdown_linux()
{
    logger->log_warning(log_name, "Shutting down system in 5 seconds");
    logger->log_warning(log_name, "Bye");
    usleep( 5*1000*1000);

    if ( system("shutdown -P now") != 0)
    {
        logger->log_error(log_name, "System did not return 0");
    }
    else
    {
        logger ->log_warning(log_name, "System is going to shut down");
    }
}


void CTCPListener::tcp_packet(uint32_t src_port, uint32_t dest_port)
{
    std::pair<uint32_t, uint32_t> pair;
    int32_t index = find_port(dest_port, &pair);
    if( (index != -1) && is_for_host)
    {
        logger->log_debug(log_name, "Packet, Source Port: " + std::to_string(src_port)+", Dest Port: "+std::to_string(dest_port));
        request_count += pair.second;
        logger->log_debug(log_name, "Increasing request count to " + std::to_string(request_count));
    }
}


void CTCPListener::ip_packet(std::string src_ip, std::string dest_ip)
{
    UNUSED(src_ip);
    is_for_host = (dest_ip == host_ip);
}


int32_t CTCPListener::find_port(uint32_t port, std::pair<uint32_t, uint32_t>* pair)
{
    for(uint32_t i = 0; i < this->port_weight_list.size(); i++)
    {
        auto value = port_weight_list[i];
        if(value.first == port)
        {
            pair->first = value.first;
            pair->second = value.second;
            return static_cast<int32_t>(i);
        }
    }
    return -1;
}
