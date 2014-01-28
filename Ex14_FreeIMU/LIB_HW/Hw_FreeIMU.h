//----------------------------------------------------------------------------
//    프로그램명 	: IMU 센서 관련 헤더
//
//    만든이     	: Cho Han Cheol(Baram)
//
//    날  짜     	:
//    
//    최종 수정  	:
//
//    MPU_Type	:
//
//    파일명     	: Hw_IMU.h
//----------------------------------------------------------------------------
                                                                                                 
#ifndef __HW_FREEIMU_H__
#define __HW_FREEIMU_H__


#ifdef   HW_FREEIMU_LOCAL
#define  EXT_HW_FREEIMU_DEF
#else
#define  EXT_HW_FREEIMU_DEF     extern
#endif



#include "Hw.h"





EXT_HW_FREEIMU_DEF void Hw_FreeIMU_Init( void );

EXT_HW_FREEIMU_DEF void Hw_FreeIMU_GetUpdateQ( float * q, s16 *raw_value, float dt );
EXT_HW_FREEIMU_DEF void Hw_FreeIMU_SetRawValues( s16 *raw_values );
EXT_HW_FREEIMU_DEF void Hw_FreeIMU_GetYawPitchRoll( float *q, float * ypr) ;




#endif
