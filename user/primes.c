#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define RD 0
#define WR 1
/*
读左边管道的第一个数字
*/
int read_first(int lp[2],int *first)
{
    if(read(lp[RD],first,sizeof(int)) == sizeof(int))
    {
        printf("prime %d\n",*first);//第一个都是素数，所以输出
        return 0;
    }
    return -1;
} 
/*
将左边管道的所有数与第一个数计算，过滤掉一个数的倍数数字，剩下的写进右边管道给子进程
*/
void trans_data(int lpipe[2],int rpipe[2],int first)
{
    int data;
    
    while(read(lpipe[RD],&data,sizeof(int)) == sizeof(int))
    {
        if(data%first)
        {
            write(rpipe[WR],&data,sizeof(int));
        }
    }
    close(lpipe[RD]);
    close(rpipe[WR]);
}
void primes(int lp[2])
{
    int first;
    close(lp[WR]);//可以关闭写端，因为后面创建的子进程不会再用这个管道进行写。
    if(read_first(lp,&first) == 0)
    {
        int p[2];
        pipe(p);
        trans_data(lp,p,first);
        if(fork() == 0)
            primes(p);//递归
        else
        {
            close(p[RD]);//记得结束后要关闭释放资源
            wait(0);
        }
    }  
    exit(0);

}


int main(int argc,char *argv[])
{
    int pid;
    int p[2];
    pipe(p);
    // close(p[RD]);//创建子进程之前关闭读端，子进程会继承父进程的文件描述符，影响子进程读
    for(int i = 2;i<=35;i++)
        write(p[WR],&i,sizeof(int));
    pid = fork();
    if(pid < 0)
    {
        printf("fork() error");
        exit(1);
    }
    else if(pid == 0)
    {
        primes(p);
    }
    else
    {
        close(p[RD]);//资源释放
        close(p[WR]);
        wait(0);
    }
    exit(0);
}