#pragma once

#include "DRNG.h"

#define TREECODE_D 2
#define TREECODE_S 63

#include <string>
#include <cstdint>

struct TreeCodeNode
{
  TreeCodeNode(AESCTRDRBG* rng) ;
  uint64_t GetLabel(uint64_t branch) throw(std::string) ;
  TreeCodeNode *GetNode(uint64_t branch) throw(std::string) ;
  private :
    void _GenerateNodes() ;
    AESCTRDRBG *_rng ;
    TreeCodeNode *_ptr[TREECODE_D] ;
    uint64_t _label[TREECODE_D] ;
} ;
