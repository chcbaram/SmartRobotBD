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
#define  HW_FREEIMU_LOCAL


#include "Hw_FreeIMU.h"
#include "Lb_Printf.h"
#include "math.h"



#define DEBUG_PRINT(x)



#define twoKpDef  (2.0f * 0.5f) // 2 * proportional gain
#define twoKiDef  (2.0f * 0.1f) // 2 * integral gain



int* raw_acc, raw_gyro, raw_magn;

// calibration parameters
int16_t gyro_off_x, gyro_off_y, gyro_off_z;
int16_t acc_off_x, acc_off_y, acc_off_z, magn_off_x, magn_off_y, magn_off_z;
float   acc_scale_x, acc_scale_y, acc_scale_z, magn_scale_x, magn_scale_y, magn_scale_z;


//float q0, q1, q2, q3; 		// quaternion elements representing the estimated orientation
float iq0, iq1, iq2, iq3;
float exInt, eyInt, ezInt;  	// scaled integral error
volatile float twoKp;      		// 2 * proportional gain (Kp)
volatile float twoKi;      		// 2 * integral gain (Ki)
volatile float q0, q1, q2, q3; 	// quaternion of sensor frame relative to auxiliary frame
volatile float integralFBx,  integralFBy, integralFBz;
unsigned long lastUpdate, now; 	// sample period expressed in milliseconds
float sampleFreq; 				// half the sample period expressed in seconds


s16 Hw_FreeIMU_Acc[3];
s16 Hw_FreeIMU_Gyro[3];
s16 Hw_FreeIMU_Compass[3];


void arr3_rad_to_deg(float * arr);
float invSqrt(float number);


void Hw_FreeIMU_getRawValues(int * raw_values);
void Hw_FreeIMU_AHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
void Hw_FreeIMU_getValues(float * values);

