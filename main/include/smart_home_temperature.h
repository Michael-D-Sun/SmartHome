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

	if((shmid = shmget(key_temp, sizeof(TEMP),0666)) < 0)  //��ȡ�����ڴ�α�ʶ��
	{
		perror("fail to shmget");
		exit(EXIT_FAILURE);
	}

	if((shmaddr = (TEMP *)shmat(shmid, NULL, 0)) < 0) //ӳ�乲���ڴ��ַ
	{
		perror("fail to shmadd");
		exit(EXIT_FAILURE);
	}

	while(1){





	}
	

#if 0
if(shmdt(shmaddr) < 0) //**********ɾ��ӳ��Ĺ����ڴ��ַ
{
	perror("fail to shmdt");
	exit(EXIT_FAILURE);
}

if(shmctl(shmid, IPC_RMID, NULL) <0) //**********ɾ�������ڴ�
{
	perror("fail to shmctl");
	exit(EXIT_FAILURE);
}
#endif
}
#endif
