#pragma once

#include "BlockCipher.h"

#include <random>
#include <string>

//interface
struct DeterministicRandomNumberGenerator
{
    uint64_t Next(uint64_t lb, uint64_t ub) ;
    virtual void Reseed(std::string seed) = 0 ;
    virtual ~DeterministicRandomNumberGenerator() {} ;
    protected:
        DeterministicRandomNumberGenerator() {} //prevent auto-gen
        virtual uint64_t _min() = 0 ;
        virtual uint64_t _max() = 0 ;
        virtual uint64_t _Next() = 0 ;
} ;

typedef DeterministicRandomNumberGenerator DRNG ;

struct AESCTRDRBG : public DRNG
{
    AESCTRDRBG(std::string seed) throw(std::string) ;
    void Reseed(std::string seed) ;
    private:
        uint64_t _min() ;
        uint64_t _max() ;
        uint64_t _Next() ;
} ;

struct RNGTester
{
    bool Test(DRNG *rng) throw(std::string) ;
} ;

