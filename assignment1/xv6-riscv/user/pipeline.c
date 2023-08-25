#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void recur(int n, int pro, int sum){
    if(pro==n){
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
            printf("The current pid is : [%d] and x is %d\n", getpid(), sum+getpid());

            int towrite = sum+getpid();

            if(write(pipefd[1], &towrite, sizeof(int)) < 0){
                printf("Error: Cannot write. Aborted\n");
            }
            wait((int *)0);

            close(pipefd[1]);
            close(pipefd[0]);
        }
        else{
            int getsum;
            if(read(pipefd[0], &getsum, sizeof(int)) < 0){
                printf("Error: Cannot read. Aborted\n");
            }
        
            close(pipefd[1]);
            close(pipefd[0]);

            recur(n-1, pro, getsum);
        }
    }
}

int main(int argc, char * argv[]){

    if(argc!=3){
        printf("Invalid parameters: The number of arguments should be exactly 2. Please try again.\n");
        exit(0);
    }
    if(atoi(argv[1])<=0){
        printf("Invalid parameters: m should be positive. Please try again.\n");
        exit(0);
    }
    int n = atoi(argv[1]);
    int x = atoi(argv[2]);

    recur(n, 0, x);
    exit(0);
}
