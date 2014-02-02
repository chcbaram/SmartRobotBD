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
//    파일명     	: Hw_MultiIMU.h
//----------------------------------------------------------------------------
                                                                                                 
#ifndef __HW_MULTIIMU_H__
#define __HW_MULTIIMU_H__


#ifdef   HW_MULTIIMU_LOCAL
#define  EXT_HW_MULTIIMU_DEF
#else
#define  EXT_HW_MULTIIMU_DEF     extern
#endif



#include "Hw.h"





EXT_HW_MULTIIMU_DEF void Hw_MultiIMU_Init( void );




#endif
