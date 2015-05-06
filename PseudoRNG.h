#pragma once

class PseudoRNG
{
    public :
        PseudoRNG() ;
        uint64_t Next(uint64_t rangeMax) ;
        bool Initialize(uint64_t seed) ;
} ;
