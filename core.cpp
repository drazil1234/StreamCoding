#include "DRNG.h"
#include "Socket.h"
#include "TreeCode.h"
#include "BlueBerryCode.h"

#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <thread>
#include <cstdint>
#include <fstream>
#include <vector>
#include <algorithm>

int c0 = 20 ;
int c1 = 2 ;
int maxN = 4*1024 ;
int EncodeR = 0, DecodeR = 0 ;

void Encode()
{
  Socket s("127.0.0.1", 9090) ;
  std::fstream f("correct.txt", std::ios::out | std::ios::trunc) ;
  AESCTRDRBG rng("The seed of the demo program for stream coding.", \
      "23ASDFpo34wiasd 98fa$#WR09asdf AW#RsadfASDF a43wrSDFz 902%$#@aweSDF ") ;

  TreeCodeEncoder *code ;
  BlueBerryCode bc ;
  std::vector<uint8_t> data ;
  std::string seed ;
  for(int i=0;i<32;i++)
  {
    uint8_t cur = 0 ;
    std::vector<uint8_t> bits = rng.GetBits(8) ;
    for(int j=0;j<8;j++)
      cur = (cur<<1) | bits[j] ;
    seed += (char) cur ;
  }
  code = new TreeCodeEncoder(seed) ;

  for(int n=1;n<=maxN;n++)
  {
    while(DecodeR+2<n) usleep(200) ;
    EncodeR = n ;
    uint8_t bit = rand()&1 ;
    f << (int)bit ;
    data.push_back(bit) ;

    uint64_t cw = code->Encode(bit) ;
    cw = bc.Encode(cw) ;
    std::vector<uint8_t> vec ;
    vec.resize(8) ;
    for(int i=0;i<8;i++)
    {
      vec[i] = cw&((1ULL<<8)-1) ;
      cw >>= 8 ;
    }
    s.Send(vec) ;
  }

  f.close() ;
  delete code ;
}

void Decode()
{
  Socket s("127.0.0.1", 9091) ;
  std::fstream f("decoded.txt", std::ios::out | std::ios::trunc) ;
  AESCTRDRBG rng("The seed of the demo program for stream coding.", \
      "23ASDFpo34wiasd 98fa$#WR09asdf AW#RsadfASDF a43wrSDFz 902%$#@aweSDF ") ;

  TreeCodeDecoder *code ;
  BlueBerryCode bc ;
  std::string seed ;
  for(int i=0;i<32;i++)
  {
    uint8_t cur = 0 ;
    std::vector<uint8_t> bits = rng.GetBits(8) ;
    for(int j=0;j<8;j++)
      cur = (cur<<1) | bits[j] ;
    seed += (char) cur ;
  }
  code = new TreeCodeDecoder(seed) ;

  for(int n=1;n<=maxN;n++)
  {
    while(EncodeR+2<n) usleep(200) ;
    DecodeR = n ;

    std::vector<uint8_t> buf ;
    s.Recv(8, buf) ;
    uint64_t cw = 0 ;
    for(int i=7;i>=0;i--)
      cw = (cw<<8) | buf[i] ;
    cw = bc.Decode(cw) ;
    const std::vector<uint64_t> &data = code->Decode(cw) ;

    for(int i=0;i<n;i++)
      f << (int)data[i] ;

    if(n%1024==0)
      printf("End of Decode round %d\n",n) ;
  }

  f.close() ;
  delete code ;
}

int main(void)
{
  std::thread th1(Encode) ;
  sleep(1) ;
  std::thread th2(Decode) ;

  th1.join() ;
  th2.join() ;

  //statistic here
  std::fstream c("correct.txt", std::ios::in) ;
  std::fstream d("decoded.txt", std::ios::in) ;
  std::fstream rp("report.txt", std::ios::out | std::ios::trunc) ;

  int cnt = 0 ;
  std::vector<char> ans ;
  for(int n=0;n<maxN;n++)
  {
    char tmp ;
    c >> tmp ;
    ans.push_back(tmp) ;
  }
  for(int n=0;n<maxN;n++)
  {
    int mycnt = 0 ;
    bool good = true ;
    for(int i=0;i<=n;i++)
    {
      char tmp ;
      d >> tmp ;
      if(good && ans[i]==tmp)
        mycnt++ ;
      else good = false ;
    }
    rp << n+1 << ", " << mycnt << std::endl ;
    cnt = mycnt ;
  }
  printf("We decoded %d prefix correctly.\n", cnt) ;

  c.close() ;
  d.close() ;
  rp.close() ;

  return 0 ;
}
