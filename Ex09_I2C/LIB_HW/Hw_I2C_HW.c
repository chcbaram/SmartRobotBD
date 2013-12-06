//----------------------------------------------------------------------------
//    프로그램명 	: I2C HW 관련 함수
//
//    만든이     	: 
//
//    날  짜     	: 
//    
//    최종 수정  	:
//
//    MPU_Type		:
//
//    파일명     	: Hw_I2C_GW.c
//----------------------------------------------------------------------------


//----- 헤더파일 열기
//
#define  HW_I2C_HW_LOCAL


#include "Hw_I2C_HW.h"
#include "Lb_Printf.h"


#define HW_I2C_HW_CH_MAX  			2






void Hw_I2C_HW_IO_Setup( void );
u16  Hw_I2C_HW_WaitForBitSet( u8 Ch, u32 *reg_addr, u32 reg_bit, u32 time_out );

/*
void Hw_I2C_SW_StartTransmit( u8 Ch );
void Hw_I2C_SW_EndTransmit  ( u8 Ch );


u8 Hw_I2C_SW_WriteByte_NoAck( u8 Ch, u8   Data );
u8 Hw_I2C_SW_WriteByte_Ack  ( u8 Ch, u8   Data );
u8 Hw_I2C_SW_ReadByte_NoAck ( u8 Ch, u8 *pData );
u8 Hw_I2C_SW_ReadByte_Ack   ( u8 Ch, u8 *pData );
*/





