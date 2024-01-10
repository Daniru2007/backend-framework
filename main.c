#include "server/server.h"
#include "stdio.h"

void func(server *web_server) { printf("FUUUUUCK YOU!\n"); }
void func2(server *web_server) { printf("KISS MY PISS!\n"); }

int main() {
  server *test_server = create_server(8000);
  add_route(test_server, "/something", func);
  add_route(test_server, "/something/shit", func2);
  accept_clients(test_server);
  close_server(test_server);
}
