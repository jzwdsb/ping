//
// Created by manout on 18-5-25.
//

#ifndef ICMP_IP_ADDRESS_H
#define ICMP_IP_ADDRESS_H

#include <string>

class IP_Address
{
public :
    IP_Address() = default;
    IP_Address(const IP_Address&) = default;
    explicit IP_Address(std::string ip);
    explicit IP_Address(int ip);
    void reset_IP(std::string ip);
    std::string to_string();
    int to_num();
    int distance(IP_Address r_hs);
    IP_Address& operator+=(int n);
    IP_Address operator+(int n);
    IP_Address&operator++();
    IP_Address operator++(int);
    IP_Address&operator =(const IP_Address&)= default;
    bool operator ==(const IP_Address& r_hs);
    bool operator !=(const IP_Address& r_hs);
    bool operator <(const IP_Address& r_hs);
    bool operator <=(const IP_Address& r_hs);
    
private:
    using byte = uint8_t;
    void rebuild_string();
    std::string ip_str;
    union {
        int ip_int;
        byte ip_byte[4];
    }ip_num;
};


#endif //ICMP_IP_ADDRESS_H
