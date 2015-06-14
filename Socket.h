#pragma once

#include <string>
#include <vector>
#include <cstdint>

class Socket
{
  public :
    Socket(std::string ip, int port) ;
    Socket(int port) ;
    ~Socket() ;
    bool Send(std::vector<uint8_t> buf) ; //send all or fail
    bool Recv(uint64_t n, std::vector<uint8_t> &buf) ;
    void Close() ;
  private :
    int _fd ;
} ;

