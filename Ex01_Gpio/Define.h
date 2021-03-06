//----------------------------------------------------------------------------
//    프로그램명 	:
//
//    만든이     	:
//
//    날  짜     	:
//
//    최종 수정  	:
//
//    MPU_Type		:
//
//    파일명     	: Define.h
//----------------------------------------------------------------------------


#ifndef _DEFINE_H_
#define _DEFINE_H_



#define SWAP8(A)		(A)
#define SWAP16(A)		((((A)&0x00ff)<<8) | ((A)>>8))
#define SWAP32(A)		((((A)&0x000000ff)<<24) | (((A)&0x0000ff00)<<8) | (((A)&0x00ff0000)>>8) | (((A)&0xff000000)>>24))

typedef int	bool;

#define	true			1
#define false			0


#endif
