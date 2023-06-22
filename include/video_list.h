#ifndef VIDEO_LIST_H
#define VIDEO_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kernel_list.h"

typedef struct _node
{
    char data[64];
    struct list_head list; //内核的小结构体
}node;

extern node* init_list();
extern int destory_list();
extern int print_list();

extern node* init_node();
extern int insert_node(char* data);
extern int delete_node(char* data);
extern node* find_node(char* data);

//附加函数，方便实现切换功能
extern node* change_node_init();
extern node* change_node_next();
extern node* change_node_prev();

#endif