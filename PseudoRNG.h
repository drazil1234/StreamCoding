#pragma once

#include <string>

class PseudoRNG
{
    public:
        PseudoRNG(std::string seed) ;
        uint64_t Next(uint8_t bits) ;
        void Initialize(std::string seed) ;
        bool Qualify() ;
    private:
        uint8_t _i, _j ;
        uint8_t _NextByte() ;
        uint8_t _S[256] ;
} ;

