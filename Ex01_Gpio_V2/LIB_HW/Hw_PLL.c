//----------------------------------------------------------------------------
//    ���α׷��� 	: PLL ���� �Լ�
//
//    ������     	: 
//
//    ��  ¥     	: 2013. 8.20.
//    
//    ���� ����  	:
//
//    MPU_Type		:
//
//    ���ϸ�     	: Hw_PLL.c
//----------------------------------------------------------------------------


//----- ������� ����
//
#define  HW_PLL_LOCAL


#include "Hw_PLL.h"



#define CLOCK_EXT		1		// Ŭ���� �ܺη� ����ϸ� 1



/*---------------------------------------------------------------------------
     TITLE   : Hw_PLL_Init
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_PLL_Init( void )
{
	
#if	CLOCK_EXT == 1
	//-- PLL ���� 72Mhz�� ������
	//
	REG_RCC_CFGR &= ~(0xF<<18 | 0x1<<17 | 0x1<<16);
	REG_RCC_CFGR |=  (0x7<<18 | 0x1<<16);			// 8MHz
    REG_RCC_CR   |=  0x01 << 24;                    // PLLON

	while( (REG_RCC_CR & (1<<25)) == 0 );			// PLLRDY

#else	
	//-- PLL ���� ���� 36MHz ������
	//
	REG_RCC_CFGR |= (0x7<<18);              		// ���� 4MHz
	REG_RCC_CFGR |= (0x4<<18 | 0x1<<16);			// 12MHz
	REG_RCC_CR   |=  0x01<<24;						// PLLON
	while( (REG_RCC_CR & (1<<25)) == 0 );			// PLLRDY
#endif

    
    /////PLL�� �����Ѵ�.
    /////Bits 1:0 SW : System clock switch
    //00: HSI selected as system clock
    //01: HSE selected as system clock
    //10: PLL selected as system clock   <----------------- ���õ�
    //11: not allowed
	REG_RCC_CFGR &= ~0x3;
	REG_RCC_CFGR |=  0x2;
    
    while( (REG_RCC_CFGR & 0xC) != 0x08 );
    
    //APB2�� �ִ� UART �� �ֺ���ġ�� Clock �ҽ� ��������
    //UART�� ���۵Ǳ� ���ؼ��� GPIO�� Alternation Function IO�� ���ۻ��¿� �־�� ��
	REG_RCC_APB2ENR |= 0x1 << 14 | 0x1 << 4  | 0x1 << 3  | 0x1 << 2 | 0x1 << 0;         // UART/ IOPC EN / IOPB EN / IOPA EN / AFIO EN	
}