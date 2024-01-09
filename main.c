#include "server/server.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int main() {
  server *test_server = create_server(8000);
  accept_clients(test_server);
  close_server(test_server);
}
