/******************************************************************************
 * @file      start_stm32f10x.c
 * @author    SmartRobot : e2g1234@naver.com
 * @version   V0.1
 * @date      04/15/2013
 *******************************************************************************/

//----- 헤더파일 열기
//
#define  HW_STARTUP_LOCAL

#include "Hw_StartUp.h"




//-- Private typedef
//
typedef void( *const intfunc )( void );


//-- init value for the stack pointer. defined in linker script 
//
extern unsigned long _estack;


//-- Private function prototypes
//
void Hw_StartUp_Init(void) __attribute__((__interrupt__));


__attribute__ ((section(".isr_vectorsflash")))    //linker scriptor에서 사용됨
//-- vector table
//
void (* g_pfnVectors[])(void) =
{
    (intfunc)((unsigned long)&_estack),		// The stack pointer after relocation
    Hw_StartUp_Init,                      	// Reset Handler
};



extern int main();


/*---------------------------------------------------------------------------
     TITLE   : 	Hw_StartUp_Init
     WORK    :	 
     			보드 리셋시에 처음 실행되는 함수
     ARG     : 	void
     RET     : 	void
---------------------------------------------------------------------------*/
void Hw_StartUp_Init(void)
{
	
	u32 TimeOutCnt  = 0;
	u32 HSEStatus 	= 0;
	
    // HSI or HSE 선택
    
        
    //-- CR_HSEON_Set
    //
	REG_RCC_CR |= (1<<16);	
		
	//-- 설정된 clock 이 설정 되었는지 확인
	//
	do
	{	
		HSEStatus = REG_RCC_CR & (1<<17);	// HSE 인경우
		TimeOutCnt++;		
	} while( (HSEStatus == 0) && (TimeOutCnt < 1280) );
  

	//-- FLASH Access 타이밍 설정
	//
	REG_FLASH_ACR |=  (1<<4);	// Prefetch buffer enable
	REG_FLASH_ACR &= ~(3<<0);	// LATENCY Clear
	REG_FLASH_ACR |=  (2<<0);	// LATENCY = two wait


    main();

}
