#ifndef _GET_TOUCH_H_
#define _GET_TOUCH_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//open,lseek
#include <sys/types.h> //usr/include/sys/types.h
#include <sys/stat.h>
#include <fcntl.h>
//close,access,lseek,read,write
#include <unistd.h>
//input_event
#include <linux/input.h>

#define TS_PATH "/dev/input/event0"

#define LCD_LENGTH 	480
#define LCD_WIDTH 	800
#define LCD_SIZE 	480*800

#define TS_LENGTH 	600
#define TS_WIDTH 	1024


typedef struct _coordinate
{
    int x_axis;
    int y_axis;

}Coordinate;


extern int init_ts(void);
extern int uninit_ts(void);

extern int get_coordinate_ts(Coordinate * const coord);
extern int judge_position(const Coordinate * const coord, const int start_x, const int start_y, const int width, const int height);

#endif // GET_TOUCH_H
