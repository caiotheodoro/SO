/**
 * Envia mensagens para um processo servidor com sockets UNIX
 * SOURCE: https://troydhanson.github.io/network/Unix_domain_sockets.html
 * With the abstract socket address, you don't need to maintain the socket
 * file (e.g. unlink) in the file system. And there is no file permissions control over it. 
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>

//char *socket_path = "./socket";
char *socket_path = "\0hidden";

int main(int argc, char *argv[]) {
    struct sockaddr_un addr;
    char buf[100];
    int fd,  // descritor do socket
        cl,  // identifica conexao do cliente
        rc;  // bytes recebidos

    if (argc > 1) socket_path=argv[1];

    if ( (fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        exit(-1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    
    if (*socket_path == '\0') {
        *addr.sun_path = '\0';
        strncpy(addr.sun_path+1, socket_path+1, sizeof(addr.sun_path)-2);
    } else {
        strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);
        unlink(socket_path);  // desvincular path se existe
    }

    /* mapeia o socket para o socket_path */
    if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind error");
        exit(-1);
    }

    /* configura para aguardar conexões */
    if (listen(fd, 5) == -1) {
        perror("listen error");
        exit(-1);
    }

    while (1) {
      /* aguarda conexões dos clientes */
      if ( (cl = accept(fd, NULL, NULL)) == -1) {
          perror("accept error");
          continue;
      }

      /* lê dados envidos pelos clientes */
      while ( (rc = read(cl, buf, sizeof(buf))) > 0 ) {
          printf("read %u bytes: %.*s\n", rc, rc, buf);
      }
      
      /* trata erros */
      if (rc == -1) {
          perror("read");
          exit(-1);
      } else if (rc == 0) {
          printf("EOF\n");
          close(cl);
      }
    }

    return 0;
}