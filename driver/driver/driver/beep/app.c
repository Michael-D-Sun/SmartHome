#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include "smart_home_beep.h"

int main(int argc,char ** argv)
{
	int fd = open("/dev/smart_home_beep",O_RDWR);
	if(fd < 0)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}

	while(1)
	{
		ioctl(fd,BEEP_ON);
		sleep(1);
		ioctl(fd,BEEP_OFF);
		sleep(1);
	}
	return 0;
}
