#include "DRNG.h"
#include "Socket.h"
#include <cstdint>
#include <vector>
#include <cstdio>
#include <unistd.h>

#define ERROR_RATE 100000

int main(void)
{
  Socket in(9090) ;
  Socket out(9091) ;
  AESCTRDRBG rng("This is an init string for the destroyer!", "Q@#R)FADS12va#Radsf ascqw3rAW$q wv0sdR") ;

  for(;;)
  {
    std::vector<uint8_t> buf ;
    if(!in.Recv(1, buf)) break ;
    for(int i=0;i<8;i++)
      if(rng.Next(0, ERROR_RATE-1)==0)
      {
        printf("mess up a bit\n") ;
        buf[0] ^= (1<<i) ;
      }
    if(!out.Send(buf)) break ;
  }

  return 0 ;
}

