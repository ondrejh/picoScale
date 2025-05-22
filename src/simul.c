#include "include.h"

#define BUFFER_SIZE 1024

int server_fd, client_fd;
struct sockaddr_in address;
socklen_t addrlen;

void server_init() {
  server_fd = -1;
  client_fd = -1;
  addrlen = sizeof(address);

  printf("Starting server...\n");

  // 1. vytvoreni socketu
  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }
  printf("Socket created OK.\n");

  // 2. nastaveni adresy
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  address.sin_port = htons(PORT);

  // 3. bind
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  printf("Binding OK.\n");

  // 4. listen
  if (listen(server_fd, 3) < 0) {
    perror("listen failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  printf("Listening OK.\n");

  // 5. nastaveni neblokujiciho rezimu
  fcntl(server_fd, F_SETFL, O_NONBLOCK);

  printf("Server is running on port %d.\n", PORT);
}

bool client_connected() {
  if (client_fd < 0) {
    client_fd = accept(server_fd, (struct sockaddr *)&address, &addrlen);
    if (client_fd >= 0) {
      printf("Client connected\n");
      fcntl(client_fd, F_SETFL, O_NONBLOCK);
    }
  }
  return (client_fd >= 0);
}

int client_read(char *buff) {
  if (client_fd >= 0) {
    memset(buff, 0, BUFFER_SIZE);
    ssize_t bytes = read(client_fd, buff, BUFFER_SIZE - 1);
    if (bytes > 0) {
      printf("Client sent: %s\n", buff);
      send(client_fd, buff, strlen(buff), 0);
    } else if (bytes == 0) {
      printf("Client disconnected\n");
      close(client_fd);
      client_fd = -1;
    } else if ((errno != EAGAIN) && (errno != EWOULDBLOCK)) {
      perror("read error");
      close(client_fd);
      client_fd = -1;
    }
    return bytes;
  }
  return 0;
}

void client_write(char *buff, int buflen) {
  send(client_fd, buff, buflen, 0);
}
