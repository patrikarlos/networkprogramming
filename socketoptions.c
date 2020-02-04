#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
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
    perror(": getsockopt SO_RCVBUF");
  } else {
	printf("receive buffer size = %d\n", len);
  }
  if (getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &len, &i) < 0) {
    perror(": getsockopt SO_SNDBUF");
  } else {
	printf("send buffer size = %d\n", len);
  }	

  if (getsockopt(sockfd, SOL_SOCKET, SO_RCVLOWAT, &len, &i) < 0) {
    perror(": getsockopt SO_RCVLOWAT");
  } else {
	printf("rcv low watermark= %d\n", len);
  }
  
  if (getsockopt(sockfd, SOL_SOCKET, SO_SNDLOWAT, &len, &i) < 0) {
    perror(": getsockopt SO_SNDLOWAT");
  } else {
	printf("snd low watermark = %d\n", len);
  }

  t1 =1000000; t2 = sizeof(int);
  if (setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &t1, t2) < 0) {
      perror(": setsockopt SO_RCVBUF");
  }

  i = sizeof(len);
  if (getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &len, &i) < 0) {
    perror(": getsockopt SO_RCVBUF ");
  } else {
	printf("modified receive buffer size = %d\n", len);
  }
 
  t1 = 2; t2 = sizeof(int);
  if (setsockopt(sockfd, SOL_SOCKET, SO_RCVLOWAT, &t1, t2) < 0) {
      perror(": setsockopt SO_RCVLOWAT");
  } else {
	 
	  if (getsockopt(sockfd, SOL_SOCKET, SO_RCVLOWAT, &len, &i) < 0) {
		perror(": getsockopt SO_RCVLOWAT");
	  } else {
		printf("modified rcv low watermark= %d\n", len);
	  }
  }

  
  if (getsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &len, &i) < 0) {
    perror(": getsockopt SO_TCPNODELAY");
  } else {
	printf("TCPNODELAY= %d\n", len);
  }

  if (getsockopt(sockfd, IPPROTO_TCP, TCP_CORK, &len, &i) < 0) {
    perror(": getsockopt SO_TCPNODELAY");
  } else {
	printf("TCPCORK= %d\n", len);
  }

  
  
}
