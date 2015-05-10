#include "gtest/gtest.h"

TEST(AESCTR, EncryptShort)
{
}

TEST(AESCTR, EncryptLong)
{
}

TEST(AESCTR, TwoWayShort)
{
}

TEST(AESCTR, TwoWayLong)
{
}

TEST(AESCTR, Reseed)
{
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

