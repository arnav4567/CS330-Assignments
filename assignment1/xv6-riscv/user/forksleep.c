#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    
    if(argc!=3){
        printf("Invalid parameters: The number of arguments should be exactly 2.\n");
        exit(0);
    }
    if(atoi(argv[1])<=0){
        printf("Invalid parameters: m should be positive.\n");
        exit(0);
    }
    if(atoi(argv[2])!=0 && atoi(argv[2])!=1){
        printf("Invalid parameters: n can only be 0 or 1.\n");
        exit(0);
    }
    
    int m = atoi(argv[1]);
    int n = atoi(argv[2]);
    int b = fork();
    if(b<0){
        printf("Fork failed.\n");
        exit(1);
    }
    if(n==0){
        if(b==0){
            sleep(m);
            printf("%d : Child.\n", getpid());
        }
        else{
            printf("%d : Parent.\n", getpid());
            wait((int *)0);
        }
    }
    else{
        if(b==0){
            printf("%d : Child.\n", getpid());
        }
        else{
            sleep(m);
            printf("%d : Parent.\n", getpid());
        }
    }
    exit(0);
}
