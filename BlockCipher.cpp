#include "BlockCipher.h"

#include <vector>
#include <cstdint>
#include <algorithm>
#include <cstring>

constexpr uint8_t AES::_Rcon[16] = {0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a} ;
constexpr uint8_t AES::_SBox[256] =
{
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
} ;

AES::AES(std::vector<uint8_t> key) throw(std::string)
    : BlockCipher()
{
    this->_KeyExpansion(key) ;
}

void AES::_AddRoundKey(std::vector<uint8_t> &state, int iter)
{
    for(int i=0;i<16;i++)
        state[i] ^= this->_expandedKey[iter*16+i] ;
}

void AES::_SubBytes(std::vector<uint8_t> &state)
{
    for(int i=0;i<16;i++)
        state[i] = this->_SBox[state[i]] ;
}

void AES::_ShiftRows(std::vector<uint8_t> &state)
{
    for(int row=0;row<4;row++)
    {
        for(int i=0;i<row;i++)
        {
            uint8_t tmp = state[row] ;
            for(int j=0;j<3;j++)
                state[row+j*4] = state[row+j*4+4] ;
            state[row+12] = tmp ;
        }
    }
}

uint8_t AES::_GF8Mul(uint8_t a, int b)
{
    if(b==1) return a ;

    uint8_t res = (((b&1)==1)?a:0) ;
    bool isXor = ((a&0x80)>0) ;
    res ^= (a<<1) ;
    if(isXor) res ^= 0x1b ;

    return res ;
}

void AES::_MixColumns(std::vector<uint8_t> &state)
{
    static const int mat[4][4] = 
    {
        { 2, 3, 1, 1 },
        { 1, 2, 3, 1 },
        { 1, 1, 2, 3 },
        { 3, 1, 1, 2 }
    } ;

    uint8_t tmp[4] ;

    for(int col=0;col<4;col++)
    {
        for(int i=0;i<4;i++) tmp[i] = state[col*4+i] ;
        for(int i=0;i<4;i++)
        {
            state[col*4+i] = 0 ;
            for(int j=0;j<4;j++)
                state[col*4+i] ^= this->_GF8Mul(tmp[j], mat[i][j]) ;
        }
    }
}

std::vector<uint8_t> AES::Encode(std::vector<uint8_t> plain) throw(std::string)
{
    if(plain.size()!=16)
        throw(std::string("Wrong Block Size.")) ;

    std::vector<uint8_t> cipher = plain ;

    this->_AddRoundKey(cipher, 0) ;

    int nR = 0 ;
    switch(this->_keySize)
    {
        case 16:
            nR = 10 ;
            break ;
        case 24:
            nR = 12 ;
            break ;
        case 32:
            nR = 14 ;
            break ;
    }

    for(int i=1;i<nR;i++)
    {
        this->_SubBytes(cipher) ;
        this->_ShiftRows(cipher) ;
        this->_MixColumns(cipher) ;
        this->_AddRoundKey(cipher, i) ;
    }

    this->_SubBytes(cipher) ;
    this->_ShiftRows(cipher) ;
    this->_AddRoundKey(cipher, nR) ;

    return cipher ;
}

std::vector<uint8_t> AES::Decode(std::vector<uint8_t> cipher) throw(std::string)
{
    throw(std::string("Not implemented.")) ;

    //XXX: not going to implement cause not used in the project

    return cipher ;
}

void AES::_KeyCore(uint8_t *w, int iter)
{
    //rotate
    uint8_t tmp = w[0] ;
    w[0] = w[1] ;
    w[1] = w[2] ;
    w[2] = w[3] ;
    w[3] = tmp ;

    for(int i=0;i<4;i++)
        w[i] = this->_SBox[w[i]] ;

    w[0] = w[0]^this->_Rcon[iter] ;
}

void AES::_KeyExpansion(std::vector<uint8_t> key) throw(std::string)
{
    int expandedSize = 0 ;

    switch(key.size())
    {
        case 16:
            this->_keySize = 16 ;
            expandedSize = 176 ;
            break ;
        case 24:
            this->_keySize = 24 ;
            expandedSize = 208 ;
            break ;
        case 32:
            this->_keySize = 32 ;
            expandedSize = 240 ;
            break ;
        default:
            throw(std::string("Wrong key size.")) ;
            break ;
    }

    this->_expandedKey = key ;

    for(int i=1;(int)this->_expandedKey.size()<expandedSize;i++)
    {
        uint8_t tmp[4] ;
        for(int j=0;j<4;j++)
            tmp[j] = this->_expandedKey[this->_expandedKey.size()-4+j] ;

        this->_KeyCore(tmp, i) ;
        for(int j=0;j<4;j++)
            this->_expandedKey.push_back(tmp[j]^this->_expandedKey[this->_expandedKey.size()-this->_keySize]) ;

        for(int j=0;j<3;j++)
            for(int k=0;k<4;k++)
                this->_expandedKey.push_back(this->_expandedKey[this->_expandedKey.size()-4]^this->_expandedKey[this->_expandedKey.size()-this->_keySize]) ;

        if(this->_keySize==32)
        {
            for(int j=0;j<4;j++)
                tmp[j] = this->_expandedKey[this->_expandedKey.size()-4+j] ;
            for(int j=0;j<4;j++) tmp[j] = this->_SBox[tmp[j]] ;
            for(int j=0;j<4;j++)
                this->_expandedKey.push_back(tmp[j]^this->_expandedKey[this->_expandedKey.size()-this->_keySize]) ;
        }

        if(this->_keySize==24 || this->_keySize==32)
            for(int j=0;j<2;j++)
                for(int k=0;k<4;k++)
                    this->_expandedKey.push_back(this->_expandedKey[this->_expandedKey.size()-4]^this->_expandedKey[this->_expandedKey.size()-this->_keySize]) ;

        if(this->_keySize==32)
            for(int k=0;k<4;k++)
                this->_expandedKey.push_back(this->_expandedKey[this->_expandedKey.size()-4]^this->_expandedKey[this->_expandedKey.size()-this->_keySize]) ;
    }
}

