#ifndef BEEPTHREAD_H
#define BEEPTHREAD_H
int get_beep_fd(void);//获取蜂鸣器文件描述符
void beep_ctl(int fd ,unsigned long cmd);//利用cmd 控制蜂鸣器，cmd详见smart_home_cmd.h
#endif
