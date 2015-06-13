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
  TreeCodeNode node(new AESCTRDRBG("123e;lkjsdffpogih24r;lkahsdf;oiefdkqjewr","asdfoi3rdfiadf;knx/zxvkln2p3")) ;
  ASSERT_THROW(node.GetLabel(2), std::string) ;
  ASSERT_THROW(node.GetNode(2), std::string) ;
}

std::vector<uint8_t> GetBits(TreeCodeNode *node, int n)
{
  std::vector<uint8_t> bits ;
  std::queue<TreeCodeNode *> q ;
  q.push(node) ;
  while(bits.size()<(unsigned int)n)
  {
    TreeCodeNode *node = q.front() ;
    q.pop() ;

    uint64_t label = node->GetLabel(0) ;
    for(int i=62;i>=0;i--)
      bits.push_back((label>>i)&1) ;
    label = node->GetLabel(1) ;
    for(int i=62;i>=0;i--)
      bits.push_back((label>>i)&1) ;
    q.push(node->GetNode(0)) ;
    q.push(node->GetNode(1)) ;
    delete node ;
  }
  while(q.size()>0)
  {
    TreeCodeNode *node = q.front() ;
    q.pop() ;
    delete node ;
  }

  return bits ;
}

TEST(TreeCodeNodeTest, RandomNess)
{
  int n = 7000000 ;
  TreeCodeNode *ptr = new TreeCodeNode(new AESCTRDRBG("sdofij3ijt2;kjfas;dfivdf0b244rlkjdf0bi qwer 23dfas''zx234", "asf2329asdvpe32143-z@#$1asd")) ;
  std::vector<uint8_t> bits = GetBits(ptr, n) ;
  RNGTester tester ;
  ASSERT_EQ(true, tester.Test(bits.data(), n)) ;
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv) ;
  return RUN_ALL_TESTS() ;
}

