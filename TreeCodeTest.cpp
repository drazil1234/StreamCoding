#include "gtest/gtest.h"

#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <queue>

#include "TreeCode.h"
#include "DRNG.h"

TEST(TreeCodeNodeTest, RangeTest)
{
  TreeCodeNode node("asdfoi3rdfiadf;knx/zxvkln2p3") ;
  ASSERT_THROW(node.GetLabel(2), std::string) ;
  ASSERT_THROW(node.GetNode(2), std::string) ;
}

std::vector<uint8_t> GetBits(TreeCodeNode *node, int n, bool rev = false)
{
  std::vector<uint8_t> bits ;
  std::queue<TreeCodeNode *> q ;
  q.push(node) ;
  while(bits.size()<(unsigned int)n)
  {
    TreeCodeNode *node = q.front() ;
    q.pop() ;

    for(uint64_t b=0;b<TREECODE_D;b++)
    {
      uint64_t eb = (rev?b:TREECODE_D-b-1) ;
      uint64_t label = node->GetLabel(eb) ;
      for(int i=62;i>=0;i--)
        bits.push_back((label>>i)&1) ;
      q.push(node->GetNode(eb)) ;
    }
  }

  return bits ;
}

TEST(TreeCodeNodeTest, FunctionalTest)
{
  TreeCodeNode *ptr ;
  ASSERT_NO_THROW(ptr = new TreeCodeNode("2134shd;fsdf2RAsf23$@WAFSDvoz 134sdf")) ;
  ASSERT_NO_THROW(GetBits(ptr, 10000)) ;
  ASSERT_NO_THROW(delete ptr) ;
}

TEST(TreeCodeNodeTest, Stability)
{
  TreeCodeNode node1("2134shd;fsdf2RAsf23$@WAFSDvoz 134sdf") ;
  TreeCodeNode node2("2134shd;fsdf2RAsf23$@WAFSDvoz 134sdf") ;

  GetBits(&node1, 10000) ;
  GetBits(&node2, 10000, true) ;

  ASSERT_EQ(GetBits(&node1, 10000), GetBits(&node2, 10000)) ;
}

TEST(TreeCodeNodeTest, RandomNess)
{
  int n = 7000000 ;
  TreeCodeNode *ptr = new TreeCodeNode("sdofij3ijt2;kjfas;dfivdf0b244rlkjdf0bi qwer 23dfas''zx234") ;
  std::vector<uint8_t> bits = GetBits(ptr, n) ;
  RNGTester tester ;
  ASSERT_EQ(true, tester.Test(bits.data(), n)) ;
  delete ptr ;
}

TEST(TreeCode, RangeTest)
{
  TreeCodeEncoder enc("The seed of the test of the TreeCode") ;
  TreeCodeDecoder dec("The seed of the test of the TreeCode") ;

  ASSERT_THROW(enc.Encode(2), std::string) ;
  ASSERT_THROW(dec.Decode(1ULL<<63), std::string) ;
}

TEST(TreeCode, Funtional)
{
  TreeCodeEncoder enc("The seed of the test of the TreeCode") ;
  TreeCodeDecoder dec("The seed of the test of the TreeCode") ;

  std::vector<uint64_t> data ;
  for(int r=0;r<100;r++)
  {
    uint64_t bit = rand()%2 ;
    data.push_back(bit) ;
    const std::vector<uint64_t> &ans = dec.Decode(enc.Encode(bit)) ;
    ASSERT_EQ(data, ans) ;
  }
}

TEST(TreeCode, ErrorCorrection1)
{
  TreeCodeEncoder enc("The seed of the test of the TreeCode") ;
  TreeCodeDecoder dec("The seed of the test of the TreeCode") ;

  std::vector<uint64_t> data ;
  for(int r=0;r<100;r++)
  {
    uint64_t bit = rand()%2 ;
    data.push_back(bit) ;
    const std::vector<uint64_t> &ans = dec.Decode(enc.Encode(bit)^((r<5)?rand()%(1ULL<<TREECODE_S):0)) ;
    if(r==99)
      ASSERT_EQ(data, ans) ;
  }
}

TEST(TreeCode, ErrorCorrection2)
{
  TreeCodeEncoder enc("The seed of the test of the TreeCode") ;
  TreeCodeDecoder dec("The seed of the test of the TreeCode") ;

  std::vector<uint64_t> data ;
  for(int r=0;r<100;r++)
  {
    uint64_t bit = rand()%2 ;
    data.push_back(bit) ;
    const std::vector<uint64_t> &ans = dec.Decode(enc.Encode(bit)^((r>=5 && r<10)?rand()%(1ULL<<TREECODE_S):0)) ;
    if(r==99)
      ASSERT_EQ(data, ans) ;
  }
}

TEST(TreeCode, ErrorCorrection3)
{
  TreeCodeEncoder enc("The seed of the test of the TreeCode") ;
  TreeCodeDecoder dec("The seed of the test of the TreeCode") ;

  std::vector<uint64_t> data ;
  for(int r=0;r<100;r++)
  {
    uint64_t bit = rand()%2 ;
    data.push_back(bit) ;
    const std::vector<uint64_t> &ans = dec.Decode(enc.Encode(bit)^((r==15 || r==17 || r==19 || r==50 || r==51 || r==70)?rand()%(1ULL<<TREECODE_S):0)) ;
    if(r==99)
      ASSERT_EQ(data, ans) ;
  }
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv) ;
  return RUN_ALL_TESTS() ;
}

