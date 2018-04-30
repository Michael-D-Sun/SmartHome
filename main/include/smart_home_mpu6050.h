#ifndef __HEAD_H__
#define __HEAD_H__

struct mpu6050 {
	unsigned short x;
	unsigned short y;
	unsigned short z;
	signed short temp;
};

#define SMART_HOME_MPU6050_GYRO   _IOR('K', 0, struct mpu6050)
#define SMART_HOME_MPU6050_ACCEL  _IOR('K', 1, struct mpu6050)
#define SMART_HOME_MPU6050_TEMP   _IOR('K', 2, struct mpu6050)

#endif
