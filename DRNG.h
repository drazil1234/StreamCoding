#pragma once

#include "BlockCipher.h"

#include <random>
#include <string>

//interface
struct DeterministicRandomNumberGenerator
{
    uint64_t Next(uint64_t lb, uint64_t ub) ;
    virtual void Reseed(std::string seed) throw(std::string)
    {
      throw(std::string("Not implemented/supported. seed=") + seed) ;
    }
    virtual ~DeterministicRandomNumberGenerator() {} ;
    protected:
        DeterministicRandomNumberGenerator() {} //prevent auto-gen
        virtual uint64_t min() = 0 ;
        virtual uint64_t max() = 0 ;
        virtual uint64_t operator()() = 0 ;
        typedef uint64_t result_type ;

    friend std::uniform_int_distribution<uint64_t> ;
} ;

typedef DeterministicRandomNumberGenerator DRNG ;

struct AESCTRDRBG : public DRNG
{
    AESCTRDRBG(std::string seed) throw(std::string) ;
    void Reseed(std::string seed) throw(std::string) ;
    protected:
        uint64_t min() ;
        uint64_t max() ;
        uint64_t operator()() ;
} ;

struct RNGTester
{
    bool Test(DRNG *rng) throw(std::string) ;
} ;

