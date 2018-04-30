#include"head.h"

int cgiMain()
{
	MSG msg;
	key_t key;
	int msgid;
	
	char name[16];
	cgiFormString("radio",name,16);

	if((key=ftok("/etc",'r')) == -1)
	{
		perror("ftok");
		exit(-1);
	}

	if((msgid=msgget(key,0666 | IPC_CREAT)) < 0)
	{
		perror("msget");
		exit(-1);
	}

	if(strncmp("fengshan_on",name,11 == 0))
	{
		printf("name:%s\n",name);
		msg.mtype = TYPEA; //100
		msg.commend = fan_speed_3;
		msgsnd(msgid,&msg,MSGLEN,0);
	}

	if(strncmp("fengshan_off",name,12) == 0)
	{
		printf("name:%s\n",name);
		msg.mtype = TYPEA; //100
		msg.commend = fan_speed_0;
		msgsnd(msgid,&msg,MSGLEN,0);
	}
return 0;
}


