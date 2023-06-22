#ifndef _VIDEO_PLAY_H_
#define _VIDEO_PLAY_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <string.h>
#include <stdbool.h>
#include <pthread.h>

extern int play_video(void);

#endif