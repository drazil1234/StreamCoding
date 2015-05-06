#pragma once

class BlueBerryCode
{
    public :
        BlueBerryCode(uint32_t S, uint32_t L) ;
        uint32_t Encode(uint32_t x, uint64_t seed) ;
        uint32_t Decode(uint32_t x, uint64_t seed) ;
} ;
