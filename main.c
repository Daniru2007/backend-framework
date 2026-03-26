#include "server/server.h"
#include "stdio.h"

void func(server *web_server) { printf("Testing\n"); }
void func2(server *web_server) {
  printf("Testing\n");
  printf("%s\n", web_server->args[0][0]);
  printf("%s\n", web_server->args[0][1]);
}

int main() {
  server *test_server = create_server(8000);
  add_route(test_server, "/something", func);
  add_route(test_server, "/something/param", func2);
  accept_clients(test_server);
  close_server(test_server);
}
