#ifndef __WOL_HPP__
#define __WOL_HPP__

// see: https://github.com/gumb0/wol.cpp/blob/master/wol.cpp

#include <arpa/inet.h>
#include <errno.h>
#include <getopt.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <chrono>

#include "../util/logger.hpp"

#define WOL_DEFAULT_LOG_NAME "wol"

class CWOL
{
public:
    CWOL();
    CWOL(std::string wol_address, std::string log_name, uint32_t timeout);
    ~CWOL();

    void send_wol();
    void send_wol(const std::string& hardware_addr);

private:
    unsigned int get_hex_from_string(const std::string& s);
    std::string get_ether(const std::string& hardware_addr);

    void send_wol(const std::string& hardware_addr, uint32_t port, uint64_t bcast);

    std::string wol_address;
    std::string log_name;
    int64_t timeout;
    int64_t last_sent;

    CLogger* logger;

};

#endif
