#include "PseudoRNG.h"

#include <string>
#include <algorithm>

PseudoRNG::PseudoRNG(std::string seed)
{
    this->Initialize(seed) ;
}

void PseudoRNG::Initialize(std::string seed)
{
    this->_i = this->_j = 0 ;
    for(int i=0;i<256;i++)
        this->_S[i] = seed[i%seed.size()] ;
}

uint64_t PseudoRNG::Next(uint8_t bits)
{
    uint64_t number = 0 ;
    
    while(bits>0)
    {
        uint8_t addBits = std::min(bits, uint8_t(8)) ;
        number <<= addBits ;
        number |= this->_NextByte()>>(8-addBits) ;
        bits -= addBits ;
    }

    return number ;
}

uint8_t PseudoRNG::_NextByte()
{
    this->_i = this->_i+1 ;
    this->_j = this->_j+this->_S[this->_i] ;
    std::swap(this->_S[this->_i], this->_S[this->_j]) ;
    return this->_S[this->_S[this->_i]+this->_S[this->_j]] ;
}

bool PseudoRNG::Qualify()
{
    //TODO
    return false ;
}
