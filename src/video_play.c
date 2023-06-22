#include "video_play.h"

#include "video_change.h"
#include "control.h"

pthread_mutex_t mutex;		//用于线程池同步互斥的互斥锁
pthread_cond_t  cond;		//用于让线程池里面的线程睡眠的条件变量

static bool pause_flag = false;

static pthread_t tid1;
static pthread_t tid2;
static pthread_t tid3;

void handler(void *arg)				
{
	pthread_mutex_unlock((pthread_mutex_t *)arg); //防止死锁，所以在这里添加解锁操作
}

void *send_command(void *arg)
{
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL); //使能取消线程
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL); //立即响应
	
	int fd_fifo = *(int *)arg;
	char buf[128] = "";

	while(1)
	{
	    bzero(buf, sizeof(buf));
		strcpy(buf, get_cmd()); //通过触屏得到命令
		
		if(write(fd_fifo, buf, strlen(buf)) != strlen(buf))
			perror("write"); //将命令写入命名管道

		if(strcmp(buf, "quit\n") == 0)
		{
			pthread_cancel(tid3);
			pthread_cancel(tid2);
			pthread_cancel(tid1);
		}

		pthread_mutex_lock(&mutex); //加锁
		if(strcmp(buf, "pause\n") == 0) //暂停后，所有子线程睡眠
		{
			pause_flag = true;
			get_ad();
			pause_flag = false;

			pthread_cond_signal(&cond); //唤醒正在睡眠中的线程
		}
		pthread_mutex_unlock(&mutex); //解锁
	}

}

void *send_percent(void *arg)
{
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL); //使能取消线程
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL); //立即响应
	
	int fd_fifo = *(int *)arg;
	char buf[128] = "";

	while(1)
	{
		pthread_cleanup_push(handler, (void *)&mutex);
		pthread_mutex_lock(&mutex); //加锁

		while(pause_flag == true)	//判断是否没有需要运行的任务		
		{
			pthread_cond_wait(&cond, &mutex);	//让线程睡眠		
		}

		pthread_mutex_unlock(&mutex); //解锁						
		pthread_cleanup_pop(0);	//解除登记取消线程之后所做的函数	


	    bzero(buf, sizeof(buf));
		strcpy(buf, get_percent()); //获取实时进度
		
		if(write(fd_fifo, buf, strlen(buf)) != strlen(buf))
			perror("write"); //将命令写入命名管道

		sleep(1); //降低性能消耗
	}

}


void *recv_percent(void *arg)
{
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL); //使能取消线程
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL); //立即响应

	int *fd_pipe = (int *)arg;
	char buf[128] = "";
	
	while(1)
	{
		pthread_cleanup_push(handler, (void *)&mutex);
		pthread_mutex_lock(&mutex); //加锁

		while(pause_flag == true)	//判断是否没有需要运行的任务		
		{
			pthread_cond_wait(&cond, &mutex);	//让线程睡眠		
		}

		pthread_mutex_unlock(&mutex); //解锁						
		pthread_cleanup_pop(0);	//解除登记取消线程之后所做的函数	


		read(fd_pipe[0], buf, sizeof(buf)); //从无名管道的写端读取信息打印在屏幕上

		// printf("+++++++++++++++++++++++++++++++\n");
		// printf("%s\n", buf); //测试
		// printf("+++++++++++++++++++++++++++++++\n");

		show_percent(buf);

	}
	
}


int play_video(void)
{
	int fd_fifo; //创建有名管道，用于向mplayer发送命令
	int fd_pipe[2]; //创建无名管道，用于从mplayer读取命令

	if(access("./tmp/my_fifo", F_OK) != 0) //判断管道文件是否存在
    {
        if(mkfifo("./tmp/my_fifo", 0664) == -1) //创建有名管道
        {
            perror("mkfifo");
            return -1;
        }
    }
	fd_fifo = open("./tmp/my_fifo", O_RDWR);

	if(pipe(fd_pipe) < 0) //创建无名管道
	{
		perror("pipe");
		return -1;
	}

	pid_t pid;
	pid = fork();
	if(pid < 0)
	{
		perror("fork");
		return -1;
	}
	if(pid == 0) //子进程播放mplayer
	{
		close(fd_pipe[0]);
		dup2(fd_pipe[1], 1); //将子进程的标准输出重定向到管道的写端

		execlp("mplayer", "mplayer", "-slave", "-quiet", "-input", "file=./tmp/my_fifo", "-zoom", "-x", "800", "-y", "380", change_video_init(), NULL);

		exit(0);
	}
	if(pid > 0) //父进程创建用于通信的两个管道
	{
		close(fd_pipe[1]);

		pthread_mutex_init(&mutex, NULL);		//初始化互斥锁
		pthread_cond_init(&cond, NULL);		//初始化条件变量

		pthread_create(&tid1, NULL, send_command, (void *)&fd_fifo); //用户输发送控制指令
		pthread_create(&tid2, NULL, send_percent, (void *)&fd_fifo); //自动发送进度检查指令
		pthread_create(&tid3, NULL, recv_percent, (void *)&fd_pipe[0]); //打印从无名管道收到的信息

		pthread_join(tid1, NULL);
		pthread_join(tid2, NULL);
		pthread_join(tid3, NULL);

		pthread_mutex_destroy(&mutex);
		pthread_cond_destroy(&cond);

		wait(NULL); //等待子进程关闭

		close(fd_fifo);
		close(fd_pipe[0]);

	}

	return 0;
}