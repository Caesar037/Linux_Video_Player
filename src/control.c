#include "control.h"
#include "video_change.h"

#include "get_lcd.h"
#include "get_touch.h"

static Coordinate coord;
static int init = 0;

//get_cmd
static char cmd[32] = "";
static int volume = 30;


char* get_cmd(void) //获得操作指令，用户操作
{
    bzero(cmd, sizeof(cmd));

    if(init == 0) //初始化时输入的默认命令
    {
        init = -1;
        sprintf(cmd, "loop 1\nvolume %d 1\n", volume);
        return cmd; //默认循环
    }

    while(1 == get_coordinate_ts(&coord)); //按住直到松手，这是唯一的触屏识别

    if(0 == judge_position(&coord, 0, 360, 800, 40)) //改变进度
    {
        sprintf(cmd, "seek %d 1\n", coord.x_axis*100/LCD_WIDTH);
        printf("%s\n", cmd);

        return cmd;
    }

    if(0 == judge_position(&coord, 100, 400, 100, 80)) //切换上一个
    {
        init = 0;
        sprintf(cmd, "loadfile %s\n", change_video_prev());
        printf("%s\n", cmd);

        return cmd;
    }
    if(0 == judge_position(&coord, 200, 400, 100, 80)) //音量减
    {
        volume -= 10;
        if(volume < 0)
        {
            volume = 0;
        }
        sprintf(cmd, "volume %d 1\n", volume);
        printf("%s\n", cmd);

        return cmd;
    }
    if(0 == judge_position(&coord, 300, 400, 100, 80)) //暂停
    {
        strcpy(cmd, "pause\n");
        printf("%s\n", cmd);

        return cmd;
    }
    if(0 == judge_position(&coord, 400, 400, 100, 80)) //音量加
    {
        volume += 10;
        if(volume > 100)
        {
            volume = 100;
        }
        sprintf(cmd, "volume %d 1\n", volume);
        printf("%s\n", cmd);

        return cmd;
    }
    if(0 == judge_position(&coord, 500, 400, 100, 80)) //切换下一个
    {
        init = 0;
        sprintf(cmd, "loadfile %s\n", change_video_next());
        printf("%s\n", cmd);

        return cmd;
    }
    if(0 == judge_position(&coord, 700, 400, 100, 80)) //退出
    {
        strcpy(cmd, "quit\n");
        printf("%s\n", cmd);

        return cmd;
    }

    return "\n";
}


char* get_percent(void) //获取进度，非用户操作，需要实时输入
{
   return "get_percent_pos\n";
}


void get_ad(void) //获取进度，非用户操作，需要实时输入
{
    sleep(1);
    while(1)
    {
        show_bmp_full(AD_PATH);
        show_bmp_anysize(PLAY_PATH, 0, 380, 800, 100);

        while(1 == get_coordinate_ts(&coord)); //按任意位置住直到松手
        if(0 == judge_position(&coord, 300, 400, 100, 80)) //暂停
        {
            return;
        }
    }

}


void show_percent(char * buf) //显示百分比
{
    int pos = 0;
    if(strncmp(buf, "ANS_PERCENT_POSITION", strlen("ANS_PERCENT_POSITION")) == 0)
    {
        sscanf(buf, "ANS_PERCENT_POSITION=%d", &pos);
        progress_bar(pos, 0, 380, 800, 20);
    }

    return;
}


void init_control(void)
{
    init_lcd();
    init_ts();

    get_video();
    change_video_init();

    show_bmp_full(INIT_PATH);
    sleep(3);
    show_bmp_anysize(PLAY_PATH, 0, 380, 800, 100);

    return;
}


void uninit_control(void)
{
    show_bmp_full(EXIT_PATH);

    uninit_ts();
    uninit_lcd();
    
    return;
}