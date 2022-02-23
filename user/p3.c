#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int
main(int argc, char *argv[])
{
    printf("hello world (pid:%d)\n", (int) getpid());
    int rc = fork();
    if (rc < 0) { // fork failed; exit
        fprintf(2, "fork failed\n");
        exit(1);
    } else if (rc == 0) { // child (new process)
        printf("hello, I am child (pid:%d)\n", (int) getpid());
        char *myargs[3];
        myargs[0] = "echo"; // program: "wc" (word count)
        myargs[1] = "1232131"; // argument: file to count
        myargs[2] = 0; // marks end of array
        exec(myargs[0], myargs); // runs word count
        printf("this shouldn't print out");
    } else { // parent goes down this path (main)
        int wc = wait(0);
        printf("hello, I am parent of %d (wc:%d) (pid:%d)\n",
        rc, wc, (int) getpid());
    }
    exit(0);
}