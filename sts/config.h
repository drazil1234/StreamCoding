
#if defined(__cplusplus)
extern "C" {
#endif

#ifndef _CONFIG_H_
#define	_CONFIG_H_

//#define	WINDOWS32
//#define	PROTOTYPES
//#define	LITTLE_ENDIAN
//sort out endian
#if !defined(LITTLE_ENDIAN) && !defined(BIG_ENDIAN) && defined(__BYTE_ORDER__)
  #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    #define BIG_ENDIAN
  #elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    #define LITTLE_ENDIAN
  #endif
#endif

#if !defined(LITTLE_ENDIAN) && !defined(BIG_ENDIAN)
  #if defined(__BIG_ENDIAN__)
    #define BIG_ENDIAN
  #elif defined(__LITTLE_ENDIAN__)
    #define LITTLE_ENDIAN
  #endif
#endif

#if !defined(LITTLE_ENDIAN) && !defined(BIG_ENDIAN)
  #error endian check failed, please define it manually.
#endif

//#define	LOWHI

/*
 * AUTO DEFINES (DON'T TOUCH!)
 */

#ifndef	CSTRTD
  typedef char *CSTRTD;
#endif
#ifndef	BSTRTD
  typedef unsigned char *BSTRTD;
#endif

#ifndef	BYTE
  typedef unsigned char BYTE;
#endif
#ifndef	UINT
  typedef unsigned int UINT;
#endif
#ifndef	USHORT
  typedef unsigned short USHORT;
#endif
#ifndef	ULONG
  typedef unsigned long ULONG;
#endif
#ifndef	DIGIT
  typedef USHORT DIGIT;	/* 16-bit word */
#endif
#ifndef	DBLWORD
  typedef ULONG DBLWORD;  /* 32-bit word */
#endif

#ifndef	WORD64
  typedef ULONG WORD64[2];  /* 64-bit word */
#endif

#endif /* _CONFIG_H_ */

#if defined(__cplusplus)
}
#endif
