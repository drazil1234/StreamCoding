#include "gtest/gtest.h"

#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "Socket.h"

TEST(NoiseTest, ErrorRate)
{
  Socket a("127.0.0.1", 9090) ;
  sleep(1) ;
  Socket b("127.0.0.1", 9091) ;

  std::vector<uint8_t> buf ;
  buf.resize(1) ;

  int cnt = 0 ;
  int maxR = 1000000 ;
  for(int r=0;r<maxR;r++)
  {
    uint8_t num = (uint8_t)(rand()%(1<<8)) ;
    buf[0] = num ;
    ASSERT_EQ(true, a.Send(buf)) ;
    ASSERT_EQ(true, b.Recv(1,buf)) ;
    if(r%100000==0) printf("round = %d\n", r) ;
    if(buf[0]!=num) cnt++ ;
  }

  a.Close() ;
  b.Close() ;

  ASSERT_LE(std::fabs(((double)cnt)/maxR-(1.0-std::pow(0.99999, 8))), 0.0005) ;
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv) ;
  return RUN_ALL_TESTS() ;
}

