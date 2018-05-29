#include <iostream>
#include <vector>
#include <algorithm>

#include "ping.h"
#include "IP_Address.h"

void ping_one_host(std::string ip);

std::vector<std::pair<std::string, int>> usable_ip_list;
std::vector<std::string> unusable_ip_list;
void parallel_ping(char** argv);
void print_info();
int main(int argc, char* argv[])
{
    switch(argc)
    {
        case 2:
            ping_one_host(argv[1]);
            break;
        case 3:
            parallel_ping(argv);
            print_info();
            break;
        default:
            printf("usage: scan_host ip|start_ip end_ip\n");
    }
    
   return 0;
}

void print_info()
{
    std::sort(usable_ip_list.begin(), usable_ip_list.end());
    std::sort(unusable_ip_list.begin(), unusable_ip_list.end());
    printf("usable host, count %lu :\n", usable_ip_list.size());
    for (auto ip : usable_ip_list)
    {
        printf("host ip: %s    ping timeout: %dms\n", ip.first.c_str(), ip.second);
    }
    printf("\nunusable host, count %lu\n", unusable_ip_list.size());
    for (auto ip : unusable_ip_list)
    {
        printf("host ip: %s\n", ip.c_str());
    }
}