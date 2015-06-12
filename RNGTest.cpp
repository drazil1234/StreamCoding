#include "gtest/gtest.h"

#include "BlockCipher.h"
#include "DRNG.h"
#include "hash.h"

#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <cstdio>
#include <cstdlib>
#include <cstring>

struct AESTestCase
{
  std::string key, iv ;
  std::vector<std::string> plain, cipher ;
} AESCTRTestVectors[] = {
  {
    "2b7e151628aed2a6abf7158809cf4f3c","f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff",
    {"6bc1bee22e409f96e93d7e117393172a","ae2d8a571e03ac9c9eb76fac45af8e51","30c81c46a35ce411e5fbc1191a0a52ef","f69f2445df4f9b17ad2b417be66c3710"},
    {"874d6191b620e3261bef6864990db6ce","9806f66b7970fdff8617187bb9fffdff","5ae4df3edbd5d35e5b4f09020db03eab","1e031dda2fbe03d1792170a0f3009cee"}
  },
  {
    "8e73b0f7da0e6452c810f32b809079e562f8ead2522c6b7b","f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff",
    {"6bc1bee22e409f96e93d7e117393172a","ae2d8a571e03ac9c9eb76fac45af8e51","30c81c46a35ce411e5fbc1191a0a52ef","f69f2445df4f9b17ad2b417be66c3710"},
    {"1abc932417521ca24f2b0459fe7e6e0b","090339ec0aa6faefd5ccc2c6f4ce8e94","1e36b26bd1ebc670d1bd1d665620abf7","4f78a7f6d29809585a97daec58c6b050"}
  },
  {
    "603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4","f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff",
    {"6bc1bee22e409f96e93d7e117393172a","ae2d8a571e03ac9c9eb76fac45af8e51","30c81c46a35ce411e5fbc1191a0a52ef","f69f2445df4f9b17ad2b417be66c3710"},
    {"601ec313775789a5b7a7f504bbf3d228","f443e3ca4d62b59aca84e990cacaf5c5","2b0930daa23de94ce87017ba2d84988d","dfc9c58db67aada613c2dd08457941a6"}
  }
} ;

uint8_t getVal(char ch)
{
  if('0' <= ch && ch <= '9') return (uint8_t)(ch-'0') ;
  else return (uint8_t)(ch-'a')+10 ;
}

std::vector<uint8_t> Decode(std::string str)
{
  std::vector<uint8_t> vec ;
  for(int i=0;i+1<(int)str.length();i+=2)
  {
    vec.push_back(getVal(str[i])*16+getVal(str[i+1])) ;
  }
  return vec ;
}

class AESTest : public ::testing::TestWithParam<AESTestCase>
{
} ;

TEST_P(AESTest, EncryptDecrypt)
{
  int cases = std::min(GetParam().plain.size(), GetParam().cipher.size()) ;
  ASSERT_GT(cases, 0) << "No plain/cipher to test" ;
  AES aes(Decode(GetParam().key)) ;
  std::vector<uint8_t> plain = Decode(GetParam().iv) ;
  for(int i=0;i<cases;i++)
  {
    std::vector<uint8_t> cipher = aes.Encode(plain) ;
    ASSERT_EQ(cipher.size(), (size_t)16) << "Incorrect OutputBlockSize." ;

    //increase CTR
    for(int j=15;j>=0;j--)
      if((++plain[j])!=0)
        break ;

    for(int j=0;j<16;j++)
      ASSERT_EQ(Decode(GetParam().cipher[i])[j], cipher[j]^Decode(GetParam().plain[i])[j]) << "AES Encode fail" ;
  }
}

INSTANTIATE_TEST_CASE_P(AESTest, AESTest, ::testing::ValuesIn(AESCTRTestVectors)) ;

class BlockCipherAdapter_AESCTR : public ::testing::TestWithParam<AESTestCase>
{
} ;

TEST_P(BlockCipherAdapter_AESCTR, AESCTR)
{
  int cases = std::min(GetParam().plain.size(), GetParam().cipher.size()) ;
  ASSERT_GT(cases, 0) << "No plain/cipher to test" ;

  AES aes(Decode(GetParam().key)) ;
  BlockCipherAdapter adapter(&aes, Decode(GetParam().iv), BlockCipherAdapter::Mode::CTR) ;

  for(int i=0;i<cases;i++)
    ASSERT_EQ(Decode(GetParam().cipher[i]), adapter.Encode(Decode(GetParam().plain[i]))) ;
}

INSTANTIATE_TEST_CASE_P(BlockCipherAdapter_AESCTR, BlockCipherAdapter_AESCTR, ::testing::ValuesIn(AESCTRTestVectors)) ;

TEST(BlockCipherAdapterTest, NULLAlgo)
{
  ASSERT_THROW(BlockCipherAdapter(NULL, Decode(AESCTRTestVectors[0].iv), BlockCipherAdapter::Mode::CTR), std::string) ;
}

TEST(BlockCipherAdapterTest, WrongIvLength)
{
  AES aes(Decode(AESCTRTestVectors[0].key)) ;
  ASSERT_THROW(BlockCipherAdapter(&aes, Decode(""), BlockCipherAdapter::Mode::CTR), std::string) ;
  ASSERT_THROW(BlockCipherAdapter(&aes, Decode("01234567890123456789"), BlockCipherAdapter::Mode::CTR), std::string) ;
  ASSERT_THROW(BlockCipherAdapter(&aes, Decode("0123456789012345678901234567890123456789012345678901234567890123456789"), BlockCipherAdapter::Mode::CTR), std::string) ;
}

