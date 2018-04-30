

#include "mmap.h"
#include <stdio.h>
#include <sys/types.h>	       
#include <sys/socket.h>
#include <netinet/ip.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <stdlib.h>
#include "smart_home_cmd.h"
#define Handle_error(msg) do{perror(msg);exit(-1);}while(0)


using namespace std;

void exitError(const char* errMsg) {
  perror(errMsg);
  exit(EXIT_FAILURE);
}




int init_socket(const char *ip,int port)
{
	struct sockaddr_in hostaddr;
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
	ret = connect(sockfd,(struct sockaddr *)&hostaddr,sizeof(hostaddr));
	if(-1 == ret)
	{
		perror("connect");
		exit(EXIT_FAILURE);
	}
	return sockfd;
	
}




int main(int argc, char** argv) {
	pthread_t SensorService,recv_cmd_Service;
	pthread_create(&SensorService,NULL,send_sensor_value,NULL);
	//sleep(1);
	//pthread_create(&recv_cmd_Service,NULL,recv_cmd_type,NULL);
	pthread_join(SensorService,NULL);
	//pthread_join(recv_cmd_Service,NULL);
}
