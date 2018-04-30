#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <errno.h>
#include "smart_home_mpu6050.h"
#include <unistd.h>


int cntl_mpu6050_thread()
{

	int  fd_mpu6050;
	struct  mpu6050  data;

	fd_mpu6050  = open("/dev/smart_home_mpu6050",O_RDWR);
	if(fd_mpu6050  <  0)
	{
		perror("open fail");
		exit(EXIT_FAILURE);
	}

	while(1)
	{
		ioctl(fd_mpu6050,SMART_HOME_MPU6050_GYRO,&data);

		if(500 < data.y)
		{
			
			system("madplay /mnt/steal.mp3");
		}
//		printf("gyro:x  = %d,y  = %d,z  = %d\n",data.x,data.y,data.z);
#if 0
		ioctl(fd,MPU6050_ACCEL,&data);
		printf("accel:x  = %d,y  = %d,z  = %d\n",data.x,data.y,data.z);
		ioctl(fd,MPU6050_TEMP,&data);		
		printf("temp:temp  = %f\n",(float)data.temp/340+36.53);
#endif

		sleep(1);
	}

	close(fd_mpu6050);

	return 0;
}