/*---------------------------------------------------------------------------
     TITLE   : Hw_I2C_SW_Init
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_I2C_HW_Init( void )
{
	u32 SR;


	//-- I/O 초기화
	//
	Hw_I2C_HW_IO_Setup();	


	REG_I2C1_CR1 = 0;

	//-- 1. I2C 클럭 입력 주파수 설정 
	//
	REG_I2C1_CR2 = 0 
				| ( 36 << 0 );		// FREQ = 36Mhz

	//-- 2. Clock Control 
	//
	REG_I2C1_CCR = 0 
				| (  1 << 15 )		// Fast Mode I2C
				| (  0 << 14 )		// Duty Tlow/Thig = 2
				| ( 30 <<  0 );		// 30*27ns + 2*30*27ns = 400Khz

	//-- 3. Maximum Rise Time  
	//
	REG_I2C1_TRISE = 4;				// 300ns / 100ns + 1 = 4 


	//-- Clock Enable
	//
	REG_RCC_APB1ENR |= (1<<21);		// I2C1 Clock enable


	CLR_BIT( REG_I2C1_CR1, 1 );		// I2C Mode		
	SET_BIT( REG_I2C1_CR1, 0 );		// Peripheral enable	

	SR = REG_I2C1_SR1;
	SR = REG_I2C1_SR2;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_I2C_SW_IO_Setup
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_I2C_HW_IO_Setup( void )
{

	//-- I2C1 Remap
	//
	SET_BIT( REG_AFIO_MAPR, 1 );	// SCL/PB8, SDA/PB9)


	//-- SCL PB.8
	//
	REG_GPIOB_CRH &= ~(0x0F << ( 0));	// Clear
	REG_GPIOB_CRH |=  (0x03 << ( 0));	// MODE, PB.8 Output mode, max speed 50Mhz
	REG_GPIOB_CRH |=  (0x03 << ( 2));	// CNF,  PB.8 Alternate function output Open-drain	


	//-- SDA PB.9
	//
	REG_GPIOB_CRH &= ~(0x0F << ( 4));	// Clear
	REG_GPIOB_CRH |=  (0x03 << ( 4));	// MODE, PB.9 Output mode, max speed 50Mhz
	REG_GPIOB_CRH |=  (0x03 << ( 6));	// CNF,  PB.9 Alternate function output Open-drain	
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_I2C_SW_Write
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_I2C_HW_Write( u8 Ch, u16 i2c_addr, u8* pdata, u16 len )
{
    u16 Ret = 0;;
	u8  i;
	u32 TimeOut;
	u32 SR1;
	u32 SR2;


	switch( Ch )
	{
		case 0:
			SET_BIT( REG_I2C1_CR1, 8 );	// START

			Lb_printf("S0\n" );

			Ret = Hw_I2C_HW_WaitForBitSet( Ch, (u32 *)&REG_I2C1_SR1, 0, 10000 );
			if( Ret != 0 ) break;

			Lb_printf("S1 %x\n", REG_I2C1_SR1);
			Lb_printf("S1 %x\n", REG_I2C1_SR2);


			REG_I2C1_DR = i2c_addr;		// ADDR Write 

			Ret = Hw_I2C_HW_WaitForBitSet( Ch, (u32 *)&REG_I2C1_SR1, 1, 10000 );
			if( Ret != 0 ) break;

			Lb_printf("S2 %x\n", REG_I2C1_SR1);
			Lb_printf("S2 %x\n", REG_I2C1_SR2);


			REG_I2C1_DR = 0x00;		

			Ret = Hw_I2C_HW_WaitForBitSet( Ch, (u32 *)&REG_I2C1_SR1, 7, 10000 );
			if( Ret != 0 ) break;
			Ret = Hw_I2C_HW_WaitForBitSet( Ch, (u32 *)&REG_I2C1_SR1, 2, 10000 );
			if( Ret != 0 ) break;

			Lb_printf("S2 %x\n", REG_I2C1_SR1);
			Lb_printf("S2 %x\n", REG_I2C1_SR2);

			SET_BIT( REG_I2C1_CR1, 9 );	// STOP

			Lb_printf("S3 %x\n", REG_I2C1_SR1);
			Lb_printf("S3 %x\n", REG_I2C1_SR2);


			break;
	}

	

	/*
	Hw_I2C_SW_StartTransmit(Ch);

	Ret |= Hw_I2C_SW_WriteByte_Ack(Ch, (i2c_addr<<1) | (0<<0)); // Address


	for( i=0; i<len; i++ )
	{
		Ret |= Hw_I2C_SW_WriteByte_Ack(Ch, pdata[i]);  			// Data	
	}
	
	Hw_I2C_SW_EndTransmit(Ch);
	*/
    return Ret;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_I2C_HW_Read
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_I2C_HW_Read( u8 Ch, u16 i2c_addr, u8 *pdata, u16 len )
{
	u16 Ret = 0;
	u8  i;
	u8  LastIndex = 0;

	if( len > 0 ) LastIndex = len - 1;

	/*
	Hw_I2C_SW_StartTransmit(Ch);

	Ret |= Hw_I2C_SW_WriteByte_Ack(Ch, (i2c_addr<<1) | (1<<0));  		// Address

	for( i=0; i<len; i++ )
	{
		if( i < LastIndex )
		{
			Ret |= Hw_I2C_SW_ReadByte_Ack(Ch, &pdata[i] );				// Read Data
		}
		else
		{
			Ret |= Hw_I2C_SW_ReadByte_NoAck(Ch, &pdata[i] );			// Read Data 마지막 데이터는 NACK 처리
		}
	}
	
	Hw_I2C_SW_EndTransmit(Ch);
	*/

    return Ret;
}




