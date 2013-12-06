//----------------------------------------------------------------------------
//    프로그램명 	: I2C HW 관련 헤더
//
//    만든이     	: 
//
//    날  짜     	:
//    
//    최종 수정  	:
//
//    MPU_Type		:
//
//    파일명     	: Hw_I2C_HW.h
//----------------------------------------------------------------------------
                                                                                                 
#ifndef __HW_I2C_HW_H__
#define __HW_I2C_HW_H__


#ifdef   HW_I2C_HW_LOCAL
#define  EXT_HW_I2C_HW_DEF
#else
#define  EXT_HW_I2C_HW_DEF     extern
#endif



#include "Hw.h"


#define HW_I2C_ERR_NONE		0
#define HW_I2C_ERR_TIMEOUT	1



EXT_HW_I2C_HW_DEF void  Hw_I2C_HW_Init( void );

EXT_HW_I2C_HW_DEF u16   Hw_I2C_HW_Write( u8 Ch, u16 i2c_addr, u8* pdata, u16 len );
EXT_HW_I2C_HW_DEF u16   Hw_I2C_HW_Read ( u8 Ch, u16 i2c_addr, u8 *pdata, u16 len );


#endif
