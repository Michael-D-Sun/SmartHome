#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>

#include "sqlite3.h"
#include "mysqlite.h"
#include "cgic.h"

int cgiMain(int argc, const char *argv[])
{
	msg_t msg;
	char pim[20];
	memset(&msg, 0, sizeof(msg_t));

	cgiFormString("username", msg.name, sizeof(msg.name));
	cgiFormString("userpasswd", msg.pswd, sizeof(msg.pswd));
	cgiFormString("userpermission", pim, sizeof(pim));

	if(strncmp(pim,"BEEP",4))
	{
		msg.permission = (0xff<<6)|(11<<2);
	}
	else{
		msg.permission = 0xff << 6;
	}
	
//	msg_t.mtype = 100;
	if(0 == smart_home_change_permission(&msg)) 
	{
//		smart_home_change_permission(&msg);
		
		cgiHeaderContentType("text/html\n\n");
		fprintf(cgiOut,"<HTML><HEAD>\n");
		fprintf(cgiOut,"<TITLE>修改密码提示</TITLE></HEAD>\n");
		fprintf(cgiOut,"<BODY background='../images/p5.gif' align='center'>");
		fprintf(cgiOut,"<H1>修改权限成功<H1/>\n");
		fprintf(cgiOut,"<meta http-equiv=\"refresh\" content=\"1;url=../manage.html\">");

		fprintf(cgiOut,"</BODY>\n");
		fprintf(cgiOut,"</HTML>\n");
	}
	else{
		cgiHeaderContentType("text/html\n\n");
		fprintf(cgiOut,"<HTML><HEAD>\n");
		fprintf(cgiOut,"<TITLE>修改密码提示</TITLE></HEAD>\n");
		fprintf(cgiOut,"<BODY background='../images/p5.gif' align='center'>");
		fprintf(cgiOut,"<H1>修改权限失败，请确认登陆密码是否正确<H1/>\n");
		fprintf(cgiOut,"<meta http-equiv=\"refresh\" content=\"1;url=../manage.html \">");

		fprintf(cgiOut,"</BODY>\n");
		fprintf(cgiOut,"</HTML>\n");
		return -1;
	}
	return 0;
}

