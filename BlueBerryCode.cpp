#include "BlueBerryCode.h"
#include <ctime>
#include <cstdint>
uint64_t BlueBerryCode::Encode(uint64_t x)
{
    return x<<1;
}
uint64_t BlueBerryCode::Decode(uint64_t x)
{
    if(x%2==1)return 1;
    else return x>>1; 
}
    
