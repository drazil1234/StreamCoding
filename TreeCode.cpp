#include "TreeCode.h"

#include <string>
#include <vector>

#include "DRNG.h"

TreeCodeNode::TreeCodeNode(AESCTRDRBG *rng)
{
  this->_rng = rng ;
  this->_ptr[0] = NULL ;
}

uint64_t TreeCodeNode::GetLabel(uint64_t branch) throw(std::string)
{
  if(branch>=TREECODE_D) throw(std::string("index too large")) ;
  if(this->_ptr[0]==NULL) this->_GenerateNodes() ;
  return this->_label[branch] ;
}

TreeCodeNode *TreeCodeNode::GetNode(uint64_t branch) throw(std::string)
{
  if(branch>=TREECODE_D) throw(std::string("index too large")) ;
  if(this->_ptr[0]==NULL) this->_GenerateNodes() ;
  return this->_ptr[branch] ;
}

void TreeCodeNode::_GenerateNodes()
{
  for(int i=0;i<TREECODE_D;i++)
  {
    std::vector<uint8_t> seed = this->_rng->GetBits(256) ;
    std::string seed_str ;
    for(uint64_t j=0;j<seed.size();j+=8)
    {
      uint8_t cur = 0 ;
      for(int k=0;k<8;k++) cur = (cur<<1) | seed[j+k] ;
      seed_str.push_back((char)cur) ;
    }
    this->_ptr[i] = new TreeCodeNode(new AESCTRDRBG(seed_str, "TreeCodeNodeRandomInitString")) ;
    seed = this->_rng->GetBits(TREECODE_S) ;
    this->_label[i] = 0 ;
    for(int j=0;j<TREECODE_S;j++) this->_label[i] = (this->_label[i]<<1) | seed[j] ;
  }
}

