#include <sys/ioctl.h>
#include "smart_home_beep.h"
#include "smart_home_cmd.h"
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

void beep_ctl(int fd_beep,unsigned long cmd)
{
	switch(cmd)
	{
	case BEEP_ON:
		ioctl(fd_beep, BEEP_ON);
		break;
	case BEEP_OFF:
		ioctl(fd_beep, BEEP_OFF);
		break;
	}
}
int get_beep_fd()
{
	int fd;
	fd = open("/dev/smart_home_beep",O_RDWR);
	if(fd < 0)
	{
		perror("get beep driver");
		exit(EXIT_FAILURE);
	}
	return fd;
}
