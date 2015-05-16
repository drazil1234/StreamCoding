#pragma once

#include <vector>
#include <cstdint>
#include <string>

struct BlockCipher
{
    virtual std::vector<uint8_t> Encode(std::vector<uint8_t> plain) = 0 ;
    virtual std::vector<uint8_t> Decode(std::vector<uint8_t> cipher) = 0 ;
    virtual size_t InputSize() = 0 ;
    virtual size_t OutputSize() = 0 ;
    protected:
        BlockCipher() {} ;
} ;

struct BlockCipherAdapter
{
    enum Mode {CTR} ;
    BlockCipherAdapter(BlockCipher *algo, std::vector<uint8_t> iv, Mode mode, bool isStream) throw(std::string) ;
    std::vector<uint8_t> Encode(std::vector<uint8_t> plain) throw(std::string) ;
    std::vector<uint8_t> Decode(std::vector<uint8_t> cipher) throw(std::string) ;
    private:
        bool _isStream ;
        std::vector<uint8_t> _iv ;
} ;


struct AES : public BlockCipher
{
    AES(std::vector<uint8_t> key) throw(std::string) ;
    std::vector<uint8_t> Encode(std::vector<uint8_t> plain) throw(std::string) ;
    std::vector<uint8_t> Decode(std::vector<uint8_t> cipher) throw(std::string) ;
    size_t InputSize() ;
    size_t OutputSize() ;
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

