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

int cntl_led_from_cgi(int argv)
{		
	int fd_led = argv;
	key_t key;
	int msgid;
	user_info_t msg_buf;

	if((key = ftok("/", 'z')) < 0)
	{
		perror("ftok");
		exit(1);
	}
//	printf("led_key = %d\n",key);
	msgid = msgget(key,IPC_CREAT|IPC_EXCL|0664);
//	printf("led_msgid=%d\n",msgid);
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
	printf("msgid = %d\n", msgid);

// printf("*************************in led thread*************************\n");
while(1)
	{
#if 1
//		msg_buf.type ='L';
		if(0 < msgrcv(msgid ,&msg_buf, sizeof(user_info_t)- sizeof(long), 'L', 0))
		{
//		printf("name = %s\n",msg_buf.name);
//		printf("msg_buf.cmd = %lu\n",msg_buf.cmd);
//		printf("msg_buf.permission = %lu\n",msg_buf.permission);
			switch(msg_buf.cmd)
			{
				printf("LED1_ON = %lu\n",LED1_ON);
				case LED1_ON:
//					printf("LED1_ON\n");
					ioctl(fd_led,LED1_ON);
					break;
				case LED1_OFF:
//					printf("LED1_OFF\n");
					ioctl(fd_led,LED1_OFF);
					break;
				case LED2_ON:
					ioctl(fd_led,LED2_ON);
					break;
				case LED2_OFF:
					ioctl(fd_led,LED2_OFF);
					break;	
				case LED3_ON:
					ioctl(fd_led,LED3_ON);
					break;
				case LED3_OFF:
					ioctl(fd_led,LED3_OFF);
					break;
				case LED4_ON:
					ioctl(fd_led,LED4_ON);
					break;
				case LED4_OFF:
					ioctl(fd_led,LED4_OFF);
					break;
				case LEDALL_ON:
					ioctl(fd_led,LEDALL_ON);
					break;
				case LEDALL_OFF:
					ioctl(fd_led,LEDALL_OFF);
					break;
				default:
//			printf("fuck ARM!\n");
					ioctl(fd_led,LEDALL_ON);
					break;
					
			}
		}
#endif		
	}
	
}
