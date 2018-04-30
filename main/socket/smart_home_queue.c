
#include "smart_home_queue.h"

linkqueue_t *creat_linkqueue()
{
	//创建保存头尾结点的地址的空间
	//创建 头结点
	linkqueue_t *lqp;
	link_queue_node_t *head;
	lqp = malloc(sizeof(linkqueue_t));
	head = malloc(sizeof(link_queue_node_t));
	head->next = NULL;

	lqp->head = head;
	lqp->tail = head;
	
	return lqp;
}

int in_queue(linkqueue_t *lqp,const sock_msg *data)
{
	//执行 尾部插入
	link_queue_node_t *newnode;
	newnode = malloc(sizeof(link_queue_node_t));
	newnode->msg = *data;
	newnode->next = NULL;//lqp->tail->next;
	lqp->tail->next = newnode;
	//保存新的尾部地p
	lqp->tail = newnode;

	return 0;
}

int read_queue(linkqueue_t *lqp,sock_msg *databuf)
{
	//头部删除并拷贝数据
	link_queue_node_t *temp;
	if(NULL == lqp->head->next)
		return -1;
	temp = lqp->head->next;
	lqp->head->next = temp->next;

	*databuf = temp->msg;
	free(temp);
	//如果队列空：还原尾部指针地址
	
	if(NULL == lqp->head->next)
		lqp->tail = lqp->head;
	
	return 0;
}

int get_head_linkqueue(linkqueue_t *lqp,sock_msg *databuf)
{
	if(NULL == lqp->head->next)
		return -1;
	*databuf = lqp->head->next->msg;

	return 0;
}

int is_empty_linkqueue(linkqueue_t *lqp)
{
	
	return NULL == lqp->head->next;

}

int clean_linkqueue(linkqueue_t *lqp)
{
	link_queue_node_t *temp;
	while(!is_empty_linkqueue(lqp)){
		temp = lqp->head->next;
		lqp->head->next = temp->next;

		free(temp);
	}

	return 0;

}

int dis_linkqueue(linkqueue_t *lqp)
{
	clean_linkqueue(lqp);
	free(lqp->head);
	free(lqp);

	return 0;
}

void print_linkqueue(linkqueue_t *lqp)
{
	link_queue_node_t *head;

	head = lqp->head;

	printf("head:");
	while(NULL != head->next){
		head = head->next;
		printf("%f ",head->msg.value);
	}
	printf(":tail\n");
	return ;
}


