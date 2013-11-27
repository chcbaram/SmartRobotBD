//----------------------------------------------------------------------------
//    ���α׷��� 	: LED ���� �Լ�
//
//    ������     	: 
//
//    ��  ¥     	: 
//    
//    ���� ����  	:
//
//    MPU_Type		:
//
//    ���ϸ�     	: Hw_Led.c
//----------------------------------------------------------------------------


//----- ������� ����
//
#define  HW_LED_LOCAL


#include "Hw_Led.h"





/*---------------------------------------------------------------------------
     TITLE   : Hw_Led_Init
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_Led_Init( void )
{
	u32 PinNum;

	//-- JTAG ���� Disable�ϰ� PB3,4�� GPIO�� ����Ѵ�.(�ɱ����� ���� ������� ������ ����)
	//
	REG_AFIO_MAPR &= ~(0x07 << 24);
	REG_AFIO_MAPR |=  (0x04 << 24);
	
	PinNum = 3;
	REG_GPIOB_CRL &= ~(0x0F << (PinNum*4+0));	// Clear
	REG_GPIOB_CRL |=  (0x03 << (PinNum*4+0));	// MODE, PB.3 Output mode, max speed 50Mhz
	REG_GPIOB_CRL |=  (0x00 << (PinNum*4+2));	// CNF,  PB.3 General purpose output push-pul
	
		
	Hw_Led_Off(0);
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Led_On
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_Led_On( u8 Ch )
{	
	switch( Ch )
	{
		case 0:
			CLR_BIT( REG_GPIOB_ODR, 3 );
			break;
	}
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Led_On
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_Led_Off( u8 Ch )
{
	switch( Ch )
	{
		case 0:
			SET_BIT( REG_GPIOB_ODR, 3 );
			break;
	}
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Led_On
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_Led_Toggle( u8 Ch )
{
	switch( Ch )
	{
		case 0:
			TGL_BIT( REG_GPIOB_ODR, 3 );
			break;
	}
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Led_Wait
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_Led_Wait( u32 delay )
{
    volatile u32 i;
    for ( i = 0 ; i < delay ; i++ ){ };
}



















 
