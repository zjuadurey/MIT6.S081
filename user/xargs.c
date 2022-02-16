#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

#define MSGSIZE 16

int
main(int argc, char *argv[]){
    sleep(10);
    //怎么获得第一个命令的标准化输出（作为xargs后的输入）？
    char buf[MSGSIZE];
    read(0, buf, MSGSIZE);

    //怎么获取自己的命令行参数？
    char *xargv[MAXARG];
    int xargc = 0;
    for (int i = 1; i < argc; i++){
        xargv[xargc++] = argv[i];
    }
    char *p = buf;
    for (int i = 0; i < MSGSIZE; i++){
        if (buf[i] == '\n'){
            int pid = fork();
            if (pid > 0){
                p = &buf[i + 1];
                wait(0);
            } else {
                //用exec执行命令
                buf[i] = 0;
                xargv[xargc] = p;
                xargc++;
                xargv[xargc++] = 0;
                xargc++;

                exec(xargv[0], xargv);
            }
        }
    }

 
    wait(0);
    exit(0);
}