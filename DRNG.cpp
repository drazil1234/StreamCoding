#include "DRNG.h"

#include "sts/rngtest.h"

#include <string>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <random>
#include <iostream>
#include <iomanip>
#include <limits>

#include "hash.h"
#include "BlockCipher.h"

std::vector<uint8_t> AESCTRDRBG::GetBits(uint64_t n)
{
  std::vector<uint8_t> tmp ;
  while(tmp.size()*8<n)
  {
    for(int i=15;i>=0;i--)
    {
      this->_V[i]++ ;
      if(this->_V[i]!=0)
        break ;
    }

    std::vector<uint8_t> block = this->_BlockEncrypt(this->_Key, this->_V) ;
    tmp.insert(tmp.end(), block.begin(), block.end()) ;
  }

  std::vector<uint8_t> out ;
  for(uint64_t i=0;i<n;i+=8)
  {
    uint8_t cur = tmp[i/8] ;
    for(int j=0;j<8 && i+j<n;j++)
    {
      out.push_back(cur&1) ;
      cur >>= 1 ;
    }
  }

  this->_Update(std::vector<uint8_t>(32, 0)) ;

  return out ;
}

uint64_t AESCTRDRBG::min()
{
  return 0 ;
}

uint64_t AESCTRDRBG::max()
{
  return std::numeric_limits<uint64_t>::max() ;
}

std::vector<uint8_t> AESCTRDRBG::_BlockEncrypt(std::vector<uint8_t> key, std::vector<uint8_t> v)
{
  return AES(key).Encode(v) ;
}

uint64_t AESCTRDRBG::operator()()
{
  std::vector<uint8_t> vec = this->GetBits(64) ;
  uint64_t ret = 0 ;
  for(int i=0;i<64;i++)
    ret = (ret<<1) | vec[i] ;
  return ret ;
}

void AESCTRDRBG::_Update(std::vector<uint8_t> data)
{
  std::vector<uint8_t> tmp ;

  for(int r=0;r<2;r++)
  {
    for(int i=15;i>=0;i--)
    {
      this->_V[i]++ ;
      if(this->_V[i]!=0)
        break ;
    }

    std::vector<uint8_t> odata = this->_BlockEncrypt(this->_Key, this->_V) ;
    tmp.insert(tmp.end(), odata.begin(), odata.end()) ;
  }

  for(int i=0;i<32;i++) tmp[i] ^= data[i] ;
  for(int i=0;i<16;i++)
    this->_Key[i] = tmp[i] ;
  for(int i=16;i<32;i++)
    this->_V[i-16] = tmp[i] ;
}

AESCTRDRBG::AESCTRDRBG(std::string seed, std::string personalize)
{
  std::string seed_material = seed + "AESCTRERBG" + personalize ;
  std::vector<uint8_t> seed_mat ;
  for(auto ch: seed_material) seed_mat.push_back((uint8_t)ch) ;
  seed_mat = hash::SHA256(seed_mat) ;
  this->_Key.resize(16, 0) ;
  this->_V.resize(16, 0) ;
  this->_Update(seed_mat) ;
}

bool RNGTester::Test(DRNG *rng) throw(std::string)
{
  if(rng==NULL)
    throw(std::string("RNG cannot be NULL.")) ;

  unsigned char *eps = new unsigned char[MY_N] ;
  for(int i=0;i<MY_N;i++)
    eps[i] = rng->Next(0, 1) ;

  bool res = this->Test(eps, MY_N) ;
  
  delete []eps ;

  return res ;
}

bool RNGTester::Test(unsigned char *eps, int n)
{
  double p_value = 1.0 ;

  std::cout << std::fixed << std::setprecision(6) ;

  p_value = std::min(p_value, Frequency(n, eps) ) ;
  p_value = std::min(p_value, BlockFrequency(n/100, n, eps) ) ;
  p_value = std::min(p_value, Runs(n, eps) ) ;
  p_value = std::min(p_value, LongestRunOfOnes(n, eps) ) ;
  //p_value = std::min(p_value, Rank(n, eps) ) ;
  //p_value = std::min(p_value, DiscreteFourierTransform(n, eps) ) ;
  //p_value = std::min(p_value, NonOverlappingTemplateMatchings(10, n, eps) ) ;
  //p_value = std::min(p_value, OverlappingTemplateMatchings(10, n, eps) ) ;
  p_value = std::min(p_value, Universal(n, eps) ) ;
  p_value = std::min(p_value, LinearComplexity(1000, n, eps) ) ;
  p_value = std::min(p_value, Serial(5, n, eps) ) ;
  //p_value = std::min(p_value, ApproximateEntropy(5, n, eps) ) ;
  p_value = std::min(p_value, CumulativeSums(n, eps) ) ;
  //p_value = std::min(p_value, RandomExcursions(n, eps) ) ;
  //p_value = std::min(p_value, RandomExcursionsVariant(n, eps) ) ;

  std::cout << "p_value = " << p_value << std::endl ;

  return (p_value>=0.01) ; 
}

uint64_t DRNG::Next(uint64_t lb, uint64_t ub)
{
  std::uniform_int_distribution<uint64_t> d(lb, ub) ;
  return d(*this) ;
}

