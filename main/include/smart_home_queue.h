
#ifndef _SMART_HOME_QUEUE_H_
#define _SMART_HOME_QUEUE_H_
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	unsigned long value;
	int type;
	char date[128];
}sock_msg;
typedef struct queuenode{
	sock_msg msg;
	struct queuenode *next;
}link_queue_node_t;
typedef struct {
	link_queue_node_t *head; // front
	link_queue_node_t *tail; // rear
}linkqueue_t;

#ifdef cplusplus
extern "c" {
#endif

linkqueue_t *creat_linkqueue(void);
int in_queue(linkqueue_t *lqp,const sock_msg *data);
int read_queue(linkqueue_t *lqp,sock_msg *databuf);
int get_head_linkqueue(linkqueue_t *lqp,sock_msg *databuf);
int is_empty_linkqueue(linkqueue_t *lqp);
int clean_linkqueue(linkqueue_t *lqp);
int dis_linkqueue(linkqueue_t *lqp);

#ifdef _DEBUG_SMART_
void print_linkqueue(linkqueue_t *lqp);
#endif


#ifdef cplusplus
}
#endif



#endif
