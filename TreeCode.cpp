#include "TreeCode.h"

#include <string>
#include <vector>

#include "DRNG.h"

TreeCodeEncoder::TreeCodeEncoder(std::string seed)
{
  this->_head = this->_cur = new TreeCodeNode(seed) ;
}

TreeCodeEncoder::~TreeCodeEncoder()
{
  delete this->_head ;
}

uint64_t TreeCodeEncoder::Encode(uint64_t data) throw(std::string)
{
  uint64_t res = this->_cur->GetLabel(data) ;
  this->_cur = this->_cur->GetNode(data) ;
  return res ;
}

TreeCodeNode::TreeCodeNode(std::string seed) : _rng(seed, "personalized for TreeCodeNode")
{
  for(int i=0;i<TREECODE_D;i++)
    this->_ptr[i] = NULL ;
}

TreeCodeNode::~TreeCodeNode()
{
  for(int i=0;i<TREECODE_D;i++)
    if(this->_ptr[i]!=NULL)
      delete this->_ptr[i] ;
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
    std::vector<uint8_t> seed = this->_rng.GetBits(256) ;
    std::string seed_str ;
    for(uint64_t j=0;j<seed.size();j+=8)
    {
      uint8_t cur = 0 ;
      for(int k=0;k<8;k++) cur = (cur<<1) | seed[j+k] ;
      seed_str.push_back((char)cur) ;
    }
    this->_ptr[i] = new TreeCodeNode(seed_str) ;
    seed = this->_rng.GetBits(TREECODE_S) ;
    this->_label[i] = 0 ;
    for(int j=0;j<TREECODE_S;j++) this->_label[i] = (this->_label[i]<<1) | seed[j] ;
  }
}

