//----------------------------------------------------------------------------
//    프로그램명 	: Util 관련 함수
//
//    만든이     	: 
//
//    날  짜     	: 
//    
//    최종 수정  	: 
//
//    MPU_Type		:
//
//    파일명     	: Lib_Util.h
//----------------------------------------------------------------------------



//----- 헤더파일 열기
//
#define  LIB_UTIL_LOCAL

#include "Lib_Util.h"



//-- 내부 선언
//



//-- 내부 변수
//



//-- 내부 함수
//






void delay(volatile unsigned int timeCount)
{
    while(timeCount --);
}

void delay_second(void)
{
    delay(806596);
}
