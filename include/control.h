#ifndef _CONTROL_H_
#define _CONTROL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>

#define INIT_PATH "./bmp/init.bmp"
#define PLAY_PATH "./bmp/play.bmp"
#define EXIT_PATH "./bmp/exit.bmp"
#define AD_PATH "./bmp/ad.bmp"


extern void init_control(void);
extern void uninit_control(void);

extern char* get_cmd(void);
extern void get_ad(void);

extern char* get_percent(void);
extern void show_percent(char * buf);

#endif