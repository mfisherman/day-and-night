#ifndef __SNIFFER_HPP__
#define __SNIFFER_HPP__

#include <pcap.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <thread>

#include "../util/logger.hpp"
#include "listener/sniffer_listener.hpp"
#include "parser/ethernet_parser.hpp"

#define MAX_BYTES_TO_CAPTURE 262144
#define SNIFFER_DEFAULT_DEVICE "enp4s0f1"
#define SNIFFER_DEFAULT_LOG_NAME "sniffer"

class CSniffer
{
public:
    CSniffer();
    CSniffer(std::string iface_name, std::string log_name, CEthernetParser* ether_sniffer);
    ~CSniffer();
    void register_listener(CSnifferListener* listener);
    int32_t setup(std::vector<uint32_t> ports);
    int32_t setup(std::string filter_text);
    std::thread* run_non_blocking();
    [[ noreturn ]] void run_blocking();

private:
    int32_t log( struct pcap_pkthdr* pkthdr );
    int32_t inform_listeners( struct pcap_pkthdr* pkthdr);

    CLogger* logger;
    std::string log_name;
    pcap_t* descr; // network interface handler
    std::vector<CSnifferListener*> listeners;
    CEthernetParser* ether_sniffer;
    std::string iface_name;
};



#endif
