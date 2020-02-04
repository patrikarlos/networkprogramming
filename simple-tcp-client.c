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
#define MAXDATASIZE 1400

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


int main(int argc, char *argv[])
{
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;
	char buf[MAXDATASIZE];
	char s[INET6_ADDRSTRLEN];

	if (argc != 2) {
	  fprintf(stderr,"usage: %s hostname (%d)\n",argv[0],argc);
	  exit(1);
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

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

	if (connect(sockfd,p->ai_addr, p->ai_addrlen) < 0 ) {
	  perror("talker2: connect .\n");
	  exit(1);
	}

	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
		  s, sizeof s);
	printf("client: connecting to %s\n", s);


	freeaddrinfo(servinfo);

	if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
	  perror("recv");
	  exit(1);
	}
	
	buf[numbytes] = '\0';
	
	printf("client: received '%s'\n",buf);
	char lineBuffer[MAXDATASIZE];
	char *lBptr=(char*)&lineBuffer;
	size_t buffSize = MAXDATASIZE;
	int lread;
	while(1){
	  printf(":>");
	  lread=getline(&lBptr, &buffSize,stdin);
	  // Trunkate newline
	  lineBuffer[lread-1]='\0';
	  if(strcmp(lineBuffer,"exit")==0){
	    printf("You'r leaving.\n");
	    break;
	  }
	    
	  printf("read %d bytes, buffer = '%s' \n",lread, lineBuffer);
	  if ((numbytes = send(sockfd, lineBuffer, lread, 0)) == -1) {
	    perror("sendto:");
	    exit(1);
	  }
	  printf("sent %d bytes\n", numbytes);

	  if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
	    perror("recv");
	    exit(1);
	  }
	  
	  buf[numbytes] = '\0';
	  
	  printf("client: received '%s'\n",buf);
	}
	close(sockfd);

	return 0;
}