/*---------------------------------------------------------------------------
     TITLE   : Hw_FreeIMU_Init
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_FreeIMU_Init( void )
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
	  
	// initialize scale factors to neutral values
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
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_FreeIMU_GetUpdateQ
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_FreeIMU_GetUpdateQ( float * q, s16 *raw_value, float dt ) 
{
	float val[9];

	Hw_FreeIMU_SetRawValues( raw_value );
	Hw_FreeIMU_getValues( val );
	  

	DEBUG_PRINT(val[3] * M_PI/180);
	DEBUG_PRINT(val[4] * M_PI/180);
	DEBUG_PRINT(val[5] * M_PI/180);
	DEBUG_PRINT(val[0]);
	DEBUG_PRINT(val[1]);
	DEBUG_PRINT(val[2]);
	DEBUG_PRINT(val[6]);
	DEBUG_PRINT(val[7]);
	DEBUG_PRINT(val[8]);
	  

	Lb_printf("%d %d %d \n", (s16)val[0], (s16)val[1], (s16)val[2] );


	sampleFreq = dt;	// sec
	  

	// gyro values are expressed in deg/sec, the * M_PI/180 will convert it to radians/sec
	Hw_FreeIMU_AHRSupdate(val[3] * M_PI/180, val[4] * M_PI/180, val[5] * M_PI/180, val[0], val[1], val[2], val[6], val[7], val[8]);
	  
	q[0] = q0;
	q[1] = q1;
	q[2] = q2;
	q[3] = q3;
}





void Hw_FreeIMU_GetYawPitchRollRad( float *q, float * ypr )
{
  float gx, gy, gz; // estimated gravity direction
  
  gx = 2 * (q[1]*q[3] - q[0]*q[2]);
  gy = 2 * (q[0]*q[1] + q[2]*q[3]);
  gz = q[0]*q[0] - q[1]*q[1] - q[2]*q[2] + q[3]*q[3];
  
  ypr[0] = atan2(2 * q[1] * q[2] - 2 * q[0] * q[3], 2 * q[0]*q[0] + 2 * q[1] * q[1] - 1);
  ypr[1] = atan(gx / sqrt(gy*gy + gz*gz));
  ypr[2] = atan(gy / sqrt(gx*gx + gz*gz));
}


void Hw_FreeIMU_GetYawPitchRoll( float *q, float * ypr) 
{
	Hw_FreeIMU_GetYawPitchRollRad( q, ypr);
	arr3_rad_to_deg(ypr);
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_FreeIMU_SetRawValues
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_FreeIMU_SetRawValues( s16 *raw_values )
{      
	Hw_FreeIMU_Acc[0] = raw_values[0];
	Hw_FreeIMU_Acc[1] = raw_values[1];
	Hw_FreeIMU_Acc[2] = raw_values[2];

	Hw_FreeIMU_Gyro[0] = raw_values[3];
	Hw_FreeIMU_Gyro[1] = raw_values[4];
	Hw_FreeIMU_Gyro[2] = raw_values[5];

	Hw_FreeIMU_Compass[0] = raw_values[6];
	Hw_FreeIMU_Compass[1] = raw_values[7];
	Hw_FreeIMU_Compass[2] = raw_values[8];
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_FreeIMU_Init
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
/**
 * Populates raw_values with the raw_values from the sensors
*/
void Hw_FreeIMU_getRawValues(int * raw_values) 
{
    int temp, press;

  
	raw_values[0] = Hw_FreeIMU_Acc[0];
    raw_values[1] = Hw_FreeIMU_Acc[1];
    raw_values[2] = Hw_FreeIMU_Acc[2];

	raw_values[3] = Hw_FreeIMU_Gyro[0];
	raw_values[4] = Hw_FreeIMU_Gyro[1];
	raw_values[5] = Hw_FreeIMU_Gyro[2];

	raw_values[6] = Hw_FreeIMU_Compass[0];
	raw_values[7] = Hw_FreeIMU_Compass[1];
	raw_values[8] = Hw_FreeIMU_Compass[2];

	//temp = baro.rawTemperature(MS561101BA_OSR_4096);
	temp = 25;
	raw_values[9] = temp;
    //press = baro.rawPressure(MS561101BA_OSR_4096);
    press = 0;
	raw_values[10] = press;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_FreeIMU_getValues
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
/**
 * Populates values with calibrated readings from the sensors
*/
void Hw_FreeIMU_getValues(float * values) 
{  
    int16_t accgyroval[6];
    
    accgyroval[0] = Hw_FreeIMU_Acc[0];
    accgyroval[1] = Hw_FreeIMU_Acc[1];
    accgyroval[2] = Hw_FreeIMU_Acc[2];
	accgyroval[3] = Hw_FreeIMU_Gyro[0];
	accgyroval[4] = Hw_FreeIMU_Gyro[1];
	accgyroval[5] = Hw_FreeIMU_Gyro[2];


    // remove offsets from the gyroscope
    accgyroval[3] = accgyroval[3] - gyro_off_x;
    accgyroval[4] = accgyroval[4] - gyro_off_y;
    accgyroval[5] = accgyroval[5] - gyro_off_z;

    for(int i = 0; i<6; i++) 
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
  


	values[6] = Hw_FreeIMU_Compass[0];
	values[7] = Hw_FreeIMU_Compass[1];
	values[8] = Hw_FreeIMU_Compass[2];


    // calibration 
    values[6] = (values[6] - magn_off_x) / magn_scale_x;
    values[7] = (values[7] - magn_off_y) / magn_scale_y;
    values[8] = (values[8] - magn_off_z) / magn_scale_z;
}




/*---------------------------------------------------------------------------
     TITLE   : Hw_FreeIMU_zeroGyro
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
/**
 * Computes gyro offsets
*/
 /*
void Hw_FreeIMU_zeroGyro() 
{
  const int totSamples = 3;
  int raw[11];
  float tmpOffsets[] = {0,0,0};
  
  for (int i = 0; i < totSamples; i++)
  {
    getRawValues(raw);
    tmpOffsets[0] += raw[3];
    tmpOffsets[1] += raw[4];
    tmpOffsets[2] += raw[5];
  }
  
  gyro_off_x = tmpOffsets[0] / totSamples;
  gyro_off_y = tmpOffsets[1] / totSamples;
  gyro_off_z = tmpOffsets[2] / totSamples;
}
*/





/*---------------------------------------------------------------------------
     TITLE   : Hw_FreeIMU_AHRSupdate
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
/**
 * Quaternion implementation of the 'DCM filter' [Mayhony et al].  Incorporates the magnetic distortion
 * compensation algorithms from Sebastian Madgwick's filter which eliminates the need for a reference
 * direction of flux (bx bz) to be predefined and limits the effect of magnetic distortions to yaw
 * axis only.
 * 
 * @see: http://www.x-io.co.uk/node/8#open_source_ahrs_and_imu_algorithms
*/
void Hw_FreeIMU_AHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz) 
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
  if((mx != 0.0f) && (my != 0.0f) && (mz != 0.0f)) {
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
    if(twoKi > 0.0f) 
    {
      integralFBx += twoKi * halfex * (1.0f / sampleFreq);  // integral error scaled by Ki
      integralFBy += twoKi * halfey * (1.0f / sampleFreq);
      integralFBz += twoKi * halfez * (1.0f / sampleFreq);
      gx += integralFBx;  // apply integral feedback
      gy += integralFBy;
      gz += integralFBz;
    }
    else 
    {
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
     TITLE   : Hw_FreeIMU_getQ
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
/**
 * Populates array q with a quaternion representing the IMU orientation with respect to the Earth
 * 
 * @param q the quaternion to populate
*/
void Hw_FreeIMU_getQ(float * q) 
{
  float val[9];
  Hw_FreeIMU_getValues(val);
  
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
  //sampleFreq = 1.0 / ((now - lastUpdate) / 1000000.0);	// sec
  //lastUpdate = now;
  

  // gyro values are expressed in deg/sec, the * M_PI/180 will convert it to radians/sec
  Hw_FreeIMU_AHRSupdate(val[3] * M_PI/180, val[4] * M_PI/180, val[5] * M_PI/180, val[0], val[1], val[2], val[6], val[7], val[8]);
  
  q[0] = q0;
  q[1] = q1;
  q[2] = q2;
  q[3] = q3;
}



/**
 * Returns the Euler angles in radians defined in the Aerospace sequence.
 * See Sebastian O.H. Madwick report "An efficient orientation filter for 
 * inertial and intertial/magnetic sensor arrays" Chapter 2 Quaternion representation
 * 
 * @param angles three floats array which will be populated by the Euler angles in radians
*/
void Hw_FreeIMU_getEulerRad(float * angles) 
{
  float q[4]; // quaternion
  Hw_FreeIMU_getQ(q);
  angles[0] = atan2(2 * q[1] * q[2] - 2 * q[0] * q[3], 2 * q[0]*q[0] + 2 * q[1] * q[1] - 1); // psi
  angles[1] = -asin(2 * q[1] * q[3] + 2 * q[0] * q[2]); // theta
  angles[2] = atan2(2 * q[2] * q[3] - 2 * q[0] * q[1], 2 * q[0] * q[0] + 2 * q[3] * q[3] - 1); // phi
}


/**
 * Returns the Euler angles in degrees defined with the Aerospace sequence.
 * See Sebastian O.H. Madwick report "An efficient orientation filter for 
 * inertial and intertial/magnetic sensor arrays" Chapter 2 Quaternion representation
 * 
 * @param angles three floats array which will be populated by the Euler angles in degrees
*/
void Hw_FreeIMU_getEuler(float * angles) 
{
  Hw_FreeIMU_getEulerRad(angles);
  arr3_rad_to_deg(angles);
}


/**
 * Returns the yaw pitch and roll angles, respectively defined as the angles in radians between
 * the Earth North and the IMU X axis (yaw), the Earth ground plane and the IMU X axis (pitch)
 * and the Earth ground plane and the IMU Y axis.
 * 
 * @note This is not an Euler representation: the rotations aren't consecutive rotations but only
 * angles from Earth and the IMU. For Euler representation Yaw, Pitch and Roll see FreeIMU::getEuler
 * 
 * @param ypr three floats array which will be populated by Yaw, Pitch and Roll angles in radians
*/
void Hw_FreeIMU_getYawPitchRollRad(float * ypr) 
{
  float q[4]; // quaternion
  float gx, gy, gz; // estimated gravity direction
  Hw_FreeIMU_getQ(q);
  
  gx = 2 * (q[1]*q[3] - q[0]*q[2]);
  gy = 2 * (q[0]*q[1] + q[2]*q[3]);
  gz = q[0]*q[0] - q[1]*q[1] - q[2]*q[2] + q[3]*q[3];
  
  ypr[0] = atan2(2 * q[1] * q[2] - 2 * q[0] * q[3], 2 * q[0]*q[0] + 2 * q[1] * q[1] - 1);
  ypr[1] = atan(gx / sqrt(gy*gy + gz*gz));
  ypr[2] = atan(gy / sqrt(gx*gx + gz*gz));
}


/**
 * Returns the yaw pitch and roll angles, respectively defined as the angles in degrees between
 * the Earth North and the IMU X axis (yaw), the Earth ground plane and the IMU X axis (pitch)
 * and the Earth ground plane and the IMU Y axis.
 * 
 * @note This is not an Euler representation: the rotations aren't consecutive rotations but only
 * angles from Earth and the IMU. For Euler representation Yaw, Pitch and Roll see FreeIMU::getEuler
 * 
 * @param ypr three floats array which will be populated by Yaw, Pitch and Roll angles in degrees
*/
void Hw_FreeIMU_getYawPitchRoll(float * ypr) 
{
  Hw_FreeIMU_getYawPitchRollRad(ypr);
  arr3_rad_to_deg(ypr);
}


/**
 * Converts a 3 elements array arr of angles expressed in radians into degrees
*/
void arr3_rad_to_deg(float * arr) 
{
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
	
  //volatile long i;
	volatile int i;
  	volatile float x, y;
  	volatile const float f = 1.5F;

  
  	x = number * 0.5F;
  	y = number;
  	i = * ( int * ) &y;
  	i = 0x5f375a86 - ( i >> 1 );
  	y = * ( float * ) &i;
  	y = y * ( f - ( x * y * y ) );
  
  	//y = invsqrt(number);
  	return y;
}