/*---------------------------------------------------------------------------
     TITLE   : Hw_I2C_HW_WaitForBitSet
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_I2C_HW_WaitForBitSet( u8 Ch, u32 *reg_addr, u32 reg_bit, u32 time_out )
{
	u32 Ret = 0;


	switch( Ch )
	{
		case 0:

			while( time_out-- )
			{
				if( IS_SET_BIT( *reg_addr, reg_bit ) ) break;
			};
			
			if( time_out == 0 ) 
			{
				Ret = HW_I2C_ERR_TIMEOUT;
			}
			break;

	}
	return Ret;
}




#if 0

/*---------------------------------------------------------------------------
     TITLE   : Hw_I2C_SW_StartTransmit
     WORK    : 
     ARG     : 
     			u8 Ch : I2C 채널
     RET     : void
---------------------------------------------------------------------------*/
void Hw_I2C_SW_StartTransmit( u8 Ch )
{
	//-- SDA Rising Edge 생성
	//
								// SCL	SDA
	Hw_I2C_SW_SCL_HIGH( Ch );	//  1    1
	Hw_I2C_SW_SDA_HIGH( Ch );	//  1    1
	Hw_I2C_SW_SCL_HIGH( Ch );	//	1    1
	Hw_I2C_SW_SDA_LOW ( Ch );	//  1    0
	Hw_I2C_SW_SCL_LOW ( Ch );	//  0    0
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_ADT75_EndTransmit
     WORK    : 
     ARG     : 
     			u8 Ch : I2C 채널
     RET     : void
---------------------------------------------------------------------------*/
void Hw_I2C_SW_EndTransmit( u8 Ch )
{
	//-- SDA Falling Edge 생성
	//
								// SCL	SDA   
	Hw_I2C_SW_SDA_LOW ( Ch );	//  0    0
	Hw_I2C_SW_SCL_LOW ( Ch );	//  0    0
	Hw_I2C_SW_SCL_HIGH( Ch );	//  1    0
	Hw_I2C_SW_SDA_HIGH( Ch );	//  1    1
	Hw_I2C_SW_SCL_HIGH( Ch );	//  1    1
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_I2C_SW_WriteByte_NoAck
     WORK    : 
     ARG     : 
     			u8 Ch   : I2C 채널
     			u8 Data : 전송할 바이트
     RET     : 
     			u8 : 에러코드
---------------------------------------------------------------------------*/
u8 Hw_I2C_SW_WriteByte_NoAck( u8 Ch, u8 Data )
{
	u8 i;

	for( i=0; i<8; i++ )
	{
		if( Data & (1<<7) )  Hw_I2C_SW_SDA_HIGH( Ch );
		else                 Hw_I2C_SW_SDA_LOW ( Ch );

		Data <<= 1;

		Hw_I2C_SW_SCL_HIGH( Ch );
		Hw_I2C_SW_SCL_LOW ( Ch );
	}

	Hw_I2C_SW_SCL_HIGH( Ch );
	Hw_I2C_SW_SCL_LOW( Ch );

	return 0;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_ADT75_WriteByte_Ack
     WORK    : 
     ARG     : 
     			u8 Ch   : I2C 채널
     			u8 Data : 전송할 바이트
     RET     : 
     			u8 : 0 -> Ack
---------------------------------------------------------------------------*/
u8 Hw_I2C_SW_WriteByte_Ack( u8 Ch, u8 Data )
{
	u8 i;
	u8 Ack = 1;

	for( i=0; i<8; i++ )
	{
		if( Data & (1<<7) )  Hw_I2C_SW_SDA_HIGH( Ch );
		else                 Hw_I2C_SW_SDA_LOW ( Ch );

		Data <<= 1;

		Hw_I2C_SW_SCL_HIGH( Ch );
		Hw_I2C_SW_SCL_LOW( Ch );
	}


	Hw_I2C_SW_SetDir_SDA_Input( Ch );



	// Ack 검사 
	//
	Hw_I2C_SW_SCL_HIGH( Ch );

	if( Hw_I2C_SW_SDA( Ch ) == 0 ) Ack = 0;

	Hw_I2C_SW_SCL_LOW( Ch );


	Hw_I2C_SW_SetDir_SDA_Output( Ch );

	return Ack;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_I2C_SW_ReadByte_NoAck
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u8 Hw_I2C_SW_ReadByte_NoAck( u8 Ch, u8 *pData )
{
	u8 i;
	u8 Data = 0;


	Hw_I2C_SW_SetDir_SDA_Input( Ch );

	for( i=0; i<8; i++ )
	{
		Data <<= 1;

		Hw_I2C_SW_SCL_HIGH( Ch );
		
		if( Hw_I2C_SW_SDA( Ch ) == 1 ) Data |= 1;

		Hw_I2C_SW_SCL_LOW( Ch );
	}


	Hw_I2C_SW_SDA_HIGH( Ch );

	Hw_I2C_SW_SetDir_SDA_Output( Ch );

	Hw_I2C_SW_SCL_HIGH( Ch );
	Hw_I2C_SW_SCL_LOW( Ch );

	*pData = Data;

	return 0;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_I2C_SW_ReadByte_Ack
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u8 Hw_I2C_SW_ReadByte_Ack( u8 Ch, u8 *pData )
{
	u8 i;
	u8 Data = 0;
	u8 Ack = 0;


	Hw_I2C_SW_SetDir_SDA_Input( Ch );

	for( i=0; i<8; i++ )
	{
		Data <<= 1;

		Hw_I2C_SW_SCL_HIGH( Ch );
		
		if( Hw_I2C_SW_SDA( Ch ) == 1 ) Data |= 1;

		Hw_I2C_SW_SCL_LOW( Ch );
	}

	*pData = Data;


	Hw_I2C_SW_SetDir_SDA_Output( Ch );

	// Ack 발생 
	//
	Hw_I2C_SW_SDA_LOW( Ch );

	Hw_I2C_SW_SCL_HIGH( Ch );
	Hw_I2C_SW_SCL_LOW( Ch );


	return Ack;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_I2C_SW_SCL_HIGH
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_I2C_SW_SCL_HIGH( u8 Ch )
{
	Hw_Wait_Usec(HW_I2C_SW_PULSE_DELAY);

	if( Ch == 0 )  
	{
		SET_BIT( REG_GPIOB_ODR, 6 );
	}
	else           
	{
	}

	Hw_Wait_Usec(HW_I2C_SW_PULSE_DELAY);
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_I2C_SW_SCL_LOW
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_I2C_SW_SCL_LOW( u8 Ch )
{
	Hw_Wait_Usec(HW_I2C_SW_PULSE_DELAY);

	if( Ch == 0 )  
	{
		CLR_BIT( REG_GPIOB_ODR, 6 );	
	}
	else           
	{
	}

	Hw_Wait_Usec(HW_I2C_SW_PULSE_DELAY);
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_I2C_SW_SDA_HIGH
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_I2C_SW_SDA_HIGH( u8 Ch )
{
	if( Ch == 0 )
	{
		SET_BIT( REG_GPIOB_ODR, 7 );
	}
	else           
	{
	}
}	





/*---------------------------------------------------------------------------
     TITLE   : Hw_I2C_SW_SDA_LOW
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_I2C_SW_SDA_LOW( u8 Ch )
{
	if( Ch == 0 )  
	{
		CLR_BIT( REG_GPIOB_ODR, 7 );
	}
	else           
	{
	}
}	





/*---------------------------------------------------------------------------
     TITLE   : Hw_I2C_SW_SDA
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u8 Hw_I2C_SW_SDA( u8 Ch )
{
	u8 Data = 0;

	if( Ch == 0 )
	{
		if( REG_GPIOB_IDR & (1<<7) ) Data = 1;
		else						 Data = 0;
	}
	else
	{
	}

	return Data;	
}	





/*---------------------------------------------------------------------------
     TITLE   : Hw_I2C_SW_SetDir_SDA_Input
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_I2C_SW_SetDir_SDA_Input( u8 Ch )
{
	if( Ch == 0 )
	{
		REG_GPIOB_CRL &= ~(0x0F << (28));	// Clear
		REG_GPIOB_CRL |=  (0x00 << (28));	// MODE, PB.7 Input Mode
		//REG_GPIOB_CRL |=  (0x01 << (30));	// CNF,  PB.7 Floating input	
		REG_GPIOB_CRL |=  (0x02 << (30));	// CNF,  PB.7 input	pull-up/pull-down
		
		SET_BIT( REG_GPIOB_ODR, 7 );
	}
	else
	{
	}
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_I2C_SW_SetDir_SDA_Output
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_I2C_SW_SetDir_SDA_Output( u8 Ch )
{
	if( Ch == 0 )
	{
		REG_GPIOB_CRL &= ~(0x0F << (28));	// Clear
		REG_GPIOB_CRL |=  (0x03 << (28));	// MODE, PB.7 Output mode, max speed 50Mhz
		REG_GPIOB_CRL |=  (0x00 << (30));	// CNF,  PB.7 General purpose output push-pul	
	}
	else
	{
	}
}



#endif
