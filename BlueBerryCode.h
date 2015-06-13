#pragma once
#include<vector>
#include<map>

class BlueBerryCode
{
    public :
        uint64_t Encode(uint64_t x) ;
        uint64_t Decode(uint64_t x) ;
    
    private:
        uint64_t S = 1<<62;
        uint64_t L = S<<1;    
} ;
