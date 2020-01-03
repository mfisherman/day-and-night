#ifndef __TCP_PARSER_HPP__
#define __TCP_PARSER_HPP__

#include <stdlib.h>
#include <string>
#include <vector>

#include "../listener/sniffer_listener.hpp"
#include "../../util/logger.hpp"

#include <netinet/in.h>

/* TCP header */
typedef uint32_t tcp_seq;

#define TCP_PARSER_DEFAULT_LOG_NAME "arp_sniffer"

typedef struct tcp_header {
        u_short th_sport;               /* source port */
        u_short th_dport;               /* destination port */
        tcp_seq th_seq;                 /* sequence number */
        tcp_seq th_ack;                 /* acknowledgement number */
        u_char  th_offx2;               /* data offset, rsvd */
#define TH_OFF(th)      (((th)->th_offx2 & 0xf0) >> 4)
        u_char  th_flags;
        #define TH_FIN  0x01
        #define TH_SYN  0x02
        #define TH_RST  0x04
        #define TH_PUSH 0x08
        #define TH_ACK  0x10
        #define TH_URG  0x20
        #define TH_ECE  0x40
        #define TH_CWR  0x80
        #define TH_FLAGS        (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)
        u_short th_win;                 /* window */
        u_short th_sum;                 /* checksum */
        u_short th_urp;                 /* urgent pointer */
} tcp_header_t;


class CTCPParser
{
public:
    CTCPParser();
    CTCPParser(std::string log_name);
    ~CTCPParser();

    int32_t parse(const unsigned char* p_tcp_header);
    void register_listener(CSnifferListener* listener);

private:
    int32_t log(const tcp_header* tcp_header);
    int32_t inform_listeners(const tcp_header_t* tcp_header );

    std::vector<CSnifferListener*> listeners;
    std::string log_name;
    CLogger* logger;
};


#endif
