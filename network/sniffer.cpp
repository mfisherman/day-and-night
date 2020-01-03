#include "sniffer.hpp"


CSniffer::CSniffer() : CSniffer(SNIFFER_DEFAULT_DEVICE, SNIFFER_DEFAULT_LOG_NAME, new CEthernetParser())
{
    // empty on purpose
}


CSniffer::CSniffer(std::string iface_name, std::string log_name, CEthernetParser* ether_sniffer)
{
    this->iface_name = iface_name;
    this->log_name = log_name;
    this->logger = CLogger::get_instance();
    this->descr = nullptr;
    this->ether_sniffer = ether_sniffer;
}


CSniffer::~CSniffer()
{
    // empty on purpose
}


void CSniffer::register_listener(CSnifferListener* listener)
{
    listeners.push_back(listener);
    ether_sniffer->register_listener(listener);
}


int32_t CSniffer::setup(std::vector<uint32_t> ports)
{
    std::ostringstream os;
    for (auto it = ports.begin() ; it != ports.end(); ++it)
    {
        if( ports.begin() == it)
        {
            os << "port "  << (*it);
        }
        else
        {
            os << " or port " << (*it);
        }
    }

    return this->setup(os.str());
}


int32_t CSniffer::setup(std::string filter_text)
{
    char *errbuf = static_cast<char*> ( calloc(PCAP_ERRBUF_SIZE, sizeof(char)) ); // error buffer
    memset(errbuf, 0, PCAP_ERRBUF_SIZE);

    descr = pcap_create(iface_name.c_str(), errbuf);
    if(descr == nullptr)
    {
        logger->log_error( log_name,  "Cannot open device: "+ iface_name);
        return 1;
    }

    if( pcap_activate(descr) < 0 )
    {
        logger->log_error( log_name,  "Error activating device: "+ iface_name);
        return 1;
    }

    if (pcap_datalink(descr) != DLT_EN10MB)
    {
        logger->log_error( log_name,  "Device "+ iface_name+ " does not provide Ethernet headers - not supported");
        return 1;
    }

    bpf_u_int32 netaddr = 0; // to store network address and network mask
    bpf_u_int32 mask = 0;
    if( pcap_lookupnet(iface_name.c_str(), &netaddr, &mask, errbuf) == -1) // lookup info from the capture device
    {
        logger->log_error(log_name, "Cannot get netaddr and mask from device: " + iface_name);
        return 1;
    }

    struct bpf_program filter; // place to store the bpf filter program;
    if(pcap_compile(descr, &filter, filter_text.c_str(), 0, netaddr) == -1) // compile the filter expression into a bpf filter program
    {
        logger->log_error(log_name, "Cannot compile the filter");
        return 1;
    }

    if( pcap_setfilter(descr, &filter) == -1) // load the filter program into the packet capture device
    {
        logger->log_error(log_name, "Cannot set the filter");
        return 1;
    }

    logger->log_info(log_name, "Setup successfull");

    return 0;
}


std::thread* CSniffer::run_non_blocking()
{
    auto thread = new std::thread(&CSniffer::run_blocking, this);

    return thread;
}


void CSniffer::run_blocking()
{

    const unsigned char *packet = nullptr; // received raw data
    struct pcap_pkthdr pkthdr; // packet header (timestamp, size, ...)

    logger->log_info(log_name, "Start listening for packets");
    while(true)
    {
        packet = pcap_next(descr, &pkthdr); // get one packet
        if(packet == nullptr)
        {
            logger->log_error(log_name, "pcap_next returned a nullptr");
        }
        else
        {
            log(&pkthdr);

            inform_listeners(&pkthdr);
            ether_sniffer->parse(packet);
            // packet must not be freed by the caller
        }

    }
}


int32_t CSniffer::log( struct pcap_pkthdr* pkthdr )
{
    std::string log_string = "Packet len: "+ std::to_string(pkthdr->len) + ", Caplen: " + std::to_string(pkthdr->caplen);
    logger->log_debug(log_name, log_string );

    return 0;
}


int32_t CSniffer::inform_listeners( struct pcap_pkthdr* pkthdr)
{
    for(auto it = listeners.begin(); it < listeners.end(); it++)
    {
        (*it)->packet(0, pkthdr->caplen, pkthdr->len);
    }

    return 0;
}
