#include "main_day.hpp"


int main(int argc, char** argv)
{
    std::string config_file_name = "config_day.json";
    if( argc == 2)
    {
        config_file_name = argv[1];
    }

    CConfig config(config_file_name);
    CLogger::set_file_name(config.log_file_name());
    CLogger::set_priority(config.log_priority_level());

    std::string main_log_name = config.main_log_name();
    CLogger* logger = CLogger::get_instance();
    logger->log_info(main_log_name, "started program");

    uint32_t startup_timeout_ms = config.startup_timeout_ms();
    if( startup_timeout_ms > 0)
    {
        logger->log_info(main_log_name, "sleeping for " + std::to_string(startup_timeout_ms)+" msec due to startup timeout");
        usleep( static_cast<uint32_t> (startup_timeout_ms * 1000));
    }

    auto wol = new CWOL(config.wol_address(), config.wol_log_name(), 120*1000 );

    auto tcp_parser = new CTCPParser(config.tcp_parser_log_name());
    auto ip_parser = new CIPParser( config.ip_parser_log_name(), tcp_parser );
    auto arp_parser = new CARPParser( config.arp_parser_log_name() );
    auto ethernet_parser = new CEthernetParser( config.ethernet_parser_log_name(), arp_parser, ip_parser );
    auto sniffer = new CSniffer( config.iface_name(), config.sniffer_log_name(), ethernet_parser);

    auto arp_listener = new CARPListener(wol, config.arp_listener_log_name(), config.arp_listener_interval_msec(), config.arp_listener_frequency(), config.host_ip());

    sniffer->register_listener(arp_listener);
    if( sniffer->setup("arp") == 0)
    {
        sniffer->run_blocking();
    }

    logger->log_info(main_log_name, "ended programm");
    return 0;
}



