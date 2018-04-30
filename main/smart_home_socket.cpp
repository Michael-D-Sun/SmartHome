#include "smart_home_socket.h"
#include "smart_home_queue.h"
#include <stdlib.h>
#include <string>
#include <string.h>
using namespace std;

class Socket{

	public:
		sensor_queue_t *queue;
		Socket(string ip,int port,int num){
			queue = (sensor_queue_t *)malloc(sizeof(sensor_queue_t));
			char cip[30];
			strcpy(cip,ip.c_str());
			this->sockfd = init_socket(cip,port,num,this->queue);
		}
		int AcceptConnect(int sockfd,int *confd)
		{
			return socket_accept(sockfd,confd);
		}

		int readSignalValue(int confd,SensorValue *sensorValue)
		{
			return socket_signal_read(confd,sensorValue);
		}
		int writeSignalCmd(int confd,sock_msg msg)
		{
			return  socket_signal_write(confd,msg);
		}

		int readValue(string str)
		{

			socket_read(sockfd,queue,device_fd);
			if(str == "temperature")
			{
				return queue->temperature_queue->tail->msg.value;
			}
			if(str == "sound")
			{
				return queue->sound_queue->tail->msg.value;
			}
			if(str == "gas")
			{
				return queue->gas_queue->tail->msg.value;
			}
			if(str == "light")
			{
				return queue->light_queue->tail->msg.value;
			}
			if(str == "shake")
			{
				return queue->shake_queue->tail->msg.value;
			}
		}

		int readCMD(){
			socket_read(this->sockfd,this->queue,device_fd);
		}
		void setLedFd(int fd){
			this->device_fd.led_fd = fd;
		}	
		int getSockfd()
		{
			return sockfd;
		}
		int closeConnect(int confd)
		{
			return closeConnect(confd);
		}
		sensor_queue_t * getQueue()
		{
			return queue;
		}
	private:
		device_fd_t device_fd;
		int sockfd;
		sock_msg msg;
};
