/*
** talker.c -- a datagram "client" demo
** Originates from http://beej.us/guide/bgnet/examples/talker.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define SERVERPORT "4950"	// the port users will be connecting to

int main(int argc, char *argv[])
{
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;

	if (argc != 3) {
		fprintf(stderr,"usage: talker hostname message\n");
		exit(1);
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if ((rv = getaddrinfo(argv[1], SERVERPORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and make a socket
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("talker: socket");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "talker: failed to create socket\n");
		return 2;
	}

	char myAddress[30];
	char *myAdd=&myAddress;

	struct sockaddr_in local_sin;
	socklen_t local_sinlen = sizeof(local_sin);
	getsockname(sockfd,(struct sockaddr*)&local_sin, &local_sinlen);

	myAdd=inet_ntop(local_sin.sin_family,&local_sin.sin_addr,myAddress,sizeof(myAddress));
	printf("Sending1 from  %s:%d \n", myAdd, ntohs(local_sin.sin_port));

	
	if (connect(sockfd,p->ai_addr, p->ai_addrlen) < 0 ) {
	  perror("talker2: connect .\n");
	  exit(1);
	} 

	

	getsockname(sockfd,(struct sockaddr*)&local_sin, &local_sinlen);

	myAdd=inet_ntop(local_sin.sin_family,&local_sin.sin_addr,myAddress,sizeof(myAddress));
	printf("Sending2 from  %s:%d \n", myAdd, ntohs(local_sin.sin_port));
	
	if ((numbytes = send(sockfd, argv[2], strlen(argv[2]), 0)) == -1) {
		perror("talker2: sendto");
		exit(1);
	}

	
	getsockname(sockfd,(struct sockaddr*)&local_sin, &local_sinlen);

	myAdd=inet_ntop(local_sin.sin_family,&local_sin.sin_addr,myAddress,sizeof(myAddress));
	printf("Sending3 from  %s:%d \n", myAdd, ntohs(local_sin.sin_port));
	
	freeaddrinfo(servinfo);

	printf("talker2: sent %d bytes to %s\n", numbytes, argv[1]);
	close(sockfd);

	return 0;
}
