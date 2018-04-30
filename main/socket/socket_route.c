#include"smart_home_queue.h"
#include "smart_home_socket.h"
#include "smart_home_cmd.h"
#include "smart_home_led.h"
#define Handle_error(msg) do{perror(msg);exit(-1);}while(0)
#define DEBUG
#include "smart_home_adapter.h"
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <errno.h>

//#define HOSTADDR "192.168.1.100"
//#define HOSTPORT 8888

struct sockaddr_in hostaddr,clientaddr[10];

int  init_socket(char *ip,int port,int num,sensor_queue_t *socket_read_queue)
{
	int optval = 1;
	int ret;
	int sockfd;
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0)
	{
		Handle_error("socket");
	}
	hostaddr.sin_family = AF_INET;
	hostaddr.sin_port = htons(port);
	hostaddr.sin_addr.s_addr = inet_addr(ip);
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval));
	ret = bind(sockfd,(struct sockaddr *)&hostaddr,sizeof(hostaddr));
	if(ret < 0)
	{
		Handle_error("bind");
	}
	ret = listen(sockfd,num);
	if(ret < 0)
	{
		Handle_error("listen");
	}
	socket_read_queue->temperature_queue = creat_linkqueue();
	socket_read_queue->light_queue = creat_linkqueue();
	socket_read_queue->gas_queue = creat_linkqueue();
	socket_read_queue->shake_queue = creat_linkqueue();
	socket_read_queue->sound_queue = creat_linkqueue();
	printf("sockfd = %d\n",sockfd);
	return sockfd;
}

int socket_accept(int sockfd,int *m_confd){

	int confd,len;
	sock_msg msg;
	confd = accept(sockfd,(struct sockaddr *)&clientaddr,&len);
	if(confd < 0)
	{
		Handle_error("accept");
	}
	
	printf("accept success\n");
	bzero(&msg,sizeof(msg));
	recv(confd ,&msg,sizeof(msg),0);
	*m_confd = confd;
	printf("accept msg.type = %d\n",msg.type);
	printf("accept msg.data = %s\n",msg.date);
	return msg.type;
}


int socket_signal_write(int confd,sock_msg msg)
{
	int ret;
	bzero(&msg,sizeof(msg));
	ret = send(confd,&msg,sizeof(msg),0);
	if(ret == 0)
	{
		return -1;
	}
	return 0;
	
}

int sock_msg_adpter(sock_msg msg,SensorValue *Value)
{
	int shmid;
	key_t key;
	SensorValue *shm;
	switch(msg.type)
	{

	case READ_VIBRATION:
		printf("msg.type = %d\n",msg.type);
		Value->vibrationValue = msg.value;
		printf("upload main pthread vibration\n");
		break;

	case READ_TOUCH:
		printf("msg.type = %d\n",msg.type);
		Value->touchValue = msg.value;
		printf("upload main pthread touch\n");
		break;

	case READ_TEMPERATURE:
		printf("msg.type = %d\n",msg.type);
		Value->temperatureValue = msg.value;
		printf("upload main pthread temperature\n");
		break;
	case READ_SOUND:
		printf("msg.type = %d\n",msg.type);
		Value->soundValue = msg.value;
		printf("upload main pthread sound\n");
		break;
	default:
		printf("unkonw type!\n");
		return -1;
	}
	if((key = ftok("/", 'm')) < 0)
	{
		perror("ftok");
		exit(1);
	}
	shmid = shmget(key, sizeof(SensorValue), IPC_CREAT|IPC_EXCL|0664);
	/*send masge to sharememery*/
	if(shmid < 0)
	{
		if(errno == EEXIST)
		{
			shmid = shmget(key,sizeof(SensorValue), 0664);
		}
		else
		{		
			perror("shmget error");
			exit(1);
		}
	}
	shm = shmat(shmid, NULL, 0);
	
	memcpy(shm,Value,sizeof(SensorValue));
}



int socket_signal_read(int confd,SensorValue *Value)
{


	sock_msg msg;
	int ret;
	bzero(&msg,sizeof(sock_msg));
	ret = recv(confd ,&msg,sizeof(sock_msg),0);
	if(ret == 0){
		close(confd);
		return SOCKET_CONNECT_CLOSE;
	}else{
		printf("get msg.type = %d\n",msg.type);
		printf("get msg.value = %d\n",msg.value);
	//	printf("READ_TOUCH  %d\n",READ_TOUCH);
	//	printf("READ_TEMPERATURE  %d\n",READ_TEMPERATURE);
	//	printf("READ_VIBRATION  %d\n",READ_VIBRATION);
	//	printf("READ_SOUND  %d\n",READ_SOUND);
		sock_msg_adpter(msg,Value);
	}
	return msg.type;
}
int close_connect(int confd)
{
	close(confd);
	return 0;
}




