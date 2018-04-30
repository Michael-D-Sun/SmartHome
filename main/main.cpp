#include "smart_home_C_app_lib.h"
#include "smart_home_cmd.h"
#include "smart_home_socket.h"
#include "smart_home_socket.cpp"
#include "smart_home_queue.h"
#include "smart_home_adapter.h"
#include "smart_home_msg_queue.h"

#include <iostream>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

int gconfd,gsockfd;
sensor_queue_t *socket_queue = NULL;
SensorValue sensor_value;


class Led{
	public:
		Led()
		{
			this->fd = get_led_fd();	
		}

		void setLed(unsigned long cmd)
		{
			led_ctl(this->fd,cmd);
		}

		int get_fd()
		{
			return fd;
		}
	private:
		int fd;

};

class Beep{
	public:
		Beep()
		{
			this->fd = get_beep_fd();
		}
		void setBeep(unsigned long cmd){

			beep_ctl(this->fd,cmd);
		}
		int get_fd()
		{
			return this->fd;
		}
	private:
		int fd;

};
void *SocketSensorRead(void *he)
{
	int mode;
	while(1){

		if(SOCKET_CONNECT_CLOSE == socket_signal_read(gconfd,&sensor_value))
		{
			printf("connect close!\n");
			sleep(1);
			close(gconfd);
			mode = socket_accept(gsockfd,&gconfd);
			printf("new connect accept!\n");
			if(mode == SOCKET_CONNECT_SEND_CMD)
			{
				SocketSensorRead(0);
			}
			else if(mode == SOCKET_CONNECT_READY_CMD)
			{
				break;
			}
		}
		printf("once!\n");
	}
}

int fd_led;
int fd_beep;



int funccc()
{
	return 0;
}

void *cntl_led_cgi(void * argv)
{
	int fd = fd_led;
	cntl_led_from_cgi(fd);
}

void *cntl_beep_cgi(void *argv)
{
	int fd = fd_beep;
	cntl_beep_from_cgi(fd);

}

void *cntl_mpu6050_cgi(void *argv)
{
	cntl_mpu6050_thread();

}

void *cntl_detector_thread(void *argv)
{
	int fd = fd_beep;
	detector_mythread(fd);

}

int main(int argc, const char *argv[])
{
	Led led;
	Beep beep;
	
	fd_led = led.get_fd();

	fd_beep = beep.get_fd();
	int confd,mode;
	pthread_t sensor_tid;
	pthread_t cntl_led;
	pthread_t cntl_beep;
	pthread_t cntl_mpu6050;
	pthread_t cntl_detector;
	
	pthread_create(&cntl_led, NULL, cntl_led_cgi, NULL); //thread to contrl led from cgi
	pthread_create(&cntl_beep, NULL, cntl_beep_cgi, NULL); //thread to contrl beep from cgi
	pthread_create(&cntl_mpu6050,NULL,cntl_mpu6050_cgi, NULL);
	pthread_create(&cntl_detector,NULL,cntl_detector_thread, NULL);
	
	sock_msg msg;
	strncpy(msg.date,"fucker",7);
	Socket *socket = new Socket("192.168.1.100",5000,10);
	cout<<"create socket success"<<endl;
	gsockfd = socket->getSockfd();
//	socket_queue == socket->getQueue();
	while(1)
	{
		mode = socket->AcceptConnect(socket->getSockfd(),&confd);
		if(mode == SOCKET_CONNECT_SEND_CMD)
		{
			gconfd = confd;
			printf("get sensor value Mode!!\n");
			pthread_create(&sensor_tid,NULL,SocketSensorRead,NULL);
		}
	
		else if(mode == SOCKET_CONNECT_READY_CMD)
		{
			printf("get edison cmd\n");
			strncpy(msg.date,"FS4412",7);
			socket->writeSignalCmd(confd,msg);
		}
		else{
			socket->closeConnect(confd);
		}
		pthread_join(sensor_tid,NULL);
	}
	return 0;
	
}

