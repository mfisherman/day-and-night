#include "wol.hpp"


CWOL::CWOL() : CWOL("", WOL_DEFAULT_LOG_NAME, 60*1000)
{

}


CWOL::CWOL(std::string wol_address, std::string log_name, uint32_t timeout)
{
    this->wol_address = wol_address;
    this->log_name = log_name;
    this->logger = CLogger::get_instance();
    this->timeout = timeout;
    this->last_sent = 0;
}


CWOL::~CWOL()
{
    // empty on purpose
}


unsigned int CWOL::get_hex_from_string(const std::string& s)
{
    unsigned int hex = 0;

    for (size_t i = 0; i < s.length(); ++i) {
        hex <<= 4;
        if (isdigit(s[i]) != 0) {
            hex |= s[i] - '0';
        }
        else if (s[i] >= 'a' && s[i] <= 'f') {
            hex |= s[i] - 'a' + 10;
        }
        else if (s[i] >= 'A' && s[i] <= 'F') {
            hex |= s[i] - 'A' + 10;
        }
        else {
            logger->log_error(log_name, "Failed to parse hexadecimal " + s);
            return 0;
        }
    }
    return hex;
}


std::string CWOL::get_ether(const std::string& hardware_addr)
{
    std::string ether_addr;

    for (size_t i = 0; i < hardware_addr.length(); )
    {
        // Parse two characters at a time.
        unsigned int hex = get_hex_from_string(hardware_addr.substr(i, 2));
        i += 2;

        ether_addr += static_cast<char>(hex & 0xFF);

        // We might get a colon here, but it is not required.
        if (hardware_addr[i] == ':')
        {
            ++i;
        }
    }

    if (ether_addr.length() != 6)
    {
        logger->log_error(log_name, hardware_addr + " not a valid ether address");
    }

    return ether_addr;
}


void CWOL::send_wol()
{
    send_wol(wol_address);
}


void CWOL::send_wol(const std::string& hardware_addr)
{
    send_wol(hardware_addr, 60000, 0xFFFFFFFF);
}


void CWOL::send_wol(const std::string& hardware_addr, uint32_t port, uint64_t bcast)
{
    std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds > (std::chrono::system_clock::now().time_since_epoch() );
    int64_t curr_msec = ms.count();

    if( labs( curr_msec -  last_sent ) < timeout  )
    {
        logger->log_info(log_name, "Not sending WOL, due to timeout (" + std::to_string(timeout) +" msec) not reached yet. Diff: " + std::to_string(labs( curr_msec -  last_sent ) ) + " msec");
        return;
    }

    logger->log_info(log_name, "No timeout: curr_msec: "+std::to_string(curr_msec)+" , last_sent: "+std::to_string(last_sent)+" , timeout: "+std::to_string(timeout ) );

    last_sent = curr_msec;

    logger->log_info(log_name, "Sending WOL to: " +hardware_addr);

    // Fetch the hardware address.
    const std::string ether_addr{get_ether(hardware_addr)};

    int descriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // Build the message to send.
    // (6 * 0XFF followed by 16 * destination address.)
    std::string message(6, 0xFF);
    for (size_t i = 0; i < 16; ++i)
    {
        message += ether_addr;
    }

    // Set socket options.
    const int optval{1};
    if (setsockopt(descriptor, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)) < 0)
    {
        logger->log_error(log_name, "Failed to set socket options");
    }

    // Set up address
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = bcast;
    addr.sin_port = htons(port);

    // Send the packet out.
    if (sendto(descriptor, message.c_str(), message.length(), 0, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
    {
        logger->log_error(log_name, "Failed to send packet");
    }

    if( close(descriptor) != 0)
    {
        logger->log_error(log_name, "Failed to close the socket");
    }

    logger->log_info(log_name, "Sent WOL to: " +hardware_addr);
}
