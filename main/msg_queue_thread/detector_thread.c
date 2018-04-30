#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/ioctl.h>
#include <sys/shm.h>

#include "smart_home_adapter.h"
#include "smart_home_cmd.h"
#include "smart_home_socket.h"



typedef struct{
	int temperature_max;
	int temperature_min;
	
}ENV;


int detector_mythread(int argv)
{	
	int fd;
	int shmid;
	char buf_temp[256];
	key_t key;
	SensorValue *shm;
	key_t key_temp;
	ENV *msg;
	int shmid_temp;	
//从共享内存中获取采集的数据	
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
	
	//从共享内存中获取设定的温度值		
	if((key_temp = ftok("/dev", 't')) < 0)
	{
		perror("ftok");
		exit(1);
	}

	shmid_temp = shmget(key_temp, sizeof(ENV), IPC_CREAT|IPC_EXCL|0664);
	
	if(shmid < 0)
	{
		if(errno == EEXIST)
		{
			shmid_temp = shmget(key_temp,sizeof(ENV), 0664);
		}
		else
		{		
			perror("shmget");
			exit(1);
		}
	}
	msg = shmat(shmid_temp, NULL, 0);
	
	msg->temperature_max = 100;
	msg->temperature_min = 20;
	
	while(1)
	{
		fd = open("/flite/temperature2.txt",O_RDWR|O_CREAT|O_TRUNC,0664);
		if(shm->temperatureValue > msg->temperature_max || shm->temperatureValue < msg->temperature_min)
		{
			ioctl(argv, BEEP_ON);
			sleep(5);
			ioctl(argv, BEEP_OFF);
		}
		sprintf(buf_temp,"The   temperature   now   is  %d  degree.  ",(int)shm->temperatureValue);
		write(fd,buf_temp,sizeof(buf_temp));
		memset(buf_temp, 0, sizeof(buf_temp));
		sprintf(buf_temp,"The   sound   now   is   %d   db.  ",(int)shm->soundValue - 180);
		write(fd,buf_temp,sizeof(buf_temp));
		
		system("flite -f /flite/temperature2.txt");	
		sleep(20);
		close(fd);
	}

	close(fd);
	
}
