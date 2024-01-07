#ifndef SERVER_H
#define SERVER_H
#include "netinet/in.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "sys/socket.h"
#include "unistd.h"
#include <asm-generic/socket.h>
#include <sys/socket.h>
#include <sys/types.h>

typedef struct server {
  int server_fd;
  struct sockaddr_in address;
  int opt;
  socklen_t addrlen;
  int port;
} server;

server create_server(int opt, int port);

#endif
