//
// Created by manout on 18-5-28.
//


#include <thread>
#include <vector>
#include <mutex>

#include "ping.h"
#include "IP_Address.h"

extern std::vector<std::pair<std::string, int>> usable_ip_list;
extern std::vector<std::string> unusable_ip_list;

std::mutex usable_lock;
std::mutex unusable_lock;

void ping_one_host(std::string);
void ping_segment(std::string, std::string);

void parallel_ping(char** argv)
{
    IP_Address start_ip(argv[1]);
    IP_Address end_ip(argv[2]);
    std::vector<std::thread> pool;
    while (start_ip <= end_ip)
    {
        pool.emplace_back(ping_one_host, start_ip.to_string());
        ++start_ip;
    }
    for (auto it = pool.begin(); it not_eq pool.end(); ++it)
    {
        it->join();
    }
}

void ping_one_host(std::string ip)
{
    MYPing ping(ip, 1000);
    if (ping.is_really())
    {
        printf("pinging %s\n", ip.c_str());
        ping.ping();
        if (ping.result())
        {
            usable_lock.lock();
            usable_ip_list.emplace_back(ip, ping.get_timeout());
            usable_lock.unlock();
        } else
        {
            unusable_lock.lock();
            unusable_ip_list.push_back(ip);
            unusable_lock.unlock();
        }
    } else
    {
        printf("socket error\n\n");
    }
}

void ping_segment(std::string start_ip, std::string end_ip)
{
    for (IP_Address ip(start_ip); ip not_eq IP_Address(end_ip); ++ip)
    {
        MYPing ping(ip.to_string(), 3000);
        if (ping.is_really())
        {
            ping.reset_IP(ip);
            ping.ping();
            printf("%d thread pinging %s\n", std::this_thread::get_id(),ip.to_string().c_str());
            if (ping.result())
            {
                usable_lock.lock();
                usable_ip_list.emplace_back(ping.get_address().to_string(), ping.get_timeout());
                usable_lock.unlock();
            }else
            {
                unusable_lock.lock();
                unusable_ip_list.push_back(ping.get_address().to_string());
                unusable_lock.try_lock();
            }
        } else
        {
            printf("socket error\n\n");
        }
    }
}