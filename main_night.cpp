#include "main_night.hpp"


int main(int argc, char** argv)
{
    std::string config_file_name = "config_night.json";
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

    auto tcp_parser = new CTCPParser(config.tcp_parser_log_name());
    auto ip_parser = new CIPParser( config.ip_parser_log_name(), tcp_parser );
    auto arp_parser = new CARPParser( config.arp_parser_log_name() );
    auto ethernet_parser = new CEthernetParser( config.ethernet_parser_log_name(), arp_parser, ip_parser );
    auto sniffer = new CSniffer( config.iface_name(), config.sniffer_log_name(), ethernet_parser);

    std::vector<std::pair<uint32_t, uint32_t>> port_weight_list = config.port_weight_list();
    auto tcp_listener = new CTCPListener( config.tcp_listener_log_name(), config.tcp_listener_timeout_msec(), config.host_ip(), config.request_threshold(), port_weight_list);

    sniffer->register_listener(tcp_listener);

    std::thread* thread_sniffer = nullptr;
    std::thread* thread_tcp = nullptr;

    std::vector<uint32_t> ports;
    for(auto pair : port_weight_list)
    {
        ports.push_back( pair.first );
    }
    if( sniffer->setup(ports) == 0)
    {
        thread_sniffer = sniffer->run_non_blocking();
    }
    else
    {
        logger->log_error(main_log_name, "End programm, cannot setup network");
        return 1;
    }

    thread_tcp = tcp_listener->run_non_blocking();

    logger->log_info(main_log_name, "Joining threads (waiting)");
    thread_sniffer->join();
    thread_tcp->join();

    logger->log_info(main_log_name, "Ended programm");
    return 0;
}
