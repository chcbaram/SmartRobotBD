//----------------------------------------------------------------------------
//    프로그램명 	: HMC5883 지자계 센서 관련 함수
//
//    만든이     	: Cho Han Cheol(Baram)
//
//    날  짜     	: 
//    
//    최종 수정  	:
//
//    MPU_Type	:
//
//    파일명     	: Hw_HMC5883.c
//----------------------------------------------------------------------------


//----- 헤더파일 열기
//
#define  HW_HMC5883_LOCAL


#include "Hw_HMC5883.h"
#include "Lb_Printf.h"



float x_scale,y_scale,z_scale,x_max,y_max,z_max;



u8 Hw_HMC5883_EnableFlag = FALSE;



typedef struct
{

	u8	X_Compass_H;
	u8	X_Compass_L;
	u8	Z_Compass_H;
	u8	Z_Compass_L;
	u8	Y_Compass_H;
	u8	Y_Compass_L;

} HW_HMC5883_RAW_DATA_OBJ;




void Hw_HMC5883_Calibrate( u8  gain);




/*---------------------------------------------------------------------------
     TITLE   : Hw_HMC5883_Init
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_HMC5883_Init( void )
{
	u16 Err = 0;
	u8  RegData[20];

	static u8 power_on = FALSE;


	if( power_on == TRUE && Hw_HMC5883_EnableFlag == TRUE ) return TRUE;


	Hw_HMC5883_EnableFlag = FALSE;

	Hw_Wait_ms(10);


	//-- I2C 초기화
	//
	Hw_I2C_IMU_Init();	



	if( Hw_HMC5883_CheckID() == FALSE )
	{
		Lb_printf("HMC5883 Fail to read ID \n");
		Err = HW_I2C_ERR_CHECK_ID;
	}
	else
	{
		Err = Hw_I2C_IMU_HMC5883_ReadRegs( HW_HMC5883_REG_CFG_A, RegData, 3 );

		if( Err == 0 )
		{

			Hw_I2C_IMU_HMC5883_WriteReg( HW_HMC5883_REG_MODE, 0 );


			//-- 8 평균 데이터 사용
			//
			SET_BIT( RegData[0], 6 );	// MA1 = 1
			SET_BIT( RegData[0], 5 );	// MA0 = 1

			//-- 75Hz로 출력 설정
			//
			SET_BIT( RegData[0], 4 );	// DO2 = 1
			SET_BIT( RegData[0], 3 );	// DO1 = 1
			CLR_BIT( RegData[0], 2 );	// DO0 = 0


			RegData[1] = 0;


			//-- Continuous 모드로 변경
			//
			CLR_BIT( RegData[2], 1 );	// MD1 = 0
			CLR_BIT( RegData[2], 0 );	// MD0 = 0

			Err = Hw_I2C_IMU_HMC5883_WriteRegs( HW_HMC5883_REG_CFG_A, RegData, 3 );


			//Hw_HMC5883_Calibrate(1);
			Hw_I2C_IMU_HMC5883_WriteReg( HW_HMC5883_REG_CFG_A, 0x78 );
			Hw_I2C_IMU_HMC5883_WriteReg( HW_HMC5883_REG_MODE, 0 );			


			Err = Hw_I2C_IMU_HMC5883_WriteAddr( HW_HMC5883_REG_DATA_X_MSB );
		}
	}



	power_on = TRUE;


	if( Err == 0 )
	{
		Hw_HMC5883_EnableFlag = TRUE;
	}


	return Err;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_HMC5883_CheckID
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u8 Hw_HMC5883_CheckID( void )
{
	u8  Ret = TRUE;
	u16 Err = 0;
	u8  RegData[20];
	u16 i;

	Err = Hw_I2C_IMU_HMC5883_ReadRegs( HW_HMC5883_REG_ID_A, RegData, 3 );

	//Lb_printf(" %x  %x  %x\n", RegData[0], RegData[1], RegData[2] );
	if( Err == 0 )
	{
		if( RegData[0] != 0x48 ) Ret = FALSE;
		if( RegData[1] != 0x34 ) Ret = FALSE;
		if( RegData[2] != 0x33 ) Ret = FALSE;				
	}

	if( Err != 0 ) Ret = FALSE;

	return Ret;	
}




/*---------------------------------------------------------------------------
     TITLE   : Hw_HMC5883_GetData
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_HMC5883_GetData( HW_HMC5883_DATA_OBJ *ptr_data )
{
	u16 Err;

	HW_HMC5883_RAW_DATA_OBJ raw_data;


	Err = Hw_I2C_IMU_HMC5883_ReadRegs( HW_HMC5883_REG_DATA_X_MSB, (u8 *)&raw_data, 6 );


	if( Err == 0 )
	{
		ptr_data->X_Compass = (s16)((raw_data.X_Compass_H<<8) | (raw_data.X_Compass_L<<0));	
		ptr_data->Y_Compass = (s16)((raw_data.Y_Compass_H<<8) | (raw_data.Y_Compass_L<<0));	
		ptr_data->Z_Compass = (s16)((raw_data.Z_Compass_H<<8) | (raw_data.Z_Compass_L<<0));	


  		ptr_data->X_CompassScale = ((float) ptr_data->X_Compass) / x_scale;
  		ptr_data->Y_CompassScale = ((float) ptr_data->Y_Compass) / y_scale;
  		ptr_data->Z_CompassScale = ((float) ptr_data->Z_Compass) / z_scale;

	}
	

	Err = Hw_I2C_IMU_HMC5883_WriteAddr( HW_HMC5883_REG_DATA_X_MSB );

	return Err;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_HMC5883_GetEnable
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u8 Hw_HMC5883_GetEnable( void )
{
	return Hw_HMC5883_EnableFlag;
}





void Hw_HMC5883_Calibrate( u8  gain) 
{
	 HW_HMC5883_DATA_OBJ HMC5883_Data;


  x_scale=1; // get actual values
  y_scale=1;
  z_scale=1;
  //writeReg(HMC58X3_R_CONFA, 0x010 + HMC_POS_BIAS); // Reg A DOR=0x010 + MS1,MS0 set to pos bias

  Hw_I2C_IMU_HMC5883_WriteReg( HW_HMC5883_REG_CFG_A, 0x10 + 1 );

  //setGain(gain);
  Hw_I2C_IMU_HMC5883_WriteReg( HW_HMC5883_REG_CFG_B, gain << 5 );

  float x, y, z, mx=0, my=0, mz=0, t=10;
  

  for (int i=0; i<(int)t; i++) 
  { 
    //setMode(1);
    Hw_I2C_IMU_HMC5883_WriteReg( HW_HMC5883_REG_MODE, 1 );

    //getValues(&x,&y,&z);

	Hw_HMC5883_GetData( &HMC5883_Data );

	x = HMC5883_Data.X_Compass;
	y = HMC5883_Data.Y_Compass;
	z = HMC5883_Data.Z_Compass;

	Lb_printf(" cx : %d ", HMC5883_Data.X_Compass);
	Lb_printf(" cy : %d ", HMC5883_Data.Y_Compass);
	Lb_printf(" cz : %d \n", HMC5883_Data.Z_Compass);


    if (x>mx) mx=x;
    if (y>my) my=y;
    if (z>mz) mz=z;
  }
  
  float max=0;
  if (mx>max) max=mx;
  if (my>max) max=my;
  if (mz>max) max=mz;
  x_max=mx;
  y_max=my;
  z_max=mz;
  x_scale=max/mx; // calc scales
  y_scale=max/my;
  z_scale=max/mz;

 	Lb_printf(" mx : %d ", (s16)(x_scale*100));
	Lb_printf(" my : %d ", (s16)(y_scale*100));
	Lb_printf(" mz : %d \n", (s16)(z_scale*100));


  //writeReg(HMC58X3_R_CONFA, 0x010); // set RegA/DOR back to default
  Hw_I2C_IMU_HMC5883_WriteReg( HW_HMC5883_REG_CFG_A, 0x10 );
}   
