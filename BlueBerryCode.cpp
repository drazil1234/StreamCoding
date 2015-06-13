#include "BlueBerryCode.h"
#include <ctime>
uint64_t BlueBerry::Encode(uint64_t x)
{
    return x<<1;
}
uint64_t BlueBerry::Decode(uint64_t x)
{
    if(x%2==1)return 1;
    else return x>>1; 
}
    
