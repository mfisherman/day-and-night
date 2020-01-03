#include "config.hpp"


CConfig::CConfig() : CConfig("config.json")
{
}


CConfig::CConfig(std::string file_name)
{
    std::ifstream ifs(file_name);
    Json::Reader reader;

    reader.parse(ifs, this->obj);
}


CConfig::~CConfig()
{
    // empty on purpose
}


std::string CConfig::iface_name()
{
    return obj["iface_name"].asString();
}


std::string CConfig::log_file_name()
{
    return obj["log_file_name"].asString();
}


std::string CConfig::main_log_name()
{
    return obj["main_log_name"].asString();
}


std::string CConfig::sniffer_log_name()
{
    return obj["sniffer_log_name"].asString();
}


std::string CConfig::arp_parser_log_name()
{
    return obj["arp_parser_log_name"].asString();
}


int64_t CConfig::arp_parser_interval_msec()
{
    return obj["arp_parser_interval_msec"].asInt64();
}


uint32_t CConfig::arp_parser_frequency()
{
    return obj["arp_parser_frequency"].asUInt();
}


std::string CConfig::wol_address()
{
    return obj["wol_address"].asString();
}


std::string CConfig::wol_log_name()
{
    return obj["wol_log_name"].asString();
}


std::string CConfig::ip_parser_log_name()
{
    return obj["ip_parser_log_name"].asString();
}


std::string CConfig::arp_listener_log_name()
{
    return obj["arp_listener_log_name"].asString();
}


std::string CConfig::ethernet_parser_log_name()
{
    return obj["ethernet_parser_log_name"].asString();
}


std::string CConfig::tcp_parser_log_name()
{
    return obj["tcp_parser_log_name"].asString();
}


std::string CConfig::log_priority_level()
{
    return obj["log_priority_level"].asString();
}


uint32_t CConfig::tcp_listener_timeout_msec()
{
    return obj["tcp_listener_timeout_msec"].asUInt();
}


std::string CConfig::tcp_listener_log_name()
{
    return obj["tcp_listener_log_name"].asString();
}


std::string CConfig::host_ip()
{
    return obj["host_ip"].asString();
}


uint32_t CConfig::arp_listener_interval_msec()
{
    return obj["arp_listener_interval_msec"].asUInt();
}


std::vector<uint32_t> CConfig::arp_listener_frequency()
{
    std::vector<uint32_t> result;
    Json::Value list = obj["arp_listener_frequency"];
    for (uint32_t i = 0; i < list.size(); i++)
    {
        if( ! list.isValidIndex(i))
        {
            break;
        }

        Json::Value entry = list[i];
        uint32_t value = entry.asUInt();

        result.push_back(value);
    }
    return  result;
}


uint32_t CConfig::startup_timeout_ms()
{
    return obj["startup_timeout_ms"].asUInt();
}


uint32_t CConfig::request_threshold()
{
    return obj["request_threshold"].asUInt();
}


std::vector<std::pair<uint32_t, uint32_t>> CConfig::port_weight_list()
{
    std::vector<std::pair<uint32_t, uint32_t>> result;
    Json::Value list = obj["port_weight_list"];
    for (uint32_t i = 0; i < list.size(); i++)
    {
        if( ! list.isValidIndex(i))
        {
            break;
        }

        std::pair<uint32_t, uint32_t> pair;

        Json::Value entry = list[i];
        pair.first = entry["port"].asUInt();
        pair.second = entry["weight"].asUInt();

        result.push_back(pair);
    }
    return  result;
}
