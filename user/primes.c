#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
//https://zhuanlan.zhihu.com/p/371028049
int isPrime[36];

void printPrime(int* p, int maxNum)
{   //pipe read i
    //print(i), isPrime[i] = 0
    //if j is not prime(divided by i), isPrime[j] = 0
    //
    int num;
    read(p[0], &num, sizeof(num));
    while (isPrime[num] == 0) num++;
    if (num > maxNum) return;
    printf("prime %d\n", num);
    
    for (int i = num; i <= maxNum; i++){
        if (i % num == 0) isPrime[i] = 0;
    }
    num++;
    write(p[1], &num, sizeof(num));
    if (fork() == 0){
        printPrime(p, maxNum);
    }
    wait(0);
    return;
}

int main(int argc, char *argv[])
{
    for (int i = 2; i < 36; i++) isPrime[i] = 1;
    int p[2];
    pipe(p);
    int lowBound = 1;
    write(p[1], &lowBound, sizeof(lowBound));
    printPrime(p, 35);
    close(p[0]);
    close(p[1]);
    exit(0);
    
}  