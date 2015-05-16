#include "gtest/gtest.h"

#include "BlockCipher.h"

#include <vector>
#include <string>
#include <algorithm>

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
    protected:
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

TEST(RNGTester, BadRNG)
{
}

TEST(RNGTester, GoodRNG)
{
}

TEST(DRNG, Randomness)
{
}

TEST(DRNG, MinMaxValues)
{
}

TEST(DRNG, RandomnessAfterReseed)
{
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv) ;
    return RUN_ALL_TESTS() ;
}

