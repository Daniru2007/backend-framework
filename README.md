# backend-framework

A minimal HTTP server framework written in C. This project exposes a tiny routing API for registering handlers and receiving parsed request metadata.

## Features

- **TCP HTTP server setup** with socket creation, `SO_REUSEADDR | SO_REUSEPORT`, bind, listen, and accept flow.
- **Route registration API** via `add_route(server *web_server, char *path, void (*func)(server *))`.
- **Request-line parsing** for method, URL, and HTTP version.
- **Header parsing** into key/value pairs (`web_server->headers`).
- **Query-string argument parsing** into key/value pairs (`web_server->args`) for matched routes.
- **Per-route callbacks** that receive the active `server` context.
- **Simple built-in HTTP response** (`200 OK` with an HTML body).
- **Connection lifecycle helpers**: create, accept loop, clear request state, and close server.

## Project Structure

```text
.
├── main.c              # Example app wiring routes to handlers
├── server/
│   ├── server.h        # Public data structures and API
│   └── server.c        # Socket, parser, routing, and response implementation
└── backend             # Prebuilt binary (if present)
```

## Build

Compile with GCC:

```bash
gcc -Wall -Wextra -pedantic main.c server/server.c -o backend
```

## Run

```bash
./backend
```

The sample app in `main.c` starts on port `8000` and registers:

- `/something`
- `/something/param` (reads query args in the handler)

## Example Requests

```bash
curl "http://127.0.0.1:8000/something"
curl "http://127.0.0.1:8000/something/param?name=codex&lang=c"
```

## Minimal Usage Example

```c
#include "server/server.h"

void hello(server *web_server) {
  // Access parsed fields:
  // web_server->method, web_server->url, web_server->headers, web_server->args
}

int main() {
  server *app = create_server(8000);
  add_route(app, "/hello", hello);
  accept_clients(app);
  close_server(app);
  return 0;
}
```

## Notes

This is intentionally lightweight and low-level. It is useful as a learning or experimentation base for HTTP parsing and routing in C.
