#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "smart_home_mpu6050.h"
#include <sys/ioctl.h>

int main(int argc, const char *argv[])
{
	int fd;
	struct mpu6050 data;


	fd = open("/dev/smart_home_mpu6050", O_RDWR);
	if (fd < 0)
	{
		perror("fail open");
		exit(EXIT_FAILURE);
	}

	while (1)
	{

		ioctl(fd,SMART_HOME_MPU6050_GYRO,&data);
		printf("GYRO:x = %d,y = %d,z = %d\n",data.x,data.y,data.z);
		ioctl(fd,SMART_HOME_MPU6050_ACCEL,&data);
		printf("ACCEL:x = %d,y = %d,z = %d\n",data.x,data.y,data.z);
		ioctl(fd,SMART_HOME_MPU6050_TEMP,&data);
		printf("TEMP:%f\n",(float)data.temp/340+36.53);

		sleep(1);





	}

	return 0;

}

