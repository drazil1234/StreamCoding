#include "Socket.h"

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

Socket::Socket(std::string ip, int port)
{
  this->_fd = socket(AF_INET, SOCK_STREAM, 0) ;
  struct sockaddr_in clientService ;
  clientService.sin_family = AF_INET;
  clientService.sin_addr.s_addr = inet_addr(ip.c_str());
  clientService.sin_port = htons(port); 
  connect(this->_fd, (struct sockaddr *) & clientService, sizeof (clientService)) ;
}

Socket::Socket(int port)
{
  int s = socket(AF_INET, SOCK_STREAM, 0) ;
  struct sockaddr_in service ;
  memset(&service,0,sizeof(service)) ;
  service.sin_family = AF_INET ;
  service.sin_addr.s_addr = INADDR_ANY ;
  service.sin_port = htons(port) ;
  bind(s, (struct sockaddr *) &service, sizeof (service)) ;
  listen(s, 3) ;

  socklen_t addr_len ;
  struct sockaddr addr ;

  addr_len = sizeof(struct sockaddr) ;
  this->_fd = accept(s,&addr,&addr_len) ;

  shutdown(s, SHUT_RDWR) ;
  close(s) ;
}

Socket::~Socket()
{
  this->Close() ;
}

void Socket::Close()
{
  shutdown(this->_fd, SHUT_RDWR) ;
  close(this->_fd) ;
}

bool Socket::Send(std::vector<uint8_t> buf)
{
  uint64_t sent = 0 ;
  while(sent<buf.size())
  {
    uint64_t curSent = send(this->_fd, buf.data()+sent, buf.size()-sent, 0) ;
    if(curSent<=0) return false ;
    sent += curSent ;
  }
  return true ;
}

bool Socket::Recv(uint64_t n, std::vector<uint8_t> &buf)
{
  buf.resize(n) ;
  uint64_t read = 0 ;
  while(read<n)
  {
    uint64_t curRead = recv(this->_fd, buf.data()+read, n-read, 0) ;
    if(curRead<=0) return false ;
    read += curRead ;
  }
  return true ;
}

