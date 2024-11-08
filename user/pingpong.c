#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#define RD 0
#define WR 1

int main(int argc,char *argv[])
{
    char buf = 'p';
    char puf;
    int pid;
    int P_pipe[2];
    int C_pipe[2];
    pipe(P_pipe);
    pipe(C_pipe);
    pid = fork();
    if(pid < 0)
    {
        close(P_pipe[RD]);
        close(P_pipe[WR]);
        close(C_pipe[RD]);
        close(C_pipe[WR]);
        printf("fork error!");
        exit(1);
    }
    else if(pid > 0)
    {
        close(P_pipe[RD]);
        close(C_pipe[WR]);
        write(P_pipe[WR],&buf,sizeof(char));
        
        if(read(C_pipe[RD],&puf,sizeof(char)) == sizeof(char))
        {
            printf("%d:received pong\n",getpid());
        }
        close(P_pipe[WR]);
        close(C_pipe[RD]);
        exit(0);
    }
    else
    {
        close(P_pipe[WR]);
        close(C_pipe[RD]);
        write(C_pipe[WR],&buf,sizeof(char));
        
        if(read(P_pipe[RD],&puf,sizeof(char))==sizeof(char))
        {
            sleep(1);
            printf("%d:received ping\n",getpid());
        }
        close(P_pipe[RD]);
        close(C_pipe[WR]);
        exit(0);
    }

    
}