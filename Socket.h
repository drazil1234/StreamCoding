#pragma once

#include <string>
#include <vector>
#include <cstdint>

class Socket
{
  public:
    bool Connect(std::string ip, int port) ;
    bool Listen(int port) ;
    Socket *Accept() ;
    ~Socket() ;
    bool Send(std::vector<uint8_t> buf) ; //send all or fail
    bool Recv(uint64_t n, std::vector<uint8_t> &buf) ;
    void Close() ;
} ;

