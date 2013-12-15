//----------------------------------------------------------------------------
//    프로그램명 	: I2C 관련 헤더
//
//    만든이     	: 
//
//    날  짜     	:
//    
//    최종 수정  	:
//
//    MPU_Type		:
//
//    파일명     	: Hw_I2C.h
//----------------------------------------------------------------------------
                                                                                                 
#ifndef __HW_I2C_H__
#define __HW_I2C_H__


#ifdef   HW_I2C_LOCAL
#define  EXT_HW_I2C_DEF
#else
#define  EXT_HW_I2C_DEF     extern
#endif



#include "Hw.h"
#include "Hw_I2C_SW.h"
#include "Hw_I2C_HW.h"


#define HW_I2C_USE_SW_I2C	0


#define HW_I2C_ERR_NONE		0
#define HW_I2C_ERR_TIMEOUT	1
#define HW_I2C_ERR_START	2
#define HW_I2C_ERR_ADDR		3
#define HW_I2C_ERR_TXE		4
#define HW_I2C_ERR_RXE		5
#define HW_I2C_ERR_STOP		6






EXT_HW_I2C_DEF void Hw_I2C_Init( void );

EXT_HW_I2C_DEF u16  Hw_I2C_Write( u8 Ch, u16 i2c_addr, u8 *pdata, u16 len );
EXT_HW_I2C_DEF u16  Hw_I2C_Read ( u8 Ch, u16 i2c_addr, u8 *pdata, u16 len );


#endif
