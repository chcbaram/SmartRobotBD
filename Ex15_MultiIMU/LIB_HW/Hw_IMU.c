//----------------------------------------------------------------------------
//    프로그램명 	: IMU 센서 관련 함수
//
//    만든이     	: Cho Han Cheol(Baram)
//
//    날  짜     	: 
//    
//    최종 수정  	:
//
//    MPU_Type	:
//
//    파일명     	: Hw_IMU.c
//----------------------------------------------------------------------------


//----- 헤더파일 열기
//
#define  HW_IMU_LOCAL


#include "Hw_IMU.h"
#include "Lb_Printf.h"
#include "math.h"



//-- FreeIMU
//

#define DEBUG_PRINT(x)

#define twoKpDef  (2.0f * 0.5f) // 2 * proportional gain
#define twoKiDef  (2.0f * 0.1f) // 2 * integral gain

int* 			raw_acc, raw_gyro, raw_magn;
// calibration parameters
int16_t 		gyro_off_x, gyro_off_y, gyro_off_z;



float 			iq0, iq1, iq2, iq3;
float 			exInt, eyInt, ezInt;  	// scaled integral error
volatile float 	twoKp;      		// 2 * proportional gain (Kp)
volatile float 	twoKi;      		// 2 * integral gain (Ki)
volatile float 	q0, q1, q2, q3; 	// quaternion of sensor frame relative to auxiliary frame
volatile float 	integralFBx,  integralFBy, integralFBz;
//unsigned long 	lastUpdate, now; 	// sample period expressed in milliseconds
u16 			lastUpdate, now; 	// sample period expressed in milliseconds

float 			sampleFreq; 				// half the sample period expressed in seconds


HW_IMU_DATA_OBJ Hw_IMU_SaveData;


#define CALIBRATION_H

/*
const int acc_off_x = -243;
const int acc_off_y = -596;
const int acc_off_z = -2467;
const float acc_scale_x = 13634.366669;
const float acc_scale_y = 16414.402439;
const float acc_scale_z = 17145.649495;

const int magn_off_x = -99;
const int magn_off_y = -174;
const int magn_off_z = -257;
const float magn_scale_x = 524.728029;
const float magn_scale_y = 768.891697;
const float magn_scale_z = 850.038313;
*/

const int acc_off_x = 342;
const int acc_off_y = -168;
const int acc_off_z = -2487;
const float acc_scale_x = 14398.798858;
const float acc_scale_y = 16412.149985;
const float acc_scale_z = 17198.799846;

const int magn_off_x = -87;
const int magn_off_y = -216;
const int magn_off_z = -156;
const float magn_scale_x = 548.279660;
const float magn_scale_y = 927.753161;
const float magn_scale_z = 1053.126182;


#ifndef CALIBRATION_H
int16_t 		acc_off_x, acc_off_y, acc_off_z, magn_off_x, magn_off_y, magn_off_z;
float 			acc_scale_x, acc_scale_y, acc_scale_z, magn_scale_x, magn_scale_y, magn_scale_z;
#endif


float invSqrt(float number);
void arr3_rad_to_deg(float * arr);


void Hw_IMU_FreeIMU_Init( void );
u16  Hw_IMU_FreeIMU_getRawValues(int * raw_values);
u16  Hw_IMU_FreeIMU_getValues(float * values);
u16  Hw_IMU_FreeIMU_zeroGyro( void );
void Hw_IMU_FreeIMU_AHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);

u16  Hw_IMU_FreeIMU_getQ(float * q);
u16  Hw_IMU_FreeIMU_getEulerRad(float * angles);
u16  Hw_IMU_FreeIMU_getYawPitchRollRad(float * ypr);
u16  Hw_IMU_FreeIMU_getYawPitchRoll(float * ypr);



