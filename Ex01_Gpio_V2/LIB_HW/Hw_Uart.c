//----------------------------------------------------------------------------
//    프로그램명 	: Uart 관련 함수
//
//    만든이     	: 
//
//    날  짜     	: 
//    
//    최종 수정  	: 
//
//    MPU_Type		:
//
//    파일명     	: Hw_Uart.h
//----------------------------------------------------------------------------




//----- 헤더파일 열기
//
#define  HW_UART_LOCAL

#include "Hw_Uart.h"



//-- 내부 선언
//



//-- 내부 변수
//



//-- 내부 함수
//






/*---------------------------------------------------------------------------
     TITLE   : Hw_Uart_Init
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_Uart_Init( void )
{
	//-- USART1 설정
	//
	REG_USART1_CR1  = 0 
					| ( 1 << 13 )		// USART Enable
					| ( 0 << 12 )		// 1 Start bit, 8 Data bits, n Stop bit
					| ( 0 << 10 )		// Parity control disabled
					| ( 1 <<  3 )		// Transmitter Enable
					| ( 1 <<  2 );		// Receiver Enable					
	REG_USART1_CR2  = ( 0 << 12 );		// 1 stop bit	
	REG_USART1_CR3  = 0;
	
	
	//-- 115200bps로 통신 속도 설정
	//
	REG_USART1_BRR  = 0
					| ( 39 << 4 )		// DIV_Mantissa
					| (  1 << 0 );		// DIV Fraction
	
	
	REG_USART1_CR1 |= ( 1 << 13 );	    // USART Enable
}






//------------------------------- printf 포팅용 --------------------------------
//
void print_byte(unsigned int c)
{
    if (c == '\n') print_byte('\r');
    while( !((*(volatile unsigned long *) 0x40013800) & 0x80) );
    *(volatile unsigned long *) 0x40013804 = c;
}


char get_byte(void)
{
	while( !((*(volatile unsigned long *) 0x40013800) & 0x20) );

	return (char) *(volatile unsigned long *) 0x40013804;    
}






