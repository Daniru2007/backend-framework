#include "server/server.h"

int main() {
  server *test_server = create_server(8000);
  accept_clients(test_server);
  close_server(test_server);
}
