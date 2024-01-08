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

void _read(server *web_server) {
  ssize_t valread;
  valread = read(web_server->client_socket, web_server->buffer, 1024 - 1);
  char *buf = malloc(sizeof(web_server->buffer));
  strcpy(buf, web_server->buffer);
  char *tok = strtok(buf, "\n");
  web_server->method = (char *)malloc(strlen(tok));
  web_server->url = (char *)malloc(strlen(tok));
  web_server->http_version = (char *)malloc(strlen(tok));
  sscanf(tok, "%s %s %s", web_server->method, web_server->url,
         web_server->http_version);
  web_server->header_len = 0;
  web_server->headers = (char ***)malloc(sizeof(char **) * 100);
  while (1) {
    tok = strtok(NULL, "\n");
    web_server->headers[web_server->header_len] =
        (char **)malloc(sizeof(char *) * 100);
    web_server->headers[web_server->header_len][0] =
        (char *)malloc(strlen(tok));
    web_server->headers[web_server->header_len][1] =
        (char *)malloc(strlen(tok));
    if (tok == NULL) {
      break;
    }
    if (strcmp(tok, "\r") == 0) {
      break;
    }
    sscanf(tok, "%[^:]%*c%*c%[^\n]",
           web_server->headers[web_server->header_len][0],
           web_server->headers[web_server->header_len][1]);
    web_server->header_len++;
  }

  for (int x = 0; x < web_server->header_len; x++) {
    printf("%s->", web_server->headers[x][0]);
    printf("%s\n", web_server->headers[x][1]);
  }
  free(buf);
}

void _clear(server *web_server) {

  for (int y = 0; y < web_server->header_len; y++) {
    free(web_server->headers[y][0]);
    web_server->headers[y][0] = NULL;
    free(web_server->headers[y][1]);
    web_server->headers[y][1] = NULL;
    free(web_server->headers[y]);
    web_server->headers[y] = NULL;
  }
  free(web_server->headers);
  web_server->headers = NULL;
  free(web_server->method);
  free(web_server->url);
  free(web_server->http_version);
  close(web_server->client_socket);
}

void _send(server *web_server) {
  char *msg = "HTTP/1.1 200 OK\r\nContent-Type: "
              "text/html\r\n\r\n<html><body><h1>Hi</h1></body></html>";
  send(web_server->client_socket, msg, strlen(msg), 0);
}

void accept_clients(server *web_server) {
  _listen(web_server);
  while (1) {
    _accept(web_server);
    _read(web_server);
    _send(web_server);
    _clear(web_server);
  }
  close(web_server->server_fd);
  free(web_server);
}
