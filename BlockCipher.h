#pragma once

#include <vector>

struct BlockCipher
{
    enum Mode {CTR} ;
    virtual std::vector<uint8_t> Encode(std::vector<uint8_t> data) = 0 ;
    virtual std::vector<uint8_t> Decode(std::vector<uint8_t> cipher) = 0 ;
    private :
        BlockCipher() {} //prevent auto-gen 
} ;

struct AES : public BlockCipher
{
    AES(std::vector<uint8_t> key, std::vector<uint8_t> iv, Mode mode) ;
    std::vector<uint8_t> Encode(std::vector<uint8_t> data) ;
    std::vector<uint8_t> Decode(std::vector<uint8_t> cipher) ;
} ;

