#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int primes[]={2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97};

void recur(int pr, int num){
    if(num==1){
        exit(0);
    }
    else{
        int pipefd[2];
        if(pipe(pipefd)<0){
            printf("Pipe couldn't be created. Aborted.\n");
            exit(0);
        }
        int b = fork();
        if(b<0){
            printf("Couldn't fork. Aborted\n");
        }
        else if(b>0){
            int curprime = primes[pr], c=0;
            while(num%curprime==0){
                printf("%d, ", curprime);
                c++;
                num/=curprime;
            }
            if(c) printf("[%d] \n", getpid());

            int towrite = num;
            if(write(pipefd[1], &towrite, sizeof(int)) < 0){
                printf("Error: Cannot write. Aborted\n");
            }
            
            wait((int *)0);

            close(pipefd[1]);
            close(pipefd[0]);
        }
        else{
            int getnum;
            if(read(pipefd[0], &getnum, sizeof(int)) < 0){
                printf("Error: Cannot read. Aborted\n");
            }
            close(pipefd[1]);
            close(pipefd[0]);

            recur(pr+1, getnum);
        }
        exit(0);
    }
}

int main(int argc, char * argv[]){
    if(argc!=2){
        printf("Invalid parameters: The number of arguments should be exactly 1. Please try again.\n");
        exit(0);
    }
    if(atoi(argv[1])<2 || atoi(argv[1])>100){
        printf("Invalid parameters: n should lie in [2, 100]. Please try again.\n");
        exit(0);
    }

    int n = atoi(argv[1]);
    recur(0, n);
    exit(0);
}