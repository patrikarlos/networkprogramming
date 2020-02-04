#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>

main()
{
  int sockfd;

  socklen_t i;
  size_t len;
  size_t t1, t2;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  i = sizeof(len);
  if (getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &len, &i) < 0) {
    perror(": getsockopt");
  }

  printf("receive buffer size = %d\n", len);

  if (getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &len, &i) < 0) {
    perror(": getsockopt");
  }

  printf("send buffer size = %d\n", len);

  if (getsockopt(sockfd, SOL_SOCKET, SO_RCVLOWAT, &len, &i) < 0) {
    perror(": getsockopt");
  }

  printf("rcv low watermark= %d\n", len);

  if (getsockopt(sockfd, SOL_SOCKET, SO_SNDLOWAT, &len, &i) < 0) {
    perror(": getsockopt");
  }

  printf("snd low watermark = %d\n", len);


  t1 =1000000; t2 = sizeof(int);
  if (setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &t1, t2) < 0) {
      perror(": setsockopt");
  }

  i = sizeof(len);
  if (getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &len, &i) < 0) {
    perror(": getsockopt");
  }

  printf("modified receive buffer size = %d\n", len);

 
  t1 = 2; t2 = sizeof(int);
  if (setsockopt(sockfd, SOL_SOCKET, SO_RCVLOWAT, &t1, t2) < 0) {
      perror(": setsockopt");
  }
 
  if (getsockopt(sockfd, SOL_SOCKET, SO_RCVLOWAT, &len, &i) < 0) {
    perror(": getsockopt");
  }

  printf("modified rcv low watermark= %d\n", len);

}
