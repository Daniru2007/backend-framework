#include "server/server.h"
#include "stdio.h"

void func(server *web_server) { printf("FUUUUUCK YOU!\n"); }

int main() {
  server *test_server = create_server(8000);
  add_route(test_server, "/something", func);
  accept_clients(test_server);
  close_server(test_server);
}
