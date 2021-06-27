#include <unistd.h>
#include <stdio.h>


int main(){

    int i=0;
    int N = 2;
    int x;
    while(i< N){
    pid_t pid = fork();
  
    // If fork() returns zero then it
    // means it is child process.
    if (pid == 0) {
  
        // First child needs to be printed
        // later hence this process is made
        // to sleep for 3 seconds.
        sleep(3);
  
        // This is first child process
        // getpid() gives the process
        // id and getppid() gives the
        // parent id of that process.
     //   printf("child[1] --> pid = %d and ppid = %d\n",
      //         getpid(), getppid());
    }
  
    else {
        pid_t pid1 = fork();
        if (pid1 == 0) {
            sleep(2);
          //  printf("child[2] --> pid = %d and ppid = %d\n",
           //        getpid(), getppid());
        }
        else {

            // If value returned from fork()
            // in not zero and >0 that means
            // this is parent process.
 
            // This is asked to be printed at last
            // hence made to sleep for 3 seconds.
            sleep(3);
           // printf("parent --> pid = %d\n", getpid());
            }
        }        
        i++;
        for(int j=0;j<i;j++){
            printf("-");
        }
         printf(" PID %d: %d \n",i, getpid());
    }

    scanf("%d",&x);
   

    return 0;
}