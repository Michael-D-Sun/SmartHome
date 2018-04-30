#ifndef LEDTHREAD_H
#define LEDTHREAD_H
#include <sys/ioctl.h>
#include "smart_home_cmd.h"


int get_led_fd();//获取led文件描述符
void led_ctl(int fd_led,unsigned long cmd);//通过cmd对led进行操作，cmd详见smart_home_cmd.h

#endif
