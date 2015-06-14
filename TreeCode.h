#pragma once

#include "DRNG.h"

#define TREECODE_D 2
#define TREECODE_S 63
#define TREECODE_ALPHA 0.5

#include <string>
#include <cstdint>

struct TreeCodeNode
{
  TreeCodeNode(std::string seed) ;
  uint64_t GetLabel(uint64_t branch) throw(std::string) ;
  TreeCodeNode *GetNode(uint64_t branch) throw(std::string) ;
  ~TreeCodeNode() ;
  private :
    void _GenerateNodes() ;
    AESCTRDRBG _rng ;
    TreeCodeNode *_ptr[TREECODE_D] ;
    uint64_t _label[TREECODE_D] ;
} ;

struct TreeCodeEncoder
{
  TreeCodeEncoder(std::string seed) ;
  ~TreeCodeEncoder() ;
  uint64_t Encode(uint64_t data) throw(std::string) ;
  private :
    TreeCodeNode *_head, *_cur ;
} ;

struct TreeCodeDecoder
{
  TreeCodeDecoder(std::string seed) ;
  ~TreeCodeDecoder() ;
  const std::vector<uint64_t> &GetDecoded() ;
  const std::vector<uint64_t> &Decode(uint64_t data) throw(std::string) ;
  private:
    std::vector<TreeCodeNode *> _path ;
    std::vector<uint64_t> _decoded ;
    std::vector<uint64_t> _label ;
    std::vector<uint64_t> _input ;
} ;

