#ifndef LEDTHREAD_H
#define LEDTHREAD_H
#include <sys/ioctl.h>
#include <stdio.h>
#include "smart_home_cmd.h"
#include "smart_home_led.h"
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>

int get_led_fd(void){

	int fd;
	fd = open("/dev/smart_home_led",O_RDWR);
	if(fd < 0)
	{
		perror("get led driver");
		exit(EXIT_FAILURE);
	}
	return fd;

}


void led_ctl(int fd_led,unsigned long cmd)
{
	switch(cmd)
	{
	case LED1_ON:
		ioctl(fd_led, LED1_ON);
		break;
	case LED1_OFF:
		ioctl(fd_led, LED1_OFF);
		break;
	case LED2_ON:
		ioctl(fd_led, LED2_ON);
		break;
	case LED2_OFF:
		ioctl(fd_led, LED2_OFF);
		break;
	case LED3_ON:
		ioctl(fd_led, LED3_ON);
		break;
	case LED3_OFF:
		ioctl(fd_led, LED3_OFF);
		break;
	case LED4_ON:
		ioctl(fd_led, LED4_ON);
		break;
	case LED4_OFF:
		ioctl(fd_led, LED4_OFF);
		break;
	default:
		break;
	}
}

#endif
