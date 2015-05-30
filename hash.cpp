#include "hash.h"

#include <cstdint>
#include <vector>

#define RotR(A, n) (((A)>>(n)) | ((A)<<(32-(n))))
#define ShR(A, n) ((A)>>(n))
#define Ch(X, Y, Z) (((X)&(Y)) ^ ((~(X))&(Z)))
#define Maj(X, Y, Z) (((X)&(Y)) ^ ((X)&(Z)) ^ ((Y)&(Z)))
#define Sigma0(X) (RotR((X),2) ^ RotR(X, 13) ^ RotR(X,22))
#define Sigma1(X) (RotR((X),6) ^ RotR(X, 11) ^ RotR(X,25))
#define sigma0(X) (RotR((X),7) ^ RotR(X, 18) ^ ShR(X,3))
#define sigma1(X) (RotR((X),17) ^ RotR(X, 19) ^ ShR(X,10))

const uint32_t K[64] = {
  0x428a2f98U, 0x71374491U, 0xb5c0fbcfU, 0xe9b5dba5U, 0x3956c25bU, 0x59f111f1U, 0x923f82a4U, 0xab1c5ed5U,
  0xd807aa98U, 0x12835b01U, 0x243185beU, 0x550c7dc3U, 0x72be5d74U, 0x80deb1feU, 0x9bdc06a7U, 0xc19bf174U,
  0xe49b69c1U, 0xefbe4786U, 0x0fc19dc6U, 0x240ca1ccU, 0x2de92c6fU, 0x4a7484aaU, 0x5cb0a9dcU, 0x76f988daU,
  0x983e5152U, 0xa831c66dU, 0xb00327c8U, 0xbf597fc7U, 0xc6e00bf3U, 0xd5a79147U, 0x06ca6351U, 0x14292967U,
  0x27b70a85U, 0x2e1b2138U, 0x4d2c6dfcU, 0x53380d13U, 0x650a7354U, 0x766a0abbU, 0x81c2c92eU, 0x92722c85U,
  0xa2bfe8a1U, 0xa81a664bU, 0xc24b8b70U, 0xc76c51a3U, 0xd192e819U, 0xd6990624U, 0xf40e3585U, 0x106aa070U,
  0x19a4c116U, 0x1e376c08U, 0x2748774cU, 0x34b0bcb5U, 0x391c0cb3U, 0x4ed8aa4aU, 0x5b9cca4fU, 0x682e6ff3U,
  0x748f82eeU, 0x78a5636fU, 0x84c87814U, 0x8cc70208U, 0x90befffaU, 0xa4506cebU, 0xbef9a3f7U, 0xc67178f2U
} ;

std::vector<uint8_t> hash::SHA256(std::vector<uint8_t> data)
{
  //padding
  std::size_t len = data.size()*8 ;
  data.push_back((1<<7)) ;
  while(data.size()%(512/8)!=(448/8)) data.push_back(0) ;
  uint8_t lenArr[8] ;
  for(int i=7;i>=0;i--)
  {
    lenArr[i] = len%(1<<8) ;
    len >>= 8 ;
  }
  for(int i=0;i<8;i++)
    data.push_back(lenArr[i]) ;

  uint32_t H[8] = {
    0x6a09e667U,
    0xbb67ae85U,
    0x3c6ef372U,
    0xa54ff53aU,
    0x510e527fU,
    0x9b05688cU,
    0x1f83d9abU,
    0x5be0cd19U
  } ;

  std::size_t cur = 0 ;
  while(cur<data.size())
  {
    //block decomposition
    uint32_t w[64] = {0} ;
    for(int i=0;i<16;i++)
      for(int j=0;j<4;j++)
        w[i] = (w[i]<<8)|data[cur++] ;

    for(int i=16;i<64;i++)
      w[i] = sigma1(w[i-2])+w[i-7]+sigma0(w[i-15])+w[i-16] ;

    //hash computation
    uint32_t a = H[0] ;
    uint32_t b = H[1] ;
    uint32_t c = H[2] ;
    uint32_t d = H[3] ;
    uint32_t e = H[4] ;
    uint32_t f = H[5] ;
    uint32_t g = H[6] ;
    uint32_t h = H[7] ;
    for(int r=0;r<64;r++)
    {
      uint32_t t1 = h+Sigma1(e)+Ch(e,f,g)+K[r]+w[r] ;
      uint32_t t2 = Sigma0(a)+Maj(a,b,c) ;
      h = g ;
      g = f ;
      f = e ;
      e = d+t1 ;
      d = c ;
      c = b ;
      b = a ;
      a = t1+t2 ;
    }
    H[0] += a ;
    H[1] += b ;
    H[2] += c ;
    H[3] += d ;
    H[4] += e ;
    H[5] += f ;
    H[6] += g ;
    H[7] += h ;
  }

  //output decomposition
  std::vector<uint8_t> res ;
  res.resize(32) ;
  cur = 0 ;
  for(int i=0;i<8;i++)
    for(int j=0;j<4;j++)
    {
      res[cur+3-2*(cur%4)] = H[i]%(1<<8) ;
      cur++ ;
      H[i] >>= 8 ;
    }

  return res ;
}

