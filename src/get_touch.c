#include "get_touch.h"

static int ts_fd;


int init_ts(void)
{
    ts_fd = open(TS_PATH, O_RDONLY);
    if(-1 == ts_fd)
	{
        perror("open");
        return -1;
    }

	printf("init_ts ok\n");
    return 0;
}


int uninit_ts(void)
{
    close(ts_fd);

	printf("uninit_ts ok\n");
    return 0;
}

int get_coordinate_ts(Coordinate * const coord)
{
    struct input_event ts_event;

    while(1)
	{
        memset(&ts_event, 0, sizeof(struct input_event));
        //read默认是阻塞的，当有数据可读，读到数据，阻塞才解除
        read(ts_fd, &ts_event, sizeof(struct input_event));

        if(ts_event.type == EV_ABS) //判断是否为触摸屏事件
		{ 	
            if(ts_event.code == ABS_X) //判断是否为x轴
			{ 	
                coord->x_axis = ts_event.value*(LCD_WIDTH*1.0/TS_WIDTH);
            }
            if(ts_event.code == ABS_Y) //判断是否为y轴
			{
                coord->y_axis = ts_event.value*(LCD_LENGTH*1.0/TS_LENGTH);
            }
        }

        if(ts_event.type == EV_KEY && ts_event.code == BTN_TOUCH && ts_event.value == 0) //松开事件
            return 0; //返回0，表示松开事件

        if(ts_event.type == EV_SYN) //分割成数据包，持续事件检测
            break;
    }

    return 1; //返回1，表示按下事件
}


int judge_position(const Coordinate * const coord, const int start_x, const int start_y, const int width, const int height)
{
    if(coord->x_axis>start_x && coord->x_axis<(start_x+width))
	{
        if(coord->y_axis>start_y && coord->y_axis<(start_y+height))
		{
			printf("position:(%d, %d)\n", coord->x_axis, coord->y_axis);
            return 0;
        }
    }
    return -1;
}
