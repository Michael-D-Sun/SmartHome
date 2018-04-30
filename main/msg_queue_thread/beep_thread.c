#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/ioctl.h>
#include <sys/msg.h>

#include "smart_home_adapter.h"
#include "smart_home_cmd.h"

int cntl_beep_from_cgi(int argv)
{	
	key_t key;
	int fd_beep = argv;
	int msgid;
	user_info_t msg_buf;
	
	if((key = ftok("/", 'z')) < 0)
	{
		perror("ftok");
		exit(1);
	}
//	printf("beep_key = %d\n",key);
	msgid = msgget(key,IPC_CREAT|IPC_EXCL|0664);
//	printf("beep_msgid = %d\n",msgid);
	if(msgid < 0)
	{
		if(errno == EEXIST)
		{
			msgid = msgget(key,0664);
		}
		else
		{		
			perror("msgget");
			exit(1);
		}
	}
	while(1)
	{
//		msg_buf.type = 'B';
		if(0 < msgrcv(msgid ,&msg_buf, sizeof(user_info_t)- sizeof(long), 'B', 0))
		{
			switch(msg_buf.cmd)
			{
				case BEEP_ON:
					ioctl(fd_beep,BEEP_ON);
					break;
				case BEEP_OFF:
					ioctl(fd_beep,BEEP_OFF);
					break;
				default:
					break;
				
			}
		}	
	}
	
}
