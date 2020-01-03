#include "util.hpp"


std::string CUtil::parse_mac(const u_char (&mac_address)[6])
{
    std::stringstream ss;
    for(int i = 0; i < 6; i++)
    {
        ss << std::hex << static_cast<uint32_t>(mac_address[i]);
        if(i != 5)
        {
            ss << ":";
        }
    }
    return ss.str();
}


std::string CUtil::parse_ipv4(const u_char (&ipv4)[4])
{
    std::stringstream ss;

    for(int i = 0; i < 4; i++)
    {
        ss << static_cast<uint32_t>(ipv4[i]);
        if(i != 3)
        {
            ss << ".";
        }
    }
    return ss.str();
}


uint32_t CUtil::get_current_hour()
{
    std::time_t now = std::time(nullptr);
    std::tm calendar_time = *std::localtime( std::addressof(now) ) ;

    return (calendar_time.tm_hour % 24);
}
