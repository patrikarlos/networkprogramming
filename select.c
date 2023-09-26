#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netinet/in.h>

int main() {
  int server_sockfd, client_sockfd;
  int server_len, client_len;
  struct sockaddr_in server_address;
  struct sockaddr_in client_address;
  int result;
  fd_set readfds, testfds;
  // Create the server side socket
  server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(8888);
  server_len = sizeof(server_address);
  bind(server_sockfd, (struct sockaddr * ) & server_address, server_len);
  // Set the maximum number of listening fds to 5
  listen(server_sockfd, 5);
  FD_ZERO( & readfds);
  // Put the fd of the socket to the fd_set
  FD_SET(server_sockfd, & readfds);
  while (1) {
    char ch;
    int fd;
    int nread;

    // As select will modify the fd_set readfds
    // So we need to copy it to another fd_set testfds
    testfds = readfds;
    printf("Server is waiting\n");

    // Block indefinitely and test file descriptor changes
    // FD_SETSIZE：the system's default number of maximum file descriptors
    result = select(FD_SETSIZE, & testfds, (fd_set * ) 0, (fd_set * ) 0, (struct timeval * ) 0);
    if (result < 1) {
      perror("Failed to select!\n");
      exit(1);
    }

    // Loop all the file descriptors
    for (fd = 0; fd < FD_SETSIZE; fd++) {
      // Find the fd that associated event occurs
      if (FD_ISSET(fd, & testfds)) {
        // Determine if it is a server socket
        // if yes, it indicates that the client requests a connection
        if (fd == server_sockfd) {
          client_len = sizeof(client_address);
          client_sockfd = accept(server_sockfd,
            (struct sockaddr * ) & client_address, & client_len);
          // Add the client socket to the collection
          FD_SET(client_sockfd, & readfds);
          printf("Adding the client socket to fd %d\n", client_sockfd);
        }
        // If not, it means there is data request from the client socket
        else {
          // Get the amount of data to nread
          ioctl(fd, FIONREAD, & nread);

          // After the client data request is completed
          // The socket is closed and the corresponding fd is cleared
          if (nread == 0) {
            close(fd);
            // Remove closed fd
            // (from the unmodified fd_set readfds)
            FD_CLR(fd, & readfds);
            printf("Removing client on fd %d\n", fd);
          }
          // Processing the client data requests
          else {
            read(fd, & ch, 1);
            sleep(5);
            printf("Serving client on fd %d\n", fd);
            ch++;
            write(fd, & ch, 1);
          }
        }
      }
    }
  }

  return 0;
}
