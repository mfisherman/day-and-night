#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

#include <string>
#include <fstream>

#include <jsoncpp/json/json.h>

class CConfig
{
public:
    CConfig();
    CConfig(std::string file_name);
    ~CConfig();

    std::string iface_name();
    std::string log_file_name();
    std::string log_priority_level();
    std::string main_log_name();
    std::string sniffer_log_name();
    std::string arp_listener_log_name();
    uint32_t tcp_listener_timeout_msec();
    std::string tcp_listener_log_name();
    std::string wol_address();
    std::string wol_log_name();
    int64_t arp_parser_interval_msec();
    uint32_t arp_parser_frequency();
    std::string ip_parser_log_name();
    std::string arp_parser_log_name();
    std::string ethernet_parser_log_name();
    std::string tcp_parser_log_name();
    std::string host_ip();
    uint32_t arp_listener_interval_msec();
    std::vector<uint32_t> arp_listener_frequency();
    uint32_t startup_timeout_ms();
    uint32_t request_threshold();
    std::vector<std::pair<uint32_t, uint32_t>> port_weight_list();

private:
    Json::Value obj;

};

#endif
