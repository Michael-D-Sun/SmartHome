#ifndef TEMP_H
#define TEMP_H
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>


typedef struct{
	float temp;
}TEMP;

key_t key_temp;
int shmid;
TEMP *shmaddr;

void *mytemp(void *arg){
	key_temp = ftok("/",'w');

	if((shmid = shmget(key_temp, sizeof(TEMP),0666)) < 0)  //获取共享内存段标识符
	{
		perror("fail to shmget");
		exit(EXIT_FAILURE);
	}

	if((shmaddr = (TEMP *)shmat(shmid, NULL, 0)) < 0) //映射共享内存地址
	{
		perror("fail to shmadd");
		exit(EXIT_FAILURE);
	}

	while(1){





	}
	

#if 0
if(shmdt(shmaddr) < 0) //**********删除映射的共享内存地址
{
	perror("fail to shmdt");
	exit(EXIT_FAILURE);
}

if(shmctl(shmid, IPC_RMID, NULL) <0) //**********删除共享内存
{
	perror("fail to shmctl");
	exit(EXIT_FAILURE);
}
#endif
}
#endif
