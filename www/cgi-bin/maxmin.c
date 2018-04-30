#include"head.h"

extern char *cgiQueryString;
int cgiMain()
{
	MSG msg;
	key_t key;
	int msgid;
	int data[16];

	
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
		cgiFormIntegerBounded("air_temp_max",&data[0],0,40,3);
		msg.mtype = TYPEB; 
		msg.commend = data[0];
		msgsnd(msgid,&msg,MSGLEN,0);

		cgiFormIntegerBounded("air_temp_min",&data[1],0,40,3);
		msg.mtype = TYPEB; 
		msg.commend = data[1];
		msgsnd(msgid,&msg,MSGLEN,0);

		cgiFormIntegerBounded("air_hum_max",&data[2],0,40,3);
		msg.mtype = TYPEB; 
		msg.commend = data[2];
		msgsnd(msgid,&msg,MSGLEN,0);

		cgiFormIntegerBounded("air_hum_min",&data[3],0,40,3);
		msg.mtype = TYPEB; 
		msg.commend = data[3];
		msgsnd(msgid,&msg,MSGLEN,0);

		cgiFormIntegerBounded("soil_temp_max",&data[4],0,40,3);
		msg.mtype = TYPEB; 
		msg.commend = data[4];
		msgsnd(msgid,&msg,MSGLEN,0);

		cgiFormIntegerBounded("soil_temp_min",&data[5],0,40,3);
		msg.mtype = TYPEB; 
		msg.commend = data[5];
		msgsnd(msgid,&msg,MSGLEN,0);
	
		cgiFormIntegerBounded("soil_temp_max",&data[6],0,40,3);
		msg.mtype = TYPEB; 
		msg.commend = data[6];
		msgsnd(msgid,&msg,MSGLEN,0);
	
		cgiFormIntegerBounded("soil_temp_min",&data[7],0,40,3);
		msg.mtype = TYPEB; 
		msg.commend = data[7];
		msgsnd(msgid,&msg,MSGLEN,0);
	
		
		cgiFormIntegerBounded("sun_max",&data[8],0,40,3);
		msg.mtype = TYPEB; 
		msg.commend = data[8];
		msgsnd(msgid,&msg,MSGLEN,0);

		cgiFormIntegerBounded("sun_min",&data[9],0,40,3);
		msg.mtype = TYPEB; 
		msg.commend = data[9];
		msgsnd(msgid,&msg,MSGLEN,0);

	cgiFormIntegerBounded("CO2",&data[10],0,40,3);
		msg.mtype = TYPEB; 
		msg.commend = data[10];
		msgsnd(msgid,&msg,MSGLEN,0);

	cgiFormIntegerBounded("water",&data[11],0,40,3);
		msg.mtype = TYPEB; 
		msg.commend = data[11];
		msgsnd(msgid,&msg,MSGLEN,0);

	cgiHeaderContentType("text/html\n\n");
	fprintf(cgiOut,"<HTML><HEAD>\n");
	fprintf(cgiOut,"<TITLE>Environment cgi</TITLE></HEAD>\n");
	fprintf(cgiOut,"<BODY>");
	fprintf(cgiOut,"<H2>environment date send sucess</H2>\n");

	fprintf(cgiOut,"air_temp_max=%d\n",data[0]);
	fprintf(cgiOut,"air_temp_min=%d\n",data[1]);
	fprintf(cgiOut,"air_hum_max=%d\n",data[2]);
	fprintf(cgiOut,"air_hum_min=%d\n",data[3]);

	fprintf(cgiOut,"<p></p>");


	fprintf(cgiOut,"soil_temp_max=%d\n",data[4]);
	fprintf(cgiOut,"soil_temp_min=%d\n",data[5]);
	fprintf(cgiOut,"soil_hum_max=%d\n",data[6]);
	fprintf(cgiOut,"soil_hum_min=%d\n",data[7]);

//f(cgiOut,"<meta http-equiv=\"refresh\" content=\"1;url=../index.html\">");

	fprintf(cgiOut,"</BODY>\n");
	fprintf(cgiOut,"</HTML>\n");
	
return 0;
}


