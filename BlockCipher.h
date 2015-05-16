#pragma once

#include <vector>
#include <cstdint>
#include <string>

struct BlockCipher
{
    virtual std::vector<uint8_t> Encode(std::vector<uint8_t> plain) = 0 ;
    virtual std::vector<uint8_t> Decode(std::vector<uint8_t> cipher) = 0 ;
    protected:
        BlockCipher() {} ;
} ;

struct AES : public BlockCipher
{
    AES(std::vector<uint8_t> key) throw(std::string) ;
    std::vector<uint8_t> Encode(std::vector<uint8_t> plain) throw(std::string) ;
    std::vector<uint8_t> Decode(std::vector<uint8_t> cipher) throw(std::string) ;
    private :
        int _keySize ;
        std::vector<uint8_t> _expandedKey ;
        void _AddRoundKey(std::vector<uint8_t> &state, int iter) ;
        void _SubBytes(std::vector<uint8_t> &state) ;
        void _ShiftRows(std::vector<uint8_t> &state) ;
        void _MixColumns(std::vector<uint8_t> &state) ;
        void _KeyExpansion(std::vector<uint8_t> key) throw(std::string) ;
        uint8_t _GF8Mul(uint8_t a, int b) ;
        void _KeyCore(uint8_t *w, int iter) ;
        static const uint8_t _Rcon[16] ;
        static const uint8_t _SBox[256] ;
} ;

