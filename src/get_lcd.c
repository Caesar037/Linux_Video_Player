#include "get_lcd.h"

static int lcd_fd;
static int ts_fd;
static int *mem_p;

int init_lcd(void)
{
    lcd_fd = open(LCD_PATH,O_RDWR);
    if(lcd_fd == -1)
	{
        perror("open");
        return -1;
    }

    //屏幕内存映射
    mem_p = (int*)mmap(NULL,LCD_SIZE*4,PROT_READ|PROT_WRITE,MAP_SHARED,lcd_fd,0);
    if(mem_p == MAP_FAILED)
	{
        perror("mmap");
        return -1;
    }
	printf("init_lcd ok\n");
    return 0;
}


int uninit_lcd(void)
{
    //解除映射
    munmap(mem_p,LCD_SIZE*4);
    mem_p = NULL;
    //关闭文件
    close(lcd_fd);
	printf("uninit_lcd ok\n");
    return 0;
}


int show_bmp_full(const char * const bmp_path)
{
    //1.打开图片
    int bmp_fd = open(bmp_path,O_RDONLY);
    if(bmp_fd == -1)
	{
        perror("open");
        return -1;
    }

    char bmp_buf[LCD_SIZE*3];
    int tmp_buf[LCD_SIZE];

    //2.图片文件光标偏移
    lseek(bmp_fd,54,SEEK_SET);

    //3.读取图片数据
    read(bmp_fd,bmp_buf,sizeof(bmp_buf));

    for(int i=0;i<LCD_SIZE;i++)
	{
        tmp_buf[i] = 0X00<<24 | bmp_buf[i*3+2]<<16 | bmp_buf[i*3+1]<<8 | bmp_buf[i*3];
    }

    for(int y=0;y<LCD_LENGTH;y++)
    {
        for(int x=0;x<LCD_WIDTH;x++)
        {
            //映射好的内存就可以当做LCD来使用,将图片的数据直接赋值到映射好的内存上---相当于写入到LCD显示屏
            mem_p[y*LCD_WIDTH+x] = tmp_buf[(LCD_LENGTH-1-y)*LCD_WIDTH+x];
        }
    }

    close(bmp_fd);
	printf("show_bmp_full ok\n");
    return 0;
}


int show_bmp_anysize(const char * const bmp_path,int start_x,int start_y,int width,int height)
{
    //1.打开图片
    int bmp_fd = open(bmp_path,O_RDONLY);
    if(bmp_fd == -1)
	{
        perror("open");
        return -1;
    }

    if((start_x+width) > LCD_WIDTH && (start_y+height) > LCD_LENGTH)
	{
        printf("error: set start_pos\n");
        return -1;
    }

    char bmp_buf[width*height*3];
    int tmp_buf[width*height];

    //2.图片文件光标偏移
    lseek(bmp_fd,54,SEEK_SET);

    //3.读取图片数据
    read(bmp_fd,bmp_buf,sizeof(bmp_buf));

    //4.把3字节变成4字节
    for(int i=0;i<width*height;i++)
	{
        tmp_buf[i] = 0x00<<24 | bmp_buf[3*i+2]<<16 | bmp_buf[3*i+1]<<8 | bmp_buf[3*i+0];
    }

    //5.将数据映射到LCD显示屏
    for(int y = start_y;y<(start_y+height);y++)
	{
        for(int x = start_x;x<(start_x+width);x++)
		{
            mem_p[y*LCD_WIDTH+x] = tmp_buf[(height-1-(y-start_y))*width+(x-start_x)];
        }
    }

    close(bmp_fd);
	printf("show_bmp_anysize ok\n");
    return 0;
}


int show_background(int colo)
{
    //显示全屏底色
    int i = 0;
    for(i = 0; i < LCD_SIZE; i++)
    {
        mem_p[i] = colo;
    }

    return 0;
}


int progress_bar(int rate,int start_x, int start_y, int width, int height)
{
    int x = 0,y = 0;

    //白色空条
    for( x=start_x; x<(start_x+width); x++)
    {
        for( y=start_y; y<(start_y+height); y++)
        {
            mem_p[y*LCD_WIDTH+x] = WHITE;
        }
    }
    
    for( x=start_x; x<((start_x+width)*rate/100); x++)
    {
        for( y=start_y; y<(start_y+height); y++)
        {
            mem_p[y*LCD_WIDTH + x] = GREEN;
        }
    }

    return 0;
}