TEST(BlockCipherAdapterTest, WrongInputLength)
{
  AES aes(Decode(AESCTRTestVectors[0].key)) ;
  BlockCipherAdapter adapter(&aes, Decode(AESCTRTestVectors[0].iv), BlockCipherAdapter::Mode::CTR) ;
  ASSERT_THROW(adapter.Encode(Decode("")), std::string) ;
  ASSERT_THROW(adapter.Encode(Decode("01234567890123456789")), std::string) ;
  ASSERT_THROW(adapter.Encode(Decode("0123456789012345678901234567890123456789012345678901234567890123456789")), std::string) ;
}

TEST(AESTest, WrongKeyLength)
{
  ASSERT_THROW(AES(Decode("")), std::string) ;
  ASSERT_THROW(AES(Decode("01234567890123456789")), std::string) ;
  ASSERT_THROW(AES(Decode("0123456789012345678901234567890123456789012345678901234567890123456789")), std::string) ;
}

TEST(AESTest, WrongInputBlockSize)
{
  AES aes(Decode(AESCTRTestVectors[0].key)) ;
  ASSERT_THROW(aes.Encode(Decode("")), std::string) ;
  ASSERT_THROW(aes.Encode(Decode("01234567890123456789")), std::string) ;
  ASSERT_THROW(aes.Encode(Decode("0123456789012345678901234567890123456789012345678901234567890123456789")), std::string) ;
}

TEST(AESTest, BlockSize)
{
  AES aes(Decode(AESCTRTestVectors[0].key)) ;
  ASSERT_EQ((size_t)16, aes.InputSize()) ;
  ASSERT_EQ((size_t)16, aes.OutputSize()) ;
}

TEST(SHA256Test, HashTest)
{
  std::vector<uint8_t> input ;
  std::vector<uint8_t> expected_output ;
  for(uint8_t i = 0x61; i<=0x63; i++)
    input.push_back(i) ;
  expected_output = {
    0xbaU, 0x78U, 0x16U, 0xbfU,
    0x8fU, 0x01U, 0xcfU, 0xeaU,
    0x41U, 0x41U, 0x40U, 0xdeU,
    0x5dU, 0xaeU, 0x22U, 0x23U,
    0xb0U, 0x03U, 0x61U, 0xa3U,
    0x96U, 0x17U, 0x7aU, 0x9cU,
    0xb4U, 0x10U, 0xffU, 0x61U,
    0xf2U, 0x00U, 0x15U, 0xadU
  } ;
  ASSERT_EQ(hash::SHA256(input), expected_output) ;
  input.clear() ;
  for(uint8_t i=0x61;i<=0x6e;i++)
    for(int j=0;j<4;j++)
      input.push_back(i+j) ;
  expected_output = 
  {
    0x24U, 0x8dU, 0x6aU, 0x61U,
    0xd2U, 0x06U, 0x38U, 0xb8U,
    0xe5U, 0xc0U, 0x26U, 0x93U,
    0x0cU, 0x3eU, 0x60U, 0x39U,
    0xa3U, 0x3cU, 0xe4U, 0x59U,
    0x64U, 0xffU, 0x21U, 0x67U,
    0xf6U, 0xecU, 0xedU, 0xd4U,
    0x19U, 0xdbU, 0x06U, 0xc1U
  } ;
  ASSERT_EQ(hash::SHA256(input), expected_output) ;
  input.clear() ;
  for(int i=0;i<1000000;i++)
    input.push_back(0x61) ;
  expected_output = 
  {
    0xcdU, 0xc7U, 0x6eU, 0x5cU,
    0x99U, 0x14U, 0xfbU, 0x92U,
    0x81U, 0xa1U, 0xc7U, 0xe2U,
    0x84U, 0xd7U, 0x3eU, 0x67U,
    0xf1U, 0x80U, 0x9aU, 0x48U,
    0xa4U, 0x97U, 0x20U, 0x0eU,
    0x04U, 0x6dU, 0x39U, 0xccU,
    0xc7U, 0x11U, 0x2cU, 0xd0U
  } ;
  ASSERT_EQ(hash::SHA256(input), expected_output) ;
}

TEST(RNGTester, BadRNG)
{
  struct badRNG : public DRNG
  {
    protected:
    uint64_t operator()()
    {
      return (uint64_t) sqrt(rand()) ;
    } 
    uint64_t max()
    {
      return (uint64_t) sqrt(RAND_MAX) ;
    }
    uint64_t min()
    {
      return 0 ;
    }         
  } bad ;

  RNGTester tester ;
  ASSERT_EQ(tester.Test(&bad), false) ;
}

TEST(RNGTester, NULLTest)
{
  RNGTester tester ;
  ASSERT_THROW(tester.Test(NULL), std::string) ;
}

TEST(RNGTester, GoodRNG)
{
  struct goodRNG : public DRNG
  {
    protected:
    uint64_t operator()()
    {
      return (uint64_t) rand() ;
    } 
    uint64_t max()
    {
      return (uint64_t) RAND_MAX ;
    }
    uint64_t min()
    {
      return 0 ;
    }
  } good ;

  RNGTester tester ;
  ASSERT_EQ(tester.Test(&good), true) ;
}

/*
TEST(AESDRNG, MinMaxValues)
{
}

TEST(AESDRNG, SeedConsistency)
{
}
*/

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv) ;
  return RUN_ALL_TESTS() ;
}

