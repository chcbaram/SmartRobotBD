//----------------------------------------------------------------------------
//    ���α׷��� 	: ISR ���� �Լ�
//
//    ������     	: 
//
//    ��  ¥     	: 
//    
//    ���� ����  	:
//
//    MPU_Type		:
//
//    ���ϸ�     	: Hw_ISR.c
//----------------------------------------------------------------------------


//----- ������� ����
//
#define  HW_ISR_LOCAL


#include "Hw_ISR.h"


//-- ���ͷ�Ʈ ���ʹ� Offset�� 128 word ������ ���ĵǾ����( 512 bytes ���� )
//
volatile u32 Hw_ISR_VectorTable[100] __attribute__((aligned(512)));




/*---------------------------------------------------------------------------
     TITLE   : Hw_ISR_Init
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_ISR_Init( void )
{
	//-- ���ͷ�Ʈ ���͸� 0x00000000 ���� RAM �������� �̵�
	//   ���ͷ�Ʈ �Լ��� �������� ���� ������� ����
	//
	REG_SCB_VTOR = (u32)Hw_ISR_VectorTable;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_ISR_SetIRQFunc
     WORK    :
     ARG     : 
     			u8  ISR_Num      : ���ͷ�Ʈ ��ȣ
     			u32 FuncAddress  : ���ͷ�Ʈ �߻��� �����ϰ��� �ϴ� �Լ� �ּ�
     			u8  ISR_Priority : ���ͷ�Ʈ �켱����(�������� �켱���� ����)
     RET     : void
---------------------------------------------------------------------------*/
void Hw_ISR_SetIRQFunc( u8 ISR_Num, u32 FuncAddress, u8 ISR_Priority )
{	
	u32 ISR_StartOffset = 16;
	
	Hw_ISR_VectorTable[ ISR_StartOffset + ISR_Num ] = FuncAddress;	
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_ISR_SetIRQFuncByAddr
     WORK    :
     ARG     : 
     			u8  ISR_Addr     : ���ͷ�Ʈ �ּ�
     			u32 FuncAddress  : ���ͷ�Ʈ �߻��� �����ϰ��� �ϴ� �Լ� �ּ�
     			u8  ISR_Priority : ���ͷ�Ʈ �켱����(�������� �켱���� ����)
     RET     : void
---------------------------------------------------------------------------*/
void Hw_ISR_SetIRQFuncByAddr( u32 ISR_Addr, u32 FuncAddress, u8 ISR_Priority )
{	
	u32 ISR_StartOffset = ISR_Addr/4;
	
	Hw_ISR_VectorTable[ ISR_StartOffset ] = FuncAddress;	
}
 
