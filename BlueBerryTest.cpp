#include"gtest/gtest.h"
#include "BlueBerryCode.h"
TEST(blueberrycode,Encode)
{
    BlueBerryCode BBC;
    std::cout << "simple test: 0 & 1" << std::endl;
    EXPECT_EQ(0ull,BBC.Encode(0));
    EXPECT_EQ(2ull,BBC.Encode(1));
    std::cout << "big number test: (1<<62)-1 & (1<<63)-1 " << std::endl;
    EXPECT_EQ((1ull<<63)-2,BBC.Encode((1ull<<62)-1));
    EXPECT_EQ(18446744073709551614ull,BBC.Encode(9223372036854775807ull));
}
TEST(blueberrycode,Decode)
{
    BlueBerryCode BBC;
    std::cout << "simple test: 0 & 2" << std::endl;
    EXPECT_EQ(0ull,BBC.Decode(0));
    EXPECT_EQ(2ull,BBC.Decode(4));
    std::cout << "error check: 1 & 3" << std::endl;
    EXPECT_EQ(1ull,BBC.Decode(1));
    EXPECT_EQ(1ull,BBC.Decode(3));
    std::cout << "big number: 1<<63: " << std::endl; 
    EXPECT_EQ(1ull<<62,BBC.Decode(1ull<<63));
    std::cout << "(1<<63)-1 : 18446744073709551614" << std::endl;
    EXPECT_EQ(9223372036854775807ull,BBC.Decode(18446744073709551614ull));
}
int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv) ;
  return RUN_ALL_TESTS() ;
}

