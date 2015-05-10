#include "gtest/gtest.h"

TEST (TEST1, Test1_1)
{
    ASSERT_EQ(1,1) ;
}

TEST (TEST1, TEST1_2)
{
    EXPECT_EQ(1,1) ;
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv) ;
    return RUN_ALL_TESTS() ;
}

