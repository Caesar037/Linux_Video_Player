#include "video_change.h"
#include "video_list.h"


int get_video()
{
	DIR *dir;
	struct dirent *data;

	char name[64] = {0};

	init_list();
	
	//1.打开目录
	dir = opendir(DIR_PATH);
	if(NULL == dir)
	{
		perror("opendir");
		return -1;
	}
	
	//2.读取目录
	while(1)
	{
		data = readdir(dir);		
		if(NULL == data)
		{
			break; //读完退出  
		}
		
		if(data->d_name[0] == '.')
		{
			continue; //跳过隐藏文件
		}

		//data->d_name //src.txt
		char buf1[64] = {0};
		char buf2[64] = {0};
		sscanf(data->d_name, "%[^.].%s", buf1, buf2); //buf1--src buf2--txt
		
		if(strcmp(buf2,"mp4") == 0)
		{
			bzero(name, sizeof(name));
			sprintf(name, "%s/%s", DIR_PATH, data->d_name);
			
			insert_node(name);
		}

	}
	closedir(dir);

	return 0;
}


char* change_video_init()
{
	node* p = NULL;
	p = change_node_init();

	if(p != NULL)
	{
		//printf("first: %s\n", p->data);
		return p->data;
	}

    return NULL;
}


char* change_video_next()
{
	node* p = NULL;
	p = change_node_next();

	if(p != NULL)
	{
		//printf("next: %s\n", p->data);
		return p->data;
	}

    return NULL;
}


char* change_video_prev()
{
	node* p = NULL;
	p = change_node_prev();

	if(p != NULL)
	{
		//printf("prev: %s\n",p->data);
		return p->data;
	}

    return NULL;
}