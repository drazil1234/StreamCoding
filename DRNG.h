#pragma once

#include <BlockCipher>

#include <string>

//interface
struct DeterministicRandomNumberGenerator
{
    virtual uint64_t Next(uint64_t lb, uint64_t ub) = 0 ;
    virtual void Reseed(std::string seed) = 0 ;
    virtual bool Qualify() = 0 ;
    virtual ~DeterministicRandomNumberGenerator() {} ;
    private:
        DeterministicRandomNumberGenerator() {} //prevent auto-gen
} ;

typedef DeterministicRandomNumberGenerator DRNG ;

struct BlockCipherDRBG : public DRNG
{
    BlockCipherDRBG(BlockCipher *bc) ;
    void Reseed(std::string seed) ;
    bool Qualify() ;
} ;

