#ifndef __IP_SNIFFER_HPP__
#define __IP_SNIFFER_HPP__

#include <string>
#include <stdlib.h>
#include <vector>
#include <netinet/in.h>

#include "../../util/logger.hpp"
#include "../../util/util.hpp"
#include "../listener/sniffer_listener.hpp"
#include "tcp_parser.hpp"

#define IP_PARSER_DEFAULT_LOG_NAME "ip_parser"
#define IP_HL(ip)               (((ip)->ip_vhl) & 0x0f)

typedef struct ip_header {
        u_char  ip_vhl;                 /* version << 4 | header length >> 2 */
        u_char  ip_tos;                 /* type of service */
        u_short ip_len;                 /* total length */
        u_short ip_id;                  /* identification */
        u_short ip_off;                 /* fragment offset field */
        #define IP_RF 0x8000            /* reserved fragment flag */
        #define IP_DF 0x4000            /* dont fragment flag */
        #define IP_MF 0x2000            /* more fragments flag */
        #define IP_OFFMASK 0x1fff       /* mask for fragmenting bits */
        u_char  ip_ttl;                 /* time to live */
        u_char  ip_p;                   /* protocol */
        u_short ip_sum;                 /* checksum */
        u_char ip_src[4];
        u_char ip_dst[4];
} ip_header_t;


class CIPParser
{
public:
    CIPParser();
    CIPParser(std::string log_name, CTCPParser* tcp_parser);
    ~CIPParser();

    int32_t parse(const unsigned char* p_ip_header);
    void register_listener(CSnifferListener* listener);

private:
    int32_t log(const ip_header_t* ip_header);
    int32_t inform_listeners(const ip_header_t* ip_header );

    bool is_tcp(const ip_header_t* ip_header );
    bool is_udp(const ip_header_t* ip_header);
    bool is_icmp(const ip_header_t* ip_header);
    bool is_ip(const ip_header_t* ip_header);
    std::string get_sender_ipv4(const ip_header_t* ip_header);
    std::string get_receiver_ipv4(const ip_header_t* ip_header);

    CLogger* logger;
    std::vector<CSnifferListener*> listeners;
    std::string log_name;
    CTCPParser* tcp_parser;
};

#endif
