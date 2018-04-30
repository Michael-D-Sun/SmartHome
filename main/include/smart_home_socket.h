#ifndef _SMART_HOME_SOCKET_H_
#define _SMART_HOME_SOCKET_H_
#include "smart_home_queue.h"


#include<stdio.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<strings.h>
#include"smart_home_queue.h"
#include<asm-generic/ioctl.h>


typedef struct {
	linkqueue_t *temperature_queue;
	linkqueue_t *sound_queue;
	linkqueue_t *light_queue;
	linkqueue_t *gas_queue;
	linkqueue_t *shake_queue;
} sensor_queue_t;




/*
#ifndef __cplusplus

extern "C"{

int init_socket(char *ip,int port,int num,sensor_queue_t *socket_read_queue);
int socket_read(int sockfd,sensor_queue_t *socket_read_queue);
int socket_write(int sockfd,sock_msg msg,int device);
int cmd_from_socket(int cmd,const char *name);
#endif

#ifndef __cplusplus
}
#endif
*/

#ifdef __cplusplus
extern "C"{


#endif
typedef struct
{
	int led_fd;
	int beep_fd;
}device_fd_t;

typedef struct {
	unsigned long temperatureValue;
	unsigned long vibrationValue;
	unsigned long soundValue;
	unsigned long touchValue;
}SensorValue;

int init_socket(char *ip,int port,int num,sensor_queue_t *socket_read_queue);
int socket_read(int sockfd,sensor_queue_t *socket_read_queue,device_fd_t device_fd);
int socket_write(int sockfd,sock_msg msg,int device);
int cmd_from_socket(int cmd,const char *name);
int socket_signal_read(int confd,SensorValue *socket_read_queue);
int socket_signal_write(int confd,sock_msg msg);
int socket_accept(int sockfd,int *confd);
int close_connect(int confd);

#ifdef __cplusplus
}
#endif

#endif
