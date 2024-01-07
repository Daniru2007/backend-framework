#include "server.h"

server create_server(int opt, int port) {
  server web_server = {};
  web_server.port = port;
  web_server.opt = opt;
  printf("creating server...\n");
  if ((web_server.server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket not created\n");
    exit(1);
  }
  printf("setting options...\n");
  if (setsockopt(web_server.server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                 &web_server.opt, sizeof(web_server.opt))) {
    perror("socket options not setted\n");
    exit(1);
  }
  web_server.address.sin_family = AF_INET;
  web_server.address.sin_addr.s_addr = INADDR_ANY;
  web_server.address.sin_port = htons(port);
  printf("binding the server to PORT %d\n", port);
  if (bind(web_server.server_fd, (struct sockaddr *)&web_server.address,
           sizeof(web_server.address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  return web_server;
}
