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

int c0 = 10 ;
int c1 = 2 ;
int maxN = 1024*1024 ;

void Encode()
{
  Socket s("127.0.0.1", 9090) ;
  std::fstream f("correct.txt", std::ios::out | std::ios::trunc) ;
  AESCTRDRBG rng("The seed of the demo program for stream coding.", \
      "23ASDFpo34wiasd 98fa$#WR09asdf AW#RsadfASDF a43wrSDFz 902%$#@aweSDF ") ;

  int maxK = 0 ;
  std::vector<TreeCodeEncoder *> code ;
  std::vector<int64_t> pos ;
  BlueBerryCode bc ;
  std::vector<uint8_t> data ;

  for(int n=1;n<=maxN;n++)
  {
    uint8_t bit = rand()&1 ;
    f << (int)bit ;
    data.push_back(bit) ;
    if(n==1 || n/log2(n)>maxK+1)
    {
      std::string seed ;
      for(int i=0;i<32;i++)
      {
        uint8_t cur = 0 ;
        std::vector<uint8_t> bits = rng.GetBits(8) ;
        for(int j=0;j<8;j++)
          cur = (cur<<1) | bits[j] ;
        seed += (char) cur ;
      }
      code.push_back(new TreeCodeEncoder(seed)) ;
      pos.push_back(n-1) ;
      maxK++ ;
    }

    for(int r=0;r<c0;r++)
    {
      int curK = rng.Next(0, maxK) ;
      uint64_t cw = code[curK]->Encode((pos[curK]>=0)?data[pos[curK]--]:0) ;
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
  }

  f.close() ;
  for(auto ptr: code)
    delete ptr ;
}

void Decode()
{
  Socket s("127.0.0.1", 9091) ;
  std::fstream f("decoded.txt", std::ios::out | std::ios::trunc) ;
  AESCTRDRBG rng("The seed of the demo program for stream coding.", \
      "23ASDFpo34wiasd 98fa$#WR09asdf AW#RsadfASDF a43wrSDFz 902%$#@aweSDF ") ;

  int maxK = 0 ;
  std::vector<TreeCodeDecoder *> code ;
  std::vector<int64_t> pos ;
  BlueBerryCode bc ;
  std::vector<uint8_t> data ;

  for(int n=1;n<=maxN;n++)
  {
    data.push_back(0) ;
    if(n==1 || n/log2(n)>maxK+1)
    {
      std::string seed ;
      for(int i=0;i<32;i++)
      {
        uint8_t cur = 0 ;
        std::vector<uint8_t> bits = rng.GetBits(8) ;
        for(int j=0;j<8;j++)
          cur = (cur<<1) | bits[j] ;
        seed += (char) cur ;
      }
      code.push_back(new TreeCodeDecoder(seed)) ;
      pos.push_back(n-1) ;
      maxK++ ;
    }
    for(int r=0;r<c0;r++)
    {
      std::vector<uint8_t> buf ;
      s.Recv(8, buf) ;
      uint64_t cw = 0 ;
      for(int i=7;i>=0;i--)
        cw = (cw<<8) | buf[i] ;
      cw = bc.Decode(cw) ;
      int curK = rng.Next(0, maxK) ;
      code[curK]->Decode(cw) ;
    }

    for(int i=0;i<maxK;i++)
    {
      int64_t valid = std::min(pos[i]+1, (int64_t)floor(c1*log2(n))) ;
      const std::vector<uint64_t> decoded = code[i]->GetDecoded() ;
      for(int64_t j=0;j<valid;j++)
        data[pos[i]-j] = decoded[j] ;
    }
  }

  for(auto bit: data)
    f << (int)bit ;

  f.close() ;
  for(auto ptr: code)
    delete ptr ;
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

  int cnt = 0 ;
  for(int n=0;n<maxN;n++)
  {
    char cc, cd ;
    c >> cc ;
    d >> cd ;
    if(cc!=cd) break ;
    else cnt++ ;
  }
  printf("We decoded %d prefix correctly.\n", cnt) ;

  c.close() ;
  d.close() ;

  return 0 ;
}
