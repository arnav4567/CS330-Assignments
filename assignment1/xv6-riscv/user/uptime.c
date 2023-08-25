#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    
    int res = uptime();
    printf("The uptime of the machine is : %d\n", res);
    exit(0);
}