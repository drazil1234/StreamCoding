#include "TreeCode.h"

#include <string>
#include <vector>
#include "DRNG.h"

TreeCodeDecoder::TreeCodeDecoder(std::string seed)
{
  this->_path.push_back(new TreeCodeNode(seed)) ;
}

TreeCodeDecoder::~TreeCodeDecoder()
{
  delete this->_path[0] ;
}

TreeCodeNode **N, **nodes ;
uint64_t *A, *L, *I ;
uint64_t n ;
uint64_t best ;
uint64_t *ans, *label ;

void dfs(TreeCodeNode *cur, uint64_t r, uint64_t myerr)
{
  if(myerr>=best)
    return ;

  if(r==n) 
  {
    for(uint64_t i=0;i<n;i++)
    {
      nodes[i+1] = N[i] ;
      ans[i] = A[i] ;
      label[i] = L[i] ;
    }
    best = myerr ;
    return ;
  }

  for(uint64_t i=0;i<TREECODE_D;i++)
  {
    N[r] = cur->GetNode(i) ;
    L[r] = cur->GetLabel(i) ;
    A[r] = i ;
    dfs(N[r], r+1, myerr+((L[r]==I[r])?0:1)) ;
    if(myerr>=best)
      return ;
  }
}

void findNearest(TreeCodeNode **_nodes, uint64_t *_ans, uint64_t *_label, uint64_t *input, uint64_t _n)
{
  n = _n ;
  N = new TreeCodeNode *[n] ;
  A = new uint64_t[n] ;
  L = new uint64_t[n] ;
  I = input ;
  best = n+1 ;
  ans = _ans ;
  label = _label ;
  nodes = _nodes ;

  dfs(nodes[0], 0, 0) ;

  delete []N ;
  delete []A ;
  delete []L ;
}

const std::vector<uint64_t> &TreeCodeDecoder::GetDecoded()
{
  return this->_decoded ;
}

const std::vector<uint64_t> &TreeCodeDecoder::Decode(uint64_t data) throw(std::string)
{
  if(data > (1ULL<<TREECODE_S))
    throw(std::string("data out of range")) ;

  this->_input.push_back(data) ;

  TreeCodeNode *cur = this->_path[this->_path.size()-1] ;
  for(uint64_t i=0;i<TREECODE_D;i++)
    if(cur->GetLabel(i) == data)
    {
      this->_decoded.push_back(i) ;
      this->_path.push_back(cur->GetNode(i)) ;
      this->_label.push_back(data) ;
      return this->_decoded ;
    }
  this->_decoded.push_back(0) ;
  this->_path.push_back(cur->GetNode(0)) ;
  this->_label.push_back(0) ;
  std::vector<uint64_t> error ;
  for(uint64_t i=0;i<this->_label.size();i++)
    error.push_back((this->_label[i]==this->_input[i])?0:1) ;
  if(error.size()>=2U)
      for(uint64_t i=0;i<error.size()-1;i++)
    	  error[error.size()-i-2] += error[error.size()-i-1] ;
  uint64_t ptr = error.size()-1 ;
  for(uint64_t i=0;i<error.size();i++)
    if(error[i]>TREECODE_ALPHA*(error.size()-i)/2)
    {
      ptr = i ;
      break ;
    }
  if(ptr!=error.size()-1)
    findNearest(this->_path.data()+ptr, this->_decoded.data()+ptr, this->_label.data()+ptr, this->_input.data()+ptr, this->_input.size()-ptr) ;
  return this->_decoded ;
}

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
  if(data >= TREECODE_D)
    throw(std::string("data out of range")) ;
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

