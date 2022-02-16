#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int pipeline_1[36][2];
    int pipeline_2[36][2];
    
    int isPrime[36];
    
    memset(isPrime, 1, 36);

    for (int i = 2; i <= 35; i++){
        pipe(pipeline_1[i]);
        pipe(pipeline_2[i]);
    }

    for (int i = 2; i <= 35; i++){
        if (isPrime[i]){
            for (int j = i; j <= 35; j++)
            {
                if (j == i){
                    printf("prime %d\n", j);
                    close(pipeline_1[j][0]);
                    close(pipeline_1[j][1]);
                }
                if (isPrime[j] && j != i){
                    if (j%i == 0){//j is divided by i
                        isPrime[j] = 0;
                        close(pipeline_1[j][0]);
                        close(pipeline_1[j][1]);
                    } else {
                        
                    }
                }
            }
        }
    }
    
    exit(0);
}  