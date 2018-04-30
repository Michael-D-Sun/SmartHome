#include<pthread.h>
#include"mmap.h"
#include"smart_home_cmd.h"
#include <stdio.h>
#include <sys/types.h>	       
#include <sys/socket.h>
#include <netinet/ip.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <stdlib.h>
#include <iostream>
using namespace std;
void *send_sensor_value(void * hehe)
{
	int sockfd;
	sock_msg msg;
	bzero(&msg,sizeof(msg));
	strncpy(msg.data,"Edison",7);
	msg.type = SOCKET_CONNECT_SEND_CMD;
	sockfd = init_socket("192.168.1.100",5000);
	cout<<"connect success & send OK!"<<endl;
	cout<<"msg.type:"<<msg.type<<endl;
	send(sockfd,&msg,sizeof(msg),0);

	struct mmapData* p_mmapData; // here our mmapped data will be accessed
	int fd_mmapFile; // file descriptor for memory mapped file

	/* Create shared memory object and set its size */
	fd_mmapFile = open(mmapFilePath, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	if (fd_mmapFile == -1) exitError("fd error; check errno for details");

	/* Map shared memory object read-writable */
	p_mmapData = static_cast<struct mmapData*>(mmap(NULL, sizeof(struct mmapData), PROT_READ | PROT_WRITE, MAP_SHARED, fd_mmapFile, 0));
	if (p_mmapData == MAP_FAILED) exitError("mmap error");
	/* the Arduino sketch might still be reading - by locking this program will be blocked until the mutex is unlocked from the reading sketch 
	 * in order to prevent race conditions */
	while(1){
		if (pthread_mutex_lock(&(p_mmapData->mutex)) != 0) exitError("pthread_mutex_lock");

		
		bzero(&msg,sizeof(msg));
		strncpy(msg.data,"Edison",7);
		msg.value = static_cast<unsigned long>(p_mmapData->temperature);
		msg.type = READ_TEMPERATURE;
		if(0 == send(sockfd,&msg,sizeof(msg),0))
		{
			close(sockfd);
			exitError("send");
		}
		cout<<"temperature type:"<<msg.type<<endl;
		cout<<"temperature:"<<p_mmapData->temperature<<endl;

		bzero(&msg,sizeof(msg));
		strncpy(msg.data,"Edison",7);
		msg.value = static_cast<unsigned long>(p_mmapData->soundValue);
		msg.type = READ_SOUND;
		if(0 == send(sockfd,&msg,sizeof(msg),0))
		{
			close(sockfd);
			exitError("send");
		}
		cout<<"soundValue type:"<<msg.type<<endl;
		cout<<"soundValue:"<<p_mmapData->soundValue<<endl;

		bzero(&msg,sizeof(msg));
		strncpy(msg.data,"Edison",7);
		msg.value = static_cast<unsigned long>(p_mmapData->vibration);
		msg.type = READ_VIBRATION;
		if(0 == send(sockfd,&msg,sizeof(msg),0))
		{
			close(sockfd);
			exitError("send");
		}
		cout<<"vibration type:"<<msg.type<<endl;
		cout<<"vibration:"<<p_mmapData->vibration<<endl;


		bzero(&msg,sizeof(msg));
		strncpy(msg.data,"Edison",7);
		msg.value = static_cast<unsigned long>(p_mmapData->touch);
		msg.type = READ_TOUCH;
		if(0 == send(sockfd,&msg,sizeof(msg),0))
		{
			close(sockfd);
			exitError("send");
		}
		cout<<"touch type:"<<msg.type<<endl;
		cout<<"touch:"<<p_mmapData->touch<<endl;


		// signal to waiting thread
		if (pthread_mutex_unlock(&(p_mmapData->mutex)) != 0) exitError("pthread_mutex_unlock");
		if (pthread_cond_signal(&(p_mmapData->cond)) != 0) exitError("pthread_cond_signal");
		sleep(1);
	}
	return NULL;
}

void *recv_cmd_type(void *hehe)
{
	int sockfd;
	sock_msg msg;
	bzero(&msg,sizeof(msg));
	strncpy(msg.data,"Edison",7);
	msg.type = SOCKET_CONNECT_READY_CMD;
	sockfd = init_socket("192.168.1.100",5000);
	send(sockfd,&msg,sizeof(msg),0);
	return NULL;

}

