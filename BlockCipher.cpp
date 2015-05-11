#include "BlockCipher.h"

#include <vector>
#include <cstdint>
#include <algorithm>

AES::AES(std::vector<uint8_t> key, std::vector<uint8_t> iv, Mode mode) throw(std::string)
    : BlockCipher()
{
    if(mode!=Mode::CTR)
        throw(std::string("Not supported mode.")) ;

    std::fill(this->_iv, this->_iv+16, 0) ;
    for(int i=0;i<std::min((int)iv.size(), 16);i++)
        this->_iv[i] = iv[i] ;

    this->_KeyExpansion(key) ;
}

std::vector<uint8_t> AES::Encode(std::vector<uint8_t> plain)
{
    return plain ;
}

std::vector<uint8_t> AES::Decode(std::vector<uint8_t> cipher)
{
    return cipher ;
}

void AES::_IncreaseCtr()
{
    for(int i=15;i>=0;i--)
        if(++this->_iv[i]!=0)
            break ;
}

void AES::_KeyExpansion(std::vector<uint8_t> key) throw(std::string)
{
    switch(key.size())
    {
        case 16:
            this->_nR = 10 ;
            break ;
        case 24:
            this->_nR = 12 ;
            break ;
        case 32:
            this->_nR = 14 ;
            break ;
        default:
            throw(std::string("Wrong key size.")) ;
            break ;
    }
}

