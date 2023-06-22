#include "video_list.h"

static node* head = NULL; //链表头节点
static struct list_head* pos = NULL; //当前播放视频的位置

//初始化链表
node* init_list()
{
    head = (node*)malloc(sizeof(node));
    if(head == NULL)return NULL;

    bzero(head->data, sizeof(head->data)); //没有给数据域存放数据，那么就可以把链表的这个结点当作头结点
    INIT_LIST_HEAD(&head->list); //调用内核的初始化接口

    return head;
}

//初始化节点
node* init_node()
{
    node* new = (node*)malloc(sizeof(node));
    if(new == NULL)return NULL;

    bzero(new->data, sizeof(new->data)); //没有给数据域存放数据，那么就可以把链表的这个结点当作头结点
    INIT_LIST_HEAD(&new->list); //调用内核的初始化接口

    return new;
}

//查找节点
node* find_node(char* data)
{
    //定义一个大结构体的指针
    node* p = NULL;
    //定义一个内核结构体指针
    struct list_head* pos = NULL;

    //调用内核的接口来遍历链表
    list_for_each(pos, &head->list)
    {
        p = list_entry(pos, node, list); //p就是遍历出来的结点

        //将结点的数据与要查找的数据进行比较
        if(0 == strcmp(p->data, data))
        {
            return p;
        }
    }
    return NULL;
}

//删除节点
int delete_node(char* data)
{
    //在接口中调用已设计好的接口
    node* del = NULL;
    del = find_node(data);

    if(del == NULL)
        return -1; //该链表中没有该数据

    //删除结点
    list_del(&del->list);
    free(del);

    return 0;
}

//插入节点
int insert_node(char* data)
{
    if(find_node(data) != NULL)
		return -1; //如果已经存在则返回

    node* new = init_node(); //创建要插入的新结点
	strcpy(new->data, data); //给大结构的数据域进行赋值

    //调用内核链表的插入标准结点的方法
    list_add(&new->list, &head->list);
    //list_add_tail(&new->list, &head->list);

	return 0;
}

//销毁链表
int destory_list()
{
    //定义一个大结构体的指针
    node* p = NULL;
    //定义一个内核结构体指针
    struct list_head* pos = NULL;

    list_for_each(pos, &head->list)
    {
        list_del(pos);

        p = list_entry(pos,node,list);
        free(p);
    }

    INIT_LIST_HEAD(&head->list);
    free(head);
	
	return 0;
}

//遍历打印
int print_list()
{
    //定义一个大结构体的指针
    node* p = NULL;
    //定义一个内核结构体指针
    struct list_head* pos = NULL;

    list_for_each(pos, &head->list)
    {
        p = list_entry(pos,node,list);
        printf("node: %s\n",p->data);
    }

	return 0;
}


node* change_node_init()
{
	node* p = NULL;

	pos = (&head->list)->next;
	p = list_entry(pos, node, list); //p就是遍历出来的结点

	return p;
}


node* change_node_next()
{
	node* p = NULL;
	
	pos = pos->next;
	if(pos == (&head->list))
		pos = pos->next;
	
    p = list_entry(pos, node, list); //p就是遍历出来的结点
	
    return p;
}


node* change_node_prev()
{
	node* p = NULL;
	
	pos = pos->prev;
	if(pos == (&head->list))
		pos = pos->prev;
	
    p = list_entry(pos, node, list); //p就是遍历出来的结点
	
    return p;
}

