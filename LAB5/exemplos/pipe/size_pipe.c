/**
 * Devolve o tamanho padrão do buffer de IPC pipe e fifo.
 * Código baseado em: https://gist.github.com/cyfdecyf/1ee981611050202d670c
 * Tamanho máximo de pipe: /proc/sys/fs/pipe-max-size
 * Documentação pipe: http://man7.org/linux/man-pages/man7/pipe.7.html
 */

#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>                       //mkfifo           
#include </usr/include/linux/fcntl.h>       

#define FIFO_NAME "/tmp/temp_fifo"

int main(int argc, char *argv[]) {
    int pipe_fd[2];
    int fifo_fd;
    
    pipe(pipe_fd);
    
    mkfifo (FIFO_NAME, 0664);
    fifo_fd = open(FIFO_NAME, O_RDONLY | O_NONBLOCK);   // se abrir sem O)NONBLOCK, bloqueia
    
    long pipe_size = (long)fcntl(pipe_fd[0], F_GETPIPE_SZ);
    long fifo_size = (long)fcntl(fifo_fd, F_GETPIPE_SZ);
    
    if (pipe_size == -1) perror("get pipe size failed.");
    if (fifo_size == -1) perror("get fifo size failed.");

    printf("Default pipe size: %ld\n", pipe_size);
    printf("Default fifo size: %ld\n", fifo_size);
    
    close(fifo_fd);
    
} //main
