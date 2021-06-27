#include <unistd.h>
#include <stdio.h>

int main(){

    int p1 = fork();
    printf(" PID n1: %d \n", getpid());
    
    return 0;
}