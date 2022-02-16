#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int pipePToC[2], pipeCToP[2];
    pipe(pipeCToP);
    pipe(pipePToC);
    char buf[64];
    int pid = fork();
    if (pid == 0){// child process
        read(pipePToC[0], buf, 4);
        fprintf(1, "%d: received %s\n", getpid(), buf);
        write(pipeCToP[1], "pong", 4);
    } else {// parent process
        write(pipePToC[1], "ping", 4);
        read(pipeCToP[0], buf, 4);
        fprintf(1, "%d: received %s\n", getpid(), buf);
    }
    exit(0);
}  