//
// Created by manout on 18-5-24.
//

#include "ping.h"


MYPing::MYPing()
{
    this->timeout.tv_sec = 0;
    this->timeout.tv_usec = 1000000;
    this->dest_addr.sin_family = AF_INET;
    this->sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
    if (this->sockfd < 0)
    {
        perror("socket");
        return ;
    }
    bzero(this->sendbuffer, sizeof(this->sendbuffer));
    bzero(this->recvbuffer, sizeof(this->recvbuffer));
    bzero(&this->send_hdr, sizeof(this->send_hdr));
    bzero(&this->recv_hdr, sizeof(this->recv_hdr));
    bzero(&this->readset, sizeof(this->readset));
    FD_SET(this->sockfd, &this->readset);
    this->send_hdr.type = ICMP_ECHO;
    memcpy(this->sendbuffer, &this->send_hdr, sizeof(this->send_hdr));
}

MYPing::MYPing(const std::string ip, int timeout): ip_address(ip), recv_count(0)
{
    this->timeout.tv_sec = 0;
    this->timeout.tv_usec = timeout * 1000;
    this->dest_addr.sin_family = AF_INET;
    inet_pton(AF_INET, this->ip_address.to_string().c_str(), &this->dest_addr.sin_addr);
    this->sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
    if (sockfd < 0)
    {
        perror("socket error");
        return ;
    }
    bzero(this->sendbuffer, sizeof(this->sendbuffer));
    bzero(this->recvbuffer, sizeof(this->recvbuffer));
    bzero(&this->send_hdr, sizeof(this->send_hdr));
    bzero(&this->recv_hdr, sizeof(this->recv_hdr));
    bzero(&readset, sizeof(readset));
    FD_SET(this->sockfd, &this->readset);
    this->send_hdr.type = ICMP_ECHO;
    memcpy(this->sendbuffer, &this->send_hdr, sizeof(this->send_hdr));
}

MYPing::~MYPing()
{
    close(this->sockfd);
}

void MYPing::reset_IP(IP_Address address)
{
    this->reset_IP(address.to_string());
}


void MYPing::reset_IP(std::string ip)
{
    this->ip_address.reset_IP(ip);
    inet_pton(AF_INET, this->ip_address.to_string().c_str(), &this->dest_addr.sin_addr);
}

void MYPing::set_timeout(int timeout)
{
    this->timeout.tv_usec = timeout * 1000;
}

IP_Address MYPing::get_address()
{
    return this->ip_address;
}

int MYPing::get_timeout()
{
    return static_cast<int>(this->timecount.count());
}

bool MYPing::is_really()
{
    return this->sockfd > 0;
}


bool MYPing::ping()
{
    if (this->sockfd > 0)
    {
        ssize_t rc;
        socklen_t socklen = sizeof(this->dest_addr);
        sendto(this->sockfd, this->sendbuffer, sizeof(this->send_hdr), 0, (sockaddr *) &this->dest_addr, socklen);
        auto start = std::chrono::system_clock::now();
        rc = select(this->sockfd + 1, &this->readset, nullptr, nullptr, &this->timeout);
        if (rc <= 0)
        {
            this->ping_result = false;
            return false;
        }
        this->recv_count = recvfrom(this->sockfd, this->recvbuffer, BUFFERSIZE, 0, nullptr, &socklen);
        if (this->recv_count <= 0)
        {
            perror("recvfrom :");
            this->ping_result = false;
            return false;
        }
        auto end = std::chrono::system_clock::now();
        this->timecount = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        memcpy(this->recvbuffer, &recv_hdr, sizeof(recv_hdr));
        this->ping_result = this->recv_hdr.type == ICMP_ECHOREPLY;
        return this->ping_result;
    }
    return false;
}

void MYPing::print_info()
{
    if (this->ping_result)
    {
        printf("%zi bytes from %s: icmp_seq=0x%x, time=%li ms\n",
            this->recv_count, this->ip_address.to_string().c_str()
               , this->recv_hdr.un.echo.sequence, this->timecount.count());
    }else
    {
        printf("Got no Reply\n");
    }
}

bool MYPing::result()
{
    return this->ping_result;
}


