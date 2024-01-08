#include "server.h"

server *create_server(int port) {
  server *web_server = (server *)malloc(sizeof(server));
  web_server->port = port;
  web_server->opt = 1;
  printf("creating server...\n");
  if ((web_server->server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket not created\n");
    exit(1);
  }
  printf("setting options...\n");
  if (setsockopt(web_server->server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                 &web_server->opt, sizeof(web_server->opt))) {
    perror("socket options not setted\n");
    exit(1);
  }
  web_server->address.sin_family = AF_INET;
  web_server->address.sin_addr.s_addr = INADDR_ANY;
  web_server->address.sin_port = htons(port);
  printf("binding the server to PORT %d\n", port);
  if (bind(web_server->server_fd, (struct sockaddr *)&web_server->address,
           sizeof(web_server->address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  web_server->addrlen = sizeof(web_server->address);
  return web_server;
}

void _listen(server *web_server) {
  printf("listen\n");
  if (listen(web_server->server_fd, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }
  return;
}

void _accept(server *web_server) {
  printf("accepting\n");
  if ((web_server->client_socket = accept(
           web_server->server_fd, (struct sockaddr *)&web_server->address,
           &web_server->addrlen)) < 0) {
    perror("accept");
    exit(EXIT_FAILURE);
  }
  return;
}

void accept_clients(server *web_server) {
  _listen(web_server);
  while (1) {
    ssize_t valread;
    char buffer[1024] = {0};
    char *msg = "HTTP/1.1 200 OK\r\nContent-Type: "
                "text/html\r\n\r\n<html><body><h1>Hi</h1></body></html>";
    _accept(web_server);
    valread = read(web_server->client_socket, buffer, 1024 - 1);
    char *buf = malloc(sizeof(buffer));
    strcpy(buf, buffer);
    char *tok = strtok(buf, "\n");
    char *method = (char *)malloc(strlen(tok));
    char *url = (char *)malloc(strlen(tok));
    char *http_version = (char *)malloc(strlen(tok));
    sscanf(tok, "%s %s %s", method, url, http_version);
    int i = 0;
    char ***heads = (char ***)malloc(sizeof(char **) * 100);
    while (1) {
      tok = strtok(NULL, "\n");
      heads[i] = (char **)malloc(sizeof(char *) * 100);
      heads[i][0] = (char *)malloc(strlen(tok));
      heads[i][1] = (char *)malloc(strlen(tok));
      if (tok == NULL) {
        break;
      }
      if (strcmp(tok, "\r") == 0) {
        break;
      }
      sscanf(tok, "%[^:]%*c%*c%[^\n]", heads[i][0], heads[i][1]);
      i++;
    }

    for (int x = 0; x < i; x++) {
      printf("%s->", heads[x][0]);
      printf("%s\n", heads[x][1]);
    }

    send(web_server->client_socket, msg, strlen(msg), 0);

    for (int y = 0; y < i; y++) {
      free(heads[y][0]);
      heads[y][0] = NULL;
      free(heads[y][1]);
      heads[y][1] = NULL;
      free(heads[y]);
      heads[y] = NULL;
    }
    free(heads);
    heads = NULL;
    free(buf);
    free(method);
    free(url);
    free(http_version);
    close(web_server->client_socket);
  }
  close(web_server->server_fd);
}
