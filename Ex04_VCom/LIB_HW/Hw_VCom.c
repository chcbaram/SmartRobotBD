//----------------------------------------------------------------------------
//    ���α׷��� 	: VCom ���� �Լ�
//
//    ������     	: 
//
//    ��  ¥     	: 
//    
//    ���� ����  	: 
//
//    MPU_Type		:
//
//    ���ϸ�     	: Hw_VCom.h
//----------------------------------------------------------------------------




//----- ������� ����
//
#define  HW_VCOM_LOCAL

#include "Hw_VCom.h"

#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"




//-- ���� ����
//



//-- ���� ����
//



//-- ���� �Լ�
//





/*---------------------------------------------------------------------------
     TITLE   : Hw_VCom_Init
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_VCom_Init( void )
{
	Set_System();
	Set_USBClock();
	USB_Interrupts_Config();
	USB_Init();	
}





/*---------------------------------------------------------------------------
     TITLE	: Hw_VCom_Getch
     WORK
			: �ø��� ��Ʈ���� ���� 1����Ʈ �д´�.
     ARG
     RET
---------------------------------------------------------------------------*/
u8 Hw_VCom_Getch( void )
{
	u8 Uart_GetData = 0;

	while( 1 )
	{
		// ���ۿ� ������ ���ö����� ��ٸ���.
		if( HW_UART_Q_VAILD(5) > 0 ) break;
	}

	Hw_Uart_Q_Pop( 5, &Uart_GetData );

	return Uart_GetData;
}





extern void USB_Send_Data( u8 SendData );

/*---------------------------------------------------------------------------
     TITLE	: Hw_VCom_Putch
     WORK
			: �ø��� ��Ʈ�� ���� 1����Ʈ ����
     ARG
     RET
---------------------------------------------------------------------------*/
void Hw_VCom_Putch( char Uart_PutData )
{
	USB_Send_Data( Uart_PutData );
}