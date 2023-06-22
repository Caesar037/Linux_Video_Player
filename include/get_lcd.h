#ifndef GET_LCD_H
#define GET_LCD_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//open,lseek
#include <sys/types.h> //usr/include/sys/types.h
#include <sys/stat.h>
#include <fcntl.h>
//close,access,lseek,read,write
#include <unistd.h>
//mmap
#include <sys/mman.h>

#define LCD_PATH "/dev/fb0"

#define LCD_LENGTH 	480
#define LCD_WIDTH 	800
#define LCD_SIZE 	480*800

#define GREEN 0x0000FF00
#define BLACK 0X00000000
#define WHITE 0X00FFFFFF


extern int init_lcd(void);
extern int uninit_lcd(void);

extern int show_bmp_full(const char * const bmp_path);
extern int show_bmp_anysize(const char * const bmp_path,int start_x,int start_y,int width,int height);

extern int show_background(int colo);
extern int progress_bar(int rate,int start_x, int start_y, int width, int height);

#endif // GET_LCD_H
