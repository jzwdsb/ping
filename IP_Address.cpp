//
// Created by manout on 18-5-25.
//

#include "IP_Address.h"
#include <algorithm>
#include <sstream>
#include <memory.h>

IP_Address::IP_Address(std::string ip)
{
    reset_IP(ip);
}

IP_Address::IP_Address(int ip)
{
    this->ip_num.ip_int;
    rebuild_string();
}


void IP_Address::reset_IP(std::string ip)
{
    this->ip_num.ip_int = 0;
    std::for_each(ip.begin(), ip.end(), [](char &ch)
    { if (ch == '.') ch = ' '; });
    std::stringstream ss(ip);
    /** little ending, 不这样写读出来的是错误值*/
    int seg;
    ss >> seg;
    this->ip_num.ip_byte[3] = static_cast<byte>(seg);
    ss >> seg;
    this->ip_num.ip_byte[2] = static_cast<byte>(seg);
    ss >> seg;
    this->ip_num.ip_byte[1] = static_cast<byte>(seg);
    ss >> seg;
    this->ip_num.ip_byte[0] = static_cast<byte>(seg);
    std::for_each(ip.begin(), ip.end(), [](char &ch)
    { if (ch == ' ') ch = '.'; });
    this->ip_str = std::move(ip);
}

std::string IP_Address::to_string()
{
    return this->ip_str;
}

int IP_Address::to_num()
{
    return this->ip_num.ip_int;
}

IP_Address &IP_Address::operator+=(int n)
{
    this->ip_num.ip_int += n;
    this->rebuild_string();
    return *this;
}

IP_Address IP_Address::operator+(int n)
{
    IP_Address ret(*this);
    
    return ret += n;
}

IP_Address &IP_Address::operator++()
{
    return this->operator+=(1);
}

IP_Address IP_Address::operator++(int)
{
    IP_Address ret(*this);
    this->operator+=(1);
    return ret;
}


bool IP_Address::operator==(const IP_Address& r_hs)
{
    return this->ip_num.ip_int == r_hs.ip_num.ip_int;
}

bool IP_Address::operator!=(const IP_Address& r_hs)
{
    return not this->operator==(r_hs);
}



void IP_Address::rebuild_string()
{
    this->ip_str.clear();
    for (int i = 3; i >= 0; --i)
    {
        this->ip_str += std::to_string(this->ip_num.ip_byte[i]);
        this->ip_str.push_back('.');
    }
    this->ip_str.erase(this->ip_str.end() - 1);
}

int IP_Address::distance(IP_Address r_hs)
{
    return abs(this->ip_num.ip_int - r_hs.ip_num.ip_int);
}

bool IP_Address::operator<(const IP_Address &r_hs)
{
    return this->ip_num.ip_int < r_hs.ip_num.ip_int;
}

bool IP_Address::operator<=(const IP_Address &r_hs)
{
    return this->operator<(r_hs) or this->operator==(r_hs);
}


