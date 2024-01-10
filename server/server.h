#ifndef SERVER_H
#define SERVER_H
#include "netinet/in.h"
#include "sys/socket.h"
#include "unistd.h"
#include <asm-generic/socket.h>
#include <sys/socket.h>
#include <sys/types.h>

typedef struct route route;
typedef struct server {
  int server_fd, client_socket;
  struct sockaddr_in address;
  int opt;
  socklen_t addrlen;
  int port;
  char *method;
  char *url;
  char *http_version;
  char *content;
  char buffer[2048];
  char ***headers;
  int header_len;
  route **routes;
  int _route_len;
} server;

typedef struct route {
  char *path;
  void (*func)(server *);
  char **args;
  int _arg_len;
} route;

server *create_server(int port);
void accept_clients(server *web_server);
void close_server(server *web_server);
void add_route(server *web_server, char *path, void (*func)(server *));

#endif
