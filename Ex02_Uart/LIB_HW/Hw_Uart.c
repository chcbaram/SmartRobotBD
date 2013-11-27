//----------------------------------------------------------------------------
//    ���α׷��� 	: Uart ���� �Լ�
//
//    ������     	: 
//
//    ��  ¥     	: 
//    
//    ���� ����  	: 
//
//    MPU_Type		:
//
//    ���ϸ�     	: Hw_Uart.h
//----------------------------------------------------------------------------




//----- ������� ����
//
#define  HW_UART_LOCAL

#include "Hw_Uart.h"
#include "Hw_Uart_Q.h"

#include <stdarg.h >


//-- ���� ����
//



//-- ���� ����
//
HW_UART_OBJ Hw_Uart_Ch[HW_UART_MAX_CH];


//-- ���� �Լ�
//
void Hw_Uart_Open_COM1( u32 BaudData, void (*ISR_FuncPtr)(char Ch) );




//-- UART1 ISR
//
void Hw_ISR_UART1_Handler(void)
{
	u8 UartData;
				
	UartData = REG_USART1_DR;
	
	Hw_Uart_Q_Push( 0, &UartData );
}




/*---------------------------------------------------------------------------
     TITLE   : Hw_Uart_Init
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_Uart_Init( void )
{
	u32 i;
	
	Hw_Uart_Q_Init();
	
	
	for( i=0; i<HW_UART_MAX_CH; i++ )
	{
		Hw_Uart_Ch[i].Baud 		  = 0;
		Hw_Uart_Ch[i].ISR_FuncPtr = NULL;
	}	
	
	
	Hw_Uart_Open( HW_UART_COM1, 115200, NULL );
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Uart_Open
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_Uart_Open( u8 Ch, u32 BaudData, void (*ISR_FuncPtr)(char Ch) )
{	
	switch( Ch )
	{
		case HW_UART_COM1:
			Hw_Uart_Open_COM1( BaudData, ISR_FuncPtr );
			break;
			
		case HW_UART_COM2:
			break;
			
		case HW_UART_COM3:
			break;

		case HW_UART_COM4:
			break;

		case HW_UART_COM5:
			break;			
	}
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Uart_Open_COM1
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_Uart_Open_COM1( u32 BaudData, void (*ISR_FuncPtr)(char Ch) )
{	

	Hw_Uart_Ch[HW_UART_COM1].Baud 		 = BaudData;
	Hw_Uart_Ch[HW_UART_COM1].ISR_FuncPtr = ISR_FuncPtr;
	
	
	//-- Clock Enable
	//
	SET_BIT( REG_RCC_APB2ENR, 14 );		// USART1 Clock Enable
	
	
	//-- I/O Port ����
	//
	
	
	//-- USART1 ����
	//	
	REG_USART1_CR1  = 0 
					| ( 0 << 12 )		// 1 Start bit, 8 Data bits, n Stop bit
					| ( 0 << 10 )		// Parity control disabled
					| ( 1 <<  3 )		// Transmitter Enable
					| ( 1 <<  2 );		// Receiver Enable					
	REG_USART1_CR2  = ( 0 << 12 );		// 1 stop bit	
	REG_USART1_CR3  = 0;
	
	
	//-- 115200bps�� ��� �ӵ� ����
	//
	REG_USART1_BRR  = 0
					| ( 39 << 4 )		// DIV_Mantissa
					| (  1 << 0 );		// DIV Fraction
	

	//-- ���ͷ�Ʈ ����
	//
	SET_BIT( REG_NVIC_ISER( 37/32 ), 37-32 );	// NVIC 37�� USART1 ���ͷ�Ʈ Ȱ��ȭ
	SET_BIT( REG_USART1_CR1, 5 );				// RX ���ͷ�Ʈ Ȱ��ȭ
	
	Hw_ISR_SetIRQFunc( 37, (u32)Hw_ISR_UART1_Handler, 0 );
	
	REG_USART1_CR1 |= ( 1 << 13 );	    // USART Enable
}





/*---------------------------------------------------------------------------
     TITLE	: Uart_Getch
     WORK
			: �ø��� ��Ʈ���� ���� 1����Ʈ �д´�.
     ARG
     RET
---------------------------------------------------------------------------*/
u8 Hw_Uart_Getch( u8 Ch )
{
	u8 Uart_GetData = 0;

	while( 1 )
	{
		// ���ۿ� ������ ���ö����� ��ٸ���.
		if( HW_UART_Q_VAILD(Ch) > 0 ) break;
	}

	Hw_Uart_Q_Pop( Ch, &Uart_GetData );

	return Uart_GetData;
}





/*---------------------------------------------------------------------------
     TITLE	: Uart_Putch
     WORK
			: �ø��� ��Ʈ�� ���� 1����Ʈ ����
     ARG
     RET
---------------------------------------------------------------------------*/
void Hw_Uart_Putch( u8 Ch,  char Uart_PutData )
{
    while( !(REG_USART1_SR & 0x80) );
    
    REG_USART1_DR = Uart_PutData;
}





/*---------------------------------------------------------------------------
     TITLE	: Uart_PrintEx
     WORK
     ARG
     RET
---------------------------------------------------------------------------*/
void Hw_Uart_PrintEx( u8 Ch, char *UartPrintData )
{
	while( *UartPrintData != '\0' )
	{
		Hw_Uart_Putch( Ch, *UartPrintData );
		UartPrintData++;
	}        
}




/*---------------------------------------------------------------------------
     TITLE	: Uart_Print
     WORK
     ARG
     RET
---------------------------------------------------------------------------*/
void Hw_Uart_Print( u8 Ch,  char *format, ... )
{
	char Str[200];
	va_list ap;
	
	va_start( ap, format );

	vsprintf( Str, format, ap );	

	va_end(ap);	

	Hw_Uart_PrintEx( Ch, Str );
}




/*---------------------------------------------------------------------------
     TITLE	: Uart_GetchNoWait
     WORK
     ARG
     RET
---------------------------------------------------------------------------*/
u8 Hw_Uart_GetchNoWait( u8 Ch, u8 *cReturn )
{
	// ���ۿ� ������ ������ ���з� ����.
	if( HW_UART_Q_VAILD(Ch) == 0 )
	{
		return FALSE;
	}
		   
	Hw_Uart_Q_Pop( Ch, cReturn );
	
	return TRUE;
}







//------------------------------- printf ���ÿ� --------------------------------
//
void print_byte(unsigned int c)
{
	if (c == '\n') Hw_Uart_Putch(HW_UART_COM1, '\r');
    
    Hw_Uart_Putch(HW_UART_COM1, c);
}


char get_byte(void)
{
	char Ch;
	
	Ch = Hw_Uart_Getch(HW_UART_COM1);
	
	return Ch;
}






