#ifndef _VIDEO_CHANGE_H_
#define _VIDEO_CHANGE_H_

#include <stdio.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <dirent.h>

#define DIR_PATH  "./video"

extern int get_video();

extern char* change_video_init();
extern char* change_video_next();
extern char* change_video_prev();

#endif