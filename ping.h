//
// Created by manout on 18-5-24.
//

#ifndef ICMP_PING_H
#define ICMP_PING_H

#include <cstdio>
#include <memory.h>

#include <sys/socket.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>

#include <chrono>
#include "IP_Address.h"

#define BUFFERSIZE      64

void ping(in_addr*);

class MYPing
{
public :
    MYPing();
    MYPing(std::string ip, int timeout);
    ~MYPing();
    void reset_IP(std::string ip);
    void reset_IP(IP_Address address);
    IP_Address get_address();
    int get_timeout();
    void set_timeout(int timeout);
    bool is_really();
    bool ping();
    bool result();
    void print_info();
private :
    using byte = uint8_t ;
    IP_Address ip_address;
    sockaddr_in dest_addr;
    timeval timeout;
    fd_set readset;
    icmphdr send_hdr;
    icmphdr recv_hdr;
    std::chrono::microseconds timecount;
    int sockfd;
    ssize_t recv_count;
    bool ping_result;
    byte sendbuffer[BUFFERSIZE];
    byte recvbuffer[BUFFERSIZE];
};

#endif //ICMP_PING_H
