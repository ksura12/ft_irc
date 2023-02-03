#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
  // create a socket
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    // handle error
  }

  // bind the socket to a local address and port
  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(10000);
  if (bind(sock, (struct sockaddr *) &address, sizeof(address)) < 0) {
    // handle error
  }

  // continue with socket usage
  // ...

  // close the socket
  close(sock);
  return 0;
}