int socket_read(int sockfd,sensor_queue_t *socket_read_queue,device_fd_t device_fd)
{
	int confd,ret,maxfd,i,len;
	fd_set current,global;
	FD_ZERO(&current);
	FD_ZERO(&global);
	FD_SET(sockfd,&global);
	maxfd = sockfd;
	sock_msg msg;
	//设置设备文件描述符
	printf("sockfd = %d\n",sockfd);

	int fd = device_fd.led_fd; 
	//*********************************

	len = sizeof(clientaddr);
#if 0
	while(1)
	{
		current = global;
		ret = select(maxfd+1,&current,NULL,NULL,NULL);
#ifdef DEBUG
		printf("select done \n");
#endif
		if(ret < 0)
		{
			Handle_error("select");
		}
		if(FD_ISSET(sockfd,&current))
		{
			confd = accept(sockfd,(struct sockaddr *)&clientaddr,&len);
			if(confd < 0)
			{
				Handle_error("accept");
			}
#ifdef DEBUG
			printf("new confd =%d\n",confd);
#endif
			FD_SET(confd,&global);
			maxfd = ((confd > maxfd )? confd : maxfd);
		}
		printf("current confd =%d\n",confd);
		for(i = sockfd + 1;i <= maxfd ; i++)
		{
			if(FD_ISSET(i,&current))
			{
				printf("i = %d\n",i);
				bzero(&msg,sizeof(msg));
				ret = recv(confd ,&msg,sizeof(msg),0);
				printf("get type =%d\n",msg.type);
				if(ret <= 0)
				{
					printf("client is closed");
					close(i);
					FD_CLR(i,&global);
					break;
				}
				else {
					switch(msg.type)
					{
			
					case 0:
						close(i);
						printf("close fd:%d\n",i);
						if(i == maxfd)
						{
							while(maxfd--)
							{
								if(FD_ISSET(maxfd,&current))
									break;
							}
							FD_CLR(i,&global);
							break;
						}
						break;

			
					case READ_TEMPERATURE:
						printf("recv: %s\n value %lf\n",msg.date,msg.value);
						in_queue(socket_read_queue->temperature_queue,&msg);
						sleep(1);
						break;
					case SOCKET_CONNECT_CLOSE:
#ifdef DEBUG
						printf("connect closing\n");
#endif
						close(i);
						if(i == maxfd)
						{
							while(maxfd--)
							{
								if(FD_ISSET(maxfd,&current))
									break;
							}
							FD_CLR(i,&global);
							break;
						}
						break;
					case READ_LIGHT:
						in_queue(socket_read_queue->light_queue,&msg);
#ifdef DEBUG
						printf("recv: %s\n value %lf\n",msg.date,msg.value);
#endif
						sleep(1);
						break;
					case READ_GAS:		
						in_queue(socket_read_queue->gas_queue,&msg);
#ifdef DEBUG
						printf("recv: %s\n value %lf\n",msg.date,msg.value);
#endif
						sleep(1);
						break;
					case READ_SHAKE:
						in_queue(socket_read_queue->shake_queue,&msg);
#ifdef DEBUG
						printf("recv: %s\n value %lf\n",msg.date,msg.value);
#endif
						sleep(1);
						break;
					case LED1_ON:
						if(smart_home_adapter(msg.date,msg.type) == PERMITTED){
						led_ctl(fd,LED1_ON);
						msg.type = PERMITTED;
						send(confd, &msg,sizeof(msg),0);
						}
						else {
							msg.type = NO_PERMISSION;
							send(confd,&msg,sizeof(msg),0);
						}
							
						break;
					case LED1_OFF:
						if(smart_home_adapter(msg.date,msg.type) == PERMITTED){
						led_ctl(fd,LED1_OFF);
						msg.type = PERMITTED;
						send(confd, &msg,sizeof(msg),0);
						}
						else {
							msg.type = NO_PERMISSION;
							send(confd,&msg,sizeof(msg),0);
						}
						
						break;
					case LED2_ON:
						if(smart_home_adapter(msg.date,msg.type) == PERMITTED){
						led_ctl(fd,LED2_ON);
						msg.type = PERMITTED;
						send(confd ,&msg,sizeof(msg),0);
						}
						else {
							msg.type = NO_PERMISSION;
							send(confd,&msg,sizeof(msg),0);
						}
						break;
					case LED2_OFF:
						if(smart_home_adapter(msg.date,msg.type) == PERMITTED){
						led_ctl(fd,LED2_OFF);
						msg.type = PERMITTED;
						send(confd, &msg,sizeof(msg),0);
						}
						else {
							msg.type = NO_PERMISSION;
							send(confd,&msg,sizeof(msg),0);
						}
						break;
					case LED3_ON:
						if(smart_home_adapter(msg.date,msg.type) == PERMITTED){
						led_ctl(fd,LED3_ON);
						msg.type = PERMITTED;
						send(confd ,&msg,sizeof(msg),0);
						}
						else {
							msg.type = NO_PERMISSION;
							send(confd,&msg,sizeof(msg),0);
						}
						break;
					case LED3_OFF:
						if(smart_home_adapter(msg.date,msg.type) == PERMITTED){
						led_ctl(fd,LED3_OFF);
						msg.type = PERMITTED;
						send(confd, &msg,sizeof(msg),0);
						}
						else {
							msg.type = NO_PERMISSION;
							send(confd,&msg,sizeof(msg),0);
						}
						break;
					case LED4_ON:
						if(smart_home_adapter(msg.date,msg.type) == PERMITTED){
						led_ctl(fd,LED4_ON);
						msg.type = PERMITTED;
						send(confd, &msg,sizeof(msg),0);
						}
						else {
							msg.type = NO_PERMISSION;
							send(confd,&msg,sizeof(msg),0);
						}
						break;
					case LED4_OFF:
						if(smart_home_adapter(msg.date,msg.type) == PERMITTED){
						led_ctl(fd,LED4_OFF);
						msg.type = PERMITTED;
						send(confd ,&msg,sizeof(msg),0);
						}
						else {
							msg.type = NO_PERMISSION;
							send(confd,&msg,sizeof(msg),0);
						}
						break;

					default:
						printf("unkown type\n");
						close(i);
						if(i == maxfd)
						{
							while(maxfd--)
							{
								if(FD_ISSET(maxfd,&current))
									break;
							}
							FD_CLR(i,&global);
							break;
						}
						break;	
					}

				}
			}
		}
	}
	#endif
	return 0;
}
int socket_write(int sockfd,sock_msg msg,int device)
{
	int confd,ret,maxfd,i,device_type[20],num=0,confdflag;
	fd_set current,global;
	FD_ZERO(&current);
	FD_ZERO(&global);
	FD_SET(sockfd,&global);
	maxfd = sockfd;
	int len = sizeof(clientaddr);
	while(1)
	{

		current = global;
		ret = select(maxfd + 1,&current,NULL,NULL,NULL);
		if(ret < 0)
		{
			Handle_error("select");
		}
		printf("select success\n");
		if(FD_ISSET(sockfd,&current))
		{
			confd = accept(sockfd,(struct sockaddr *)&clientaddr,&len);
			if(confd < 0)
			{
				Handle_error("accept");
			}
			printf("accept success\n");
			if(confdflag != confd)
			{
				ret = recv(i ,&msg,sizeof(msg),0);
				if(ret< 0)
				{
					Handle_error("recv of write device\n");
				}
				device_type[num++] = msg.type;
				confdflag = confd;
			}
			FD_SET(confd,&global);
			maxfd = confd > maxfd ? confd : maxfd;
		}
		for(i = sockfd + 1;i <= maxfd;i++)
		{
			if(FD_ISSET(i,&current))
			{
				//**************************
				ret = send(i,&msg,sizeof(msg),0);
				//************************
				close(i);
				if(i == maxfd)
				{
					while(maxfd--)
					{
						if(FD_ISSET(maxfd,&current))
							break;
					}
					FD_CLR(i,&global);
					break;
				}
			}
		}

		
	}


	return 0;
}


int cmd_from_socket(int cmd,const char *name)
{
	printf("cmd :%d\nname =%s\n",cmd,name);
	return 0;
}

/*
int main(int argc, const char *argv[])
{
	sensor_queue_t *socket_queue  = malloc(sizeof(sensor_queue_t));
	sock_msg msg;
	device_fd_t device_fd;
	int sockfd;
	sockfd = init_socket("192.168.1.100",5000,10,socket_queue);
	socket_read(sockfd,socket_queue,device_fd);
	return 0;
}

*/
