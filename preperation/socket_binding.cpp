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








#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

// ...

int server_socket, client_socket;
struct sockaddr_in client_address;
socklen_t client_len = sizeof(client_address);

// ...

while (1) {
  client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_len);
  if (client_socket < 0) {
    // error handling
  }

  // Use the client_socket for communication with the client
  char buffer[1024];
  int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
  if (bytes_received < 0) {
    // error handling
  }

  // Echo the received data back to the client
  int bytes_sent = send(client_socket, buffer, bytes_received, 0);
  if (bytes_sent < 0) {
    // error handling
  }

  close(client_socket);
}