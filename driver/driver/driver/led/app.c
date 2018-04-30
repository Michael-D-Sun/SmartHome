#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include "smart_home_led.h"


int main(int argc,char **argv)
{
	int fd;
	fd = open("/dev/smart_home_led",O_RDWR);
	if(fd < 0)
	{	
		perror("open");
		exit(EXIT_FAILURE);
	}

	while(1)
	{
		ioctl(fd,LED1_ON);
		sleep(1);
		ioctl(fd,LED1_OFF);
		sleep(1);
		ioctl(fd,LED2_ON);
		sleep(1);
		ioctl(fd,LED2_OFF);
		sleep(1);
		ioctl(fd,LED3_ON);
		sleep(1);
		ioctl(fd,LED3_OFF);
		sleep(1);
		ioctl(fd,LED4_ON);
		sleep(1);
		ioctl(fd,LED4_OFF);
		sleep(1);
		ioctl(fd,LEDALL_ON);
		sleep(1);
		ioctl(fd,LEDALL_OFF);
		sleep(1);
		
	}
	
	return 0;
}



