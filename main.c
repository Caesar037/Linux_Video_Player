#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "video_play.h"
#include "control.h"

int main(int argc, char *argv[])
{
    init_control();

    play_video();

    uninit_control();

    return 0;
}

