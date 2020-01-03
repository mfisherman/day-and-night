#ifndef __ARP_LISTENER_HPP__
#define __ARP_LISTENER_HPP__

#include <atomic>
#include <thread>

#include "sniffer_listener.hpp"
#include "../../util/logger.hpp"
#include "../wol.hpp"

#define TCP_LISTENER_DEFAULT_LOG_NAME "sniffer"
#define TCP_LISTENER_DEFAULT_TIMEOUT_MSEC (5*60*1000)
#define TCP_LISTENER_DEFAULT_HOST_IP "192.168.178.99"
#define TCP_LISTENER_DEFAULT_REQUEST_THRESHOLD 25

class CTCPListener : public CSnifferListener
{
public:
    CTCPListener();
    CTCPListener(std::string p_log_name);
    CTCPListener(std::string p_log_name, int64_t p_timeout_msec, std::string host_ip, uint32_t request_threshold, std::vector<std::pair<uint32_t, uint32_t>> port_weight_list);
    virtual ~CTCPListener();
    void run_blocking();
    std::thread* run_non_blocking();
    virtual void tcp_packet(uint32_t src_port, uint32_t dest_port);
    virtual void ip_packet(std::string src_ip, std::string dest_ip);

private:
    void shutdown_linux();
    int32_t find_port(uint32_t port, std::pair<uint32_t, uint32_t>* pair);

    CLogger* logger;
    std::string log_name;
    int64_t timeout_msec;
    std::atomic<uint64_t> request_count;
    std::string host_ip;
    bool is_for_host;
    uint32_t request_threshold;
    std::vector<std::pair<uint32_t, uint32_t>> port_weight_list;
};

#endif

