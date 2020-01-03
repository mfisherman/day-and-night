#include "arp_listener.hpp"


CARPListener::CARPListener(CWOL *p_wol)  : CARPListener(p_wol, ARP_LISTENER_DEFAULT_LOG_NAME)
{
    // empty on purpose
}


CARPListener::CARPListener(CWOL *p_wol, std::string p_log_name) :
              CARPListener(p_wol, p_log_name, ARP_LISTENER_DEFAULT_INTERVAL_MSEC,
                           std::vector<uint32_t> (24, ARP_LISTENER_DEFAULT_FREQUENCY), ARP_LISTENER_DEFAULT_HOST_IP)
{
    // empty on purpose
}


CARPListener::CARPListener(CWOL *p_wol, std::string p_log_name, int64_t p_interval_msec, std::vector<uint32_t> p_frequency, std::string host_ip)
{
    this->log_name = p_log_name;
    this->logger = CLogger::get_instance();
    this->wol = p_wol;
    this->interval_msec = p_interval_msec;
    this->frequency = p_frequency;
    this->host_ip = host_ip;
    current_frequency = p_frequency[ CUtil::get_current_hour() ];

    logger->log_info(this->log_name, "interval_msec=" + std::to_string(this->interval_msec) +
                        ", current_frequency=" + std::to_string(this->current_frequency) +
                        ", host_ip=" + host_ip + ", length of frequency array="+ std::to_string(this->frequency.size()) );
}


CARPListener::~CARPListener()
{
    // empty on purpose
}


void CARPListener::arp_request(std::string src_ip, std::string dest_ip)
{
    UNUSED(src_ip);
    logger->log_debug(log_name, "Received arp request for dest_ip="+dest_ip);
    if( dest_ip == host_ip)
    {
        // check, if the frequency has changed
        if( current_frequency != frequency[CUtil::get_current_hour()])
        {
            current_frequency = frequency[CUtil::get_current_hour()];
            arps_msec.clear();
        }


        logger->log_debug(log_name, "Matching dest_ip and host_ip (server)");
        add_frequency(&arps_msec, current_frequency);
        if( has_frequency(&arps_msec, interval_msec, current_frequency))
        {
            wol->send_wol();
        }
        else
        {
            logger->log_warning(log_name, "Do not send WOL, frequency not reached");
        }
    }
}


void CARPListener::arp_response(std::string src_ip, std::string dest_ip )
{
    UNUSED(dest_ip);
    logger->log_debug(log_name, "Received arp response");
    if( src_ip == host_ip)
    {
        logger->log_info(log_name, "SERVER is ONLINE");
    }
}


void CARPListener::add_frequency(std::list<int64_t>* arps_msec, uint32_t frequency)
{
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    int64_t curr_msec = ms.count();
    arps_msec->push_front(curr_msec );

    if(arps_msec->size() > frequency)
    {
        arps_msec->pop_back();
    }
}


bool CARPListener::has_frequency(std::list<int64_t>* arps_msec, int64_t interval_msec, uint32_t frequency)
{
    int64_t first = arps_msec->front();
    int64_t last = arps_msec->back();
    int64_t diff = labs(first - last);

    logger->log_info(log_name, "Received "+std::to_string(arps_msec->size()) +" many arp requests in "+ std::to_string(diff) + "msec");

    return (diff <= interval_msec ) && (arps_msec->size() >= frequency);
}