/*---------------------------------------------------------------------------
     TITLE   : Hw_IMU_Init
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_IMU_Init( void )
{
	u16 Ret = 0;


	Ret = Hw_MPU6050_Init();

	if( Ret != 0 )
	{
		Lb_printf("Hw_MPU6050_Init Ret : 0x%x\n", Ret);
		return Ret;
	} 


	Ret = Hw_HMC5883_Init();

	if( Ret != 0 )
	{
		Lb_printf("Hw_HMC5883_Init Ret : 0x%x\n", Ret);
		return Ret;
	} 

	Hw_IMU_FreeIMU_Init();
	//Hw_MultiIMU_Init();

	return Ret;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_IMU_FreeIMU_Init
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_IMU_FreeIMU_Init( void )
{
	// initialize quaternion
	q0 = 1.0f;
	q1 = 0.0f;
	q2 = 0.0f;
	q3 = 0.0f;
	exInt = 0.0;
	eyInt = 0.0;
	ezInt = 0.0;
	twoKp = twoKpDef;
	twoKi = twoKiDef;
	integralFBx = 0.0f,  integralFBy = 0.0f, integralFBz = 0.0f;
	lastUpdate = 0;
	now = 0;

	gyro_off_x = 0;
	gyro_off_y = 0;
	gyro_off_z = 0;


#ifndef CALIBRATION_H
    acc_off_x = 0;
    acc_off_y = 0;
    acc_off_z = 0;
    acc_scale_x = 1;
    acc_scale_y = 1;
    acc_scale_z = 1;

    magn_off_x = 0;
    magn_off_y = 0;
    magn_off_z = 0;
    magn_scale_x = 1;
    magn_scale_y = 1;
    magn_scale_z = 1;	
#endif

}




/*---------------------------------------------------------------------------
     TITLE   : Hw_IMU_Reset
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_IMU_Reset( HW_IMU_DATA_OBJ *ptr_data )
{
	u16 Err;

	Err = Hw_IMU_GetData( ptr_data );

	if( Err == 0 )
	{
		ptr_data->X_Angle = ptr_data->X_AccAngle;
		ptr_data->Y_Angle = ptr_data->Y_AccAngle;
	}
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_IMU_Update
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_IMU_Update( HW_IMU_DATA_OBJ *ptr_data, u32 dt )
{
	u16 Err;
	f32 GyroRateX;
	f32 GyroRateY;
	f32 Q[4];
	s16 raw_value[9];
	f32 dt_sec;
	f32 ypr[3];
	f32 yprrad[3];
	u32 i;


    int16_t head;

    float cosineRoll;
    float sineRoll;
    float cosinePitch;
    float sinePitch;
    float Xh;
    float Yh;
    float hd;

	float dv;
	float mx,my,mz;

    int16_t magneticDeclination = 0;


    #if 0
	Err = Hw_IMU_FreeIMU_getYawPitchRoll( ypr );

	if( Err == 0 )
	{
		Hw_IMU_SaveData.X_Angle = ypr[2];
		Hw_IMU_SaveData.Y_Angle = ypr[1];
		Hw_IMU_SaveData.Z_Angle = ypr[0];

		*ptr_data = Hw_IMU_SaveData;
	}
	#endif

	#if 1
	Err = Hw_IMU_GetData( ptr_data );

	if( Err == 0 )
	{
		GyroRateX = (f32)ptr_data->X_Gyro/131.0;
		GyroRateY = (f32)ptr_data->Y_Gyro/131.0;

		ptr_data->X_Angle = (0.93 * (ptr_data->X_Angle+(GyroRateX*(f32)(dt)/1000)) ) + (0.07 * ptr_data->X_AccAngle);
  		ptr_data->Y_Angle = (0.93 * (ptr_data->Y_Angle+(GyroRateY*(f32)(dt)/1000)) ) + (0.07 * ptr_data->Y_AccAngle); 
		dt_sec = ((float)dt / 1000.);



	    cosineRoll 	= cosf(ptr_data->X_Angle * M_PI / 180.);
	    sineRoll 	= sinf(ptr_data->X_Angle * M_PI / 180.);
	    cosinePitch = cosf(ptr_data->Y_Angle * M_PI / 180.);
	    sinePitch 	= sinf(ptr_data->Y_Angle * M_PI / 180.);

	    dv = invSqrt( ptr_data->X_Compass*ptr_data->X_Compass + ptr_data->Y_Compass*ptr_data->Y_Compass + ptr_data->Z_Compass*ptr_data->Z_Compass );

	    mx = dv*ptr_data->X_Compass;
	    my = dv*ptr_data->Y_Compass;
	    mz = dv*ptr_data->Z_Compass;


	    //Xh = mx * cosinePitch + my * sineRoll * sinePitch + mz * sinePitch * cosineRoll;
	    //Yh = my * cosineRoll  - mz * sineRoll;
	    Xh = mx + my + mz;
	    Yh = my - mz;

	    //hd = (atan2f(Yh, Xh) * 180.0f / M_PI);
		hd = (atan2f(mx, my) * 180.0f / M_PI);
	    	    

	    head = hd;
	    if (head < 0)
	    {
	        head += 360;
	    }

	    Lb_printf("Head : %d  %d %d \n", head, cosineRoll*180./M_PI, sineRoll*180./M_PI);





		raw_value[0] = ptr_data->X_Acc;
		raw_value[1] = ptr_data->Y_Acc;
		raw_value[2] = ptr_data->Z_Acc;

		raw_value[3] = ptr_data->X_Gyro;
		raw_value[4] = ptr_data->Y_Gyro;
		raw_value[5] = ptr_data->Z_Gyro;


		//raw_value[6] = (s16)(ptr_data->X_CompassScale + 0.5f);
		//raw_value[7] = (s16)(ptr_data->Y_CompassScale + 0.5f);
		//raw_value[8] = (s16)(ptr_data->Z_CompassScale + 0.5f);

		raw_value[6] = ptr_data->X_Compass;
		raw_value[7] = ptr_data->Y_Compass;
		raw_value[8] = ptr_data->Z_Compass;

		/*
		for( i=0; i<9; i++ )
		{
			raw_value[i] = 0;
		}
		raw_value[0] = ptr_data->X_Acc;
		raw_value[1] = ptr_data->Y_Acc;
		raw_value[2] = ptr_data->Z_Acc;

		raw_value[3] = 0;
		raw_value[4] = 0;
		raw_value[5] = ptr_data->Z_Gyro;
		*/

	}
	#endif

	return Err;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_IMU_GetData
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_IMU_GetData( HW_IMU_DATA_OBJ *ptr_data )
{
	u16 Err = 0;

	HW_MPU6050_DATA_OBJ MPU6050_Data;
	HW_HMC5883_DATA_OBJ HMC5883_Data;


	ptr_data->Enable_Acc     = FALSE;
	ptr_data->Enable_Temp    = FALSE;
	ptr_data->Enable_Gyro    = FALSE;
	ptr_data->Enable_Compass = FALSE;


	if( Hw_HMC5883_GetEnable() == TRUE )
	{
		Err = Hw_MPU6050_GetData( &MPU6050_Data );

		if( Err == 0 )
		{
			ptr_data->X_Acc = MPU6050_Data.X_Acc;
			ptr_data->Y_Acc = MPU6050_Data.Y_Acc;
			ptr_data->Z_Acc = MPU6050_Data.Z_Acc;

			ptr_data->Temp  = MPU6050_Data.Temp;

			ptr_data->X_Gyro = MPU6050_Data.X_Gyro;
			ptr_data->Y_Gyro = MPU6050_Data.Y_Gyro;
			ptr_data->Z_Gyro = MPU6050_Data.Z_Gyro;


			ptr_data->Enable_Acc     = TRUE;
			ptr_data->Enable_Temp    = TRUE;
			ptr_data->Enable_Gyro    = TRUE;	

			// 각도 계산 
			ptr_data->X_AccAngle = (atan2(MPU6050_Data.Y_Acc, MPU6050_Data.Z_Acc)+M_PI)*RAD_TO_DEG;
			ptr_data->Y_AccAngle = (atan2(MPU6050_Data.X_Acc, MPU6050_Data.Z_Acc)+M_PI)*RAD_TO_DEG;
		}
	}



	if( Hw_HMC5883_GetEnable() == TRUE )
	{
		Err = Hw_HMC5883_GetData( &HMC5883_Data );	

		if( Err == 0 )
		{
			ptr_data->X_Compass = HMC5883_Data.X_Compass;
			ptr_data->Y_Compass = HMC5883_Data.Y_Compass;
			ptr_data->Z_Compass = HMC5883_Data.Z_Compass;

			ptr_data->X_CompassScale = HMC5883_Data.X_CompassScale;
			ptr_data->Y_CompassScale = HMC5883_Data.Y_CompassScale;
			ptr_data->Z_CompassScale = HMC5883_Data.Z_CompassScale;


			ptr_data->Enable_Compass = TRUE;			
		}
	}


	return Err;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_IMU_FreeIMU_getRawValues
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_IMU_FreeIMU_getRawValues(int * raw_values) 
{
	u16 Err;

	HW_IMU_DATA_OBJ IMU_Data;



	Err = Hw_IMU_GetData( &IMU_Data );   	

	if( Err == 0 )
	{
		raw_values[0] = IMU_Data.X_Acc;
		raw_values[1] = IMU_Data.Y_Acc;
		raw_values[2] = IMU_Data.Z_Acc;

		raw_values[3] = IMU_Data.X_Gyro;
		raw_values[4] = IMU_Data.Y_Gyro;
		raw_values[5] = IMU_Data.Z_Gyro;

		raw_values[6] = IMU_Data.X_Compass;
		raw_values[7] = IMU_Data.Y_Compass;
		raw_values[8] = IMU_Data.Z_Compass;

		Hw_IMU_SaveData = IMU_Data;
	}	


	int temp, press;
    
	//TODO: possible loss of precision
	temp = 25;
	raw_values[9] = temp;
	press = 0;
	raw_values[10] = press;

	return Err;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_IMU_FreeIMU_getValues
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_IMU_FreeIMU_getValues(float * values) 
{  
    int16_t accgyroval[6];
    int i;

	u16 Err;

	HW_IMU_DATA_OBJ IMU_Data;


	Err = Hw_IMU_GetData( &IMU_Data ); 

	Hw_IMU_SaveData = IMU_Data;


	accgyroval[0] = IMU_Data.X_Acc;
	accgyroval[1] = IMU_Data.Y_Acc;
	accgyroval[2] = IMU_Data.Z_Acc;

	accgyroval[3] = IMU_Data.X_Gyro;
	accgyroval[4] = IMU_Data.Y_Gyro;
	accgyroval[5] = IMU_Data.Z_Gyro;

    
    // remove offsets from the gyroscope
    accgyroval[3] = accgyroval[3] - gyro_off_x;
    accgyroval[4] = accgyroval[4] - gyro_off_y;
    accgyroval[5] = accgyroval[5] - gyro_off_z;

    for(i = 0; i<6; i++) 
    {
      if(i < 3) 
      {
        values[i] = (float) accgyroval[i];
      }
      else 
      {
        values[i] = ((float) accgyroval[i]) / 16.4f; // NOTE: this depends on the sensitivity chosen
      }
    }
  
  
	// remove offsets and scale accelerometer (calibration)
	values[0] = (values[0] - acc_off_x) / acc_scale_x;
	values[1] = (values[1] - acc_off_y) / acc_scale_y;
	values[2] = (values[2] - acc_off_z) / acc_scale_z;
  
  
  	values[6] = IMU_Data.X_Compass;
	values[7] = IMU_Data.Y_Compass;
	values[8] = IMU_Data.Z_Compass;


	values[6] = (values[6] - magn_off_x) / magn_scale_x;
	values[7] = (values[7] - magn_off_y) / magn_scale_y;
	values[8] = (values[8] - magn_off_z) / magn_scale_z;


	#if 0
	values[0] = 0;
	values[1] = 0;
	values[2] = 0;

	values[3] = 0;
	values[4] = 0;
	values[5] = 0;

	values[6] = 1;
	values[7] = 1;
	values[8] = 1;
	#endif

	Lb_printf("mg %d %d %d\n", (s16)values[6], (s16)values[7], (s16)values[8] );


	return Err;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_IMU_FreeIMU_zeroGyro
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_IMU_FreeIMU_zeroGyro( void ) 
{
	u16 Err = 0;

  const int totSamples = 3;
  int raw[11];
  float tmpOffsets[] = {0,0,0};
  
  for (int i = 0; i < totSamples; i++)
  {
    Err = Hw_IMU_FreeIMU_getRawValues(raw);
    tmpOffsets[0] += raw[3];
    tmpOffsets[1] += raw[4];
    tmpOffsets[2] += raw[5];

    if( Err != 0 ) break;
  }
  
  gyro_off_x = tmpOffsets[0] / totSamples;
  gyro_off_y = tmpOffsets[1] / totSamples;
  gyro_off_z = tmpOffsets[2] / totSamples;


  return Err;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_IMU_FreeIMU_zeroGyro
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_IMU_FreeIMU_AHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz) 
{

  float recipNorm;
  float q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;
  float halfex = 0.0f, halfey = 0.0f, halfez = 0.0f;
  float qa, qb, qc;

  // Auxiliary variables to avoid repeated arithmetic
  q0q0 = q0 * q0;
  q0q1 = q0 * q1;
  q0q2 = q0 * q2;
  q0q3 = q0 * q3;
  q1q1 = q1 * q1;
  q1q2 = q1 * q2;
  q1q3 = q1 * q3;
  q2q2 = q2 * q2;
  q2q3 = q2 * q3;
  q3q3 = q3 * q3;
  

  // Use magnetometer measurement only when valid (avoids NaN in magnetometer normalisation)
  if((mx != 0.0f) && (my != 0.0f) && (mz != 0.0f)) 
  {
    float hx, hy, bx, bz;
    float halfwx, halfwy, halfwz;
    

    // Normalise magnetometer measurement
    recipNorm = invSqrt(mx * mx + my * my + mz * mz);
    mx *= recipNorm;
    my *= recipNorm;
    mz *= recipNorm;
    
    // Reference direction of Earth's magnetic field
    hx = 2.0f * (mx * (0.5f - q2q2 - q3q3) + my * (q1q2 - q0q3) + mz * (q1q3 + q0q2));
    hy = 2.0f * (mx * (q1q2 + q0q3) + my * (0.5f - q1q1 - q3q3) + mz * (q2q3 - q0q1));
    bx = sqrt(hx * hx + hy * hy);
    bz = 2.0f * (mx * (q1q3 - q0q2) + my * (q2q3 + q0q1) + mz * (0.5f - q1q1 - q2q2));
    
    // Estimated direction of magnetic field
    halfwx = bx * (0.5f - q2q2 - q3q3) + bz * (q1q3 - q0q2);
    halfwy = bx * (q1q2 - q0q3) + bz * (q0q1 + q2q3);
    halfwz = bx * (q0q2 + q1q3) + bz * (0.5f - q1q1 - q2q2);
    

    // Error is sum of cross product between estimated direction and measured direction of field vectors
    halfex = (my * halfwz - mz * halfwy);
    halfey = (mz * halfwx - mx * halfwz);
    halfez = (mx * halfwy - my * halfwx);


	Lb_printf("mg %d %d %d  %d %d %d  %d %d %d \n", 
			(s16)(halfex*10000), (s16)(halfey*10000), (s16)(halfez*10000),
			(s16)(halfwx*10000), (s16)(halfwy*10000), (s16)(halfwz*10000),
			(s16)(mx*1000), (s16)(my*1000), (s16)(mz*1000) );

  }

  // Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
  if((ax != 0.0f) && (ay != 0.0f) && (az != 0.0f)) 
  {
    float halfvx, halfvy, halfvz;
    
    // Normalise accelerometer measurement
    recipNorm = invSqrt(ax * ax + ay * ay + az * az);
    ax *= recipNorm;
    ay *= recipNorm;
    az *= recipNorm;
    
    // Estimated direction of gravity
    halfvx = q1q3 - q0q2;
    halfvy = q0q1 + q2q3;
    halfvz = q0q0 - 0.5f + q3q3;
  

    // Error is sum of cross product between estimated direction and measured direction of field vectors
    halfex += (ay * halfvz - az * halfvy);
    halfey += (az * halfvx - ax * halfvz);
    halfez += (ax * halfvy - ay * halfvx);
  }

  // Apply feedback only when valid data has been gathered from the accelerometer or magnetometer
  if(halfex != 0.0f && halfey != 0.0f && halfez != 0.0f) 
  {
    // Compute and apply integral feedback if enabled
    if(twoKi > 0.0f) {
      integralFBx += twoKi * halfex * (1.0f / sampleFreq);  // integral error scaled by Ki
      integralFBy += twoKi * halfey * (1.0f / sampleFreq);
      integralFBz += twoKi * halfez * (1.0f / sampleFreq);
      gx += integralFBx;  // apply integral feedback
      gy += integralFBy;
      gz += integralFBz;
    }
    else {
      integralFBx = 0.0f; // prevent integral windup
      integralFBy = 0.0f;
      integralFBz = 0.0f;
    }

    // Apply proportional feedback
    gx += twoKp * halfex;
    gy += twoKp * halfey;
    gz += twoKp * halfez;
  }
  
  // Integrate rate of change of quaternion
  gx *= (0.5f * (1.0f / sampleFreq));   // pre-multiply common factors
  gy *= (0.5f * (1.0f / sampleFreq));
  gz *= (0.5f * (1.0f / sampleFreq));
  qa = q0;
  qb = q1;
  qc = q2;
  q0 += (-qb * gx - qc * gy - q3 * gz);
  q1 += (qa * gx + qc * gz - q3 * gy);
  q2 += (qa * gy - qb * gz + q3 * gx);
  q3 += (qa * gz + qb * gy - qc * gx);
  
  // Normalise quaternion
  recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
  q0 *= recipNorm;
  q1 *= recipNorm;
  q2 *= recipNorm;
  q3 *= recipNorm;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_IMU_FreeIMU_getQ
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_IMU_FreeIMU_getQ(float * q) 
{
  u16 Err;
  u16 time_gap;

  float val[9];
  
  Err = Hw_IMU_FreeIMU_getValues(val);
  
  DEBUG_PRINT(val[3] * M_PI/180);
  DEBUG_PRINT(val[4] * M_PI/180);
  DEBUG_PRINT(val[5] * M_PI/180);
  DEBUG_PRINT(val[0]);
  DEBUG_PRINT(val[1]);
  DEBUG_PRINT(val[2]);
  DEBUG_PRINT(val[6]);
  DEBUG_PRINT(val[7]);
  DEBUG_PRINT(val[8]);
  
  //now = micros();
  now = Hw_Timer_Get_u16Count_Usec();
  time_gap = now - lastUpdate;

  sampleFreq = 1.0 / ((time_gap) / 1000000.0);

  //Lb_printf("%d us %d \n", (u16)(sampleFreq*1000), time_gap);

  lastUpdate = now;



  // gyro values are expressed in deg/sec, the * M_PI/180 will convert it to radians/sec
  Hw_IMU_FreeIMU_AHRSupdate(val[3] * M_PI/180, val[4] * M_PI/180, val[5] * M_PI/180, val[0], val[1], val[2], val[6], val[7], val[8]);

  
  q[0] = q0;
  q[1] = q1;
  q[2] = q2;
  q[3] = q3;

  return Err;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_IMU_FreeIMU_getEulerRad
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_IMU_FreeIMU_getEulerRad(float * angles) 
{
  u16 Err;
  float q[4]; // quaternion
  Err = Hw_IMU_FreeIMU_getQ(q);

  angles[0] = atan2(2 * q[1] * q[2] - 2 * q[0] * q[3], 2 * q[0]*q[0] + 2 * q[1] * q[1] - 1); // psi
  angles[1] = -asin(2 * q[1] * q[3] + 2 * q[0] * q[2]); // theta
  angles[2] = atan2(2 * q[2] * q[3] - 2 * q[0] * q[1], 2 * q[0] * q[0] + 2 * q[3] * q[3] - 1); // phi

  return Err;
}




/*---------------------------------------------------------------------------
     TITLE   : Hw_IMU_FreeIMU_getYawPitchRollRad
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_IMU_FreeIMU_getYawPitchRollRad(float * ypr) 
{
  u16 Err;
  float q[4]; // quaternion
  float gx, gy, gz; // estimated gravity direction
  
  Err = Hw_IMU_FreeIMU_getQ(q);
  
  gx = 2 * (q[1]*q[3] - q[0]*q[2]);
  gy = 2 * (q[0]*q[1] + q[2]*q[3]);
  gz = q[0]*q[0] - q[1]*q[1] - q[2]*q[2] + q[3]*q[3];
  
  ypr[0] = atan2(2 * q[1] * q[2] - 2 * q[0] * q[3], 2 * q[0]*q[0] + 2 * q[1] * q[1] - 1);
  ypr[1] = atan(gx / sqrt(gy*gy + gz*gz));
  ypr[2] = atan(gy / sqrt(gx*gx + gz*gz));

  return Err;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_IMU_FreeIMU_getYawPitchRollRad
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_IMU_FreeIMU_getYawPitchRoll(float * ypr) 
{
	u16 Err;

	Err = Hw_IMU_FreeIMU_getYawPitchRollRad(ypr);
	arr3_rad_to_deg(ypr);

	return Err;
}




/**
 * Converts a 3 elements array arr of angles expressed in radians into degrees
*/
void arr3_rad_to_deg(float * arr) {
  arr[0] *= 180/M_PI;
  arr[1] *= 180/M_PI;
  arr[2] *= 180/M_PI;
}


/**
 * Fast inverse square root implementation
 * @see http://en.wikipedia.org/wiki/Fast_inverse_square_root
*/
float invSqrt(float number) 
{
  volatile long i;
  volatile float x, y;
  volatile const float f = 1.5F;

  x = number * 0.5F;
  y = number;
  i = * ( long * ) &y;
  i = 0x5f375a86 - ( i >> 1 );
  y = * ( float * ) &i;
  y = y * ( f - ( x * y * y ) );
  return y;
}