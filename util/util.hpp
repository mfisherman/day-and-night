#ifndef __UTIL_HPP__
#define __UTIL_HPP__

#include <string>
#include <sstream>
#include <iomanip>

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif

class CUtil
{
public:
    static std::string parse_mac(const u_char (&mac_address)[6] );
    static std::string parse_ipv4(const u_char (&ipv4)[4] );
    static uint32_t get_current_hour();
};

#endif
