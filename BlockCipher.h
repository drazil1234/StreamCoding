#pragma once

#include <vector>
#include <cstdint>
#include <string>

struct BlockCipher
{
    enum Mode {CTR} ;
    virtual std::vector<uint8_t> Encode(std::vector<uint8_t> data) = 0 ;
    virtual std::vector<uint8_t> Decode(std::vector<uint8_t> cipher) = 0 ;
    protected :
        BlockCipher() {} //prevent auto-gen 
} ;

struct AES : public BlockCipher
{
    AES(std::vector<uint8_t> key, std::vector<uint8_t> iv, Mode mode) throw(std::string) ;
    std::vector<uint8_t> Encode(std::vector<uint8_t> plain) ;
    std::vector<uint8_t> Decode(std::vector<uint8_t> cipher) ;
    private :
        int _nR ;
        std::vector<uint8_t> _expanededKey ;
        uint8_t _iv[16] ;
        void _IncreaseCtr() ;
        void _KeyExpansion(std::vector<uint8_t> key) throw(std::string) ;
} ;

