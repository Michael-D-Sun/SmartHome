#ifndef __HEAD_H__
#define __HEAD_H__

struct mpu6050 {
	unsigned short x;
	unsigned short y;
	unsigned short z;
	signed short temp;
};

#define  SMPRT_DIV    0X19  //0X07
#define  CONFIG       0X1A//0X06
#define  GYRO_CONFIG  0X1B  //0X18
#define  ACCEL_CONFIG  0X1C  //0X01
#define  ACCEL_XOUT_H  0X3B
#define  ACCEL_XOUT_L  0X3C
#define  ACCEL_YOUT_H  0X3D
#define  ACCEL_YOUT_L  0X3E
#define  ACCEL_ZOUT_H  0X3F
#define  ACCEL_ZOUT_L  0X40
#define  TEMP_OUT_H    0X41
#define  TEMP_OUT_L    0X42
#define  GYRO_XOUT_H   0X43
#define  GYRO_XOUT_L   0X44
#define  GYRO_YOUT_H   0X45
#define  GYRO_YOUT_L   0X46
#define  GYRO_ZOUT_H   0X47
#define  GYRO_ZOUT_L   0X48
#define  PWR_MGMT_1    0X6B//0X00
#define SMART_HOME_MPU6050_GYRO   _IOR('K', 0, struct mpu6050)
#define SMART_HOME_MPU6050_ACCEL  _IOR('K', 1, struct mpu6050)
#define SMART_HOME_MPU6050_TEMP   _IOR('K', 2, struct mpu6050)

#endif
