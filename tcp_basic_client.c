/*
** client.c -- a stream socket client demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT "4950" // the port client will be connecting to 
#define MAXDATASIZE 1401 // max number of bytes we can get at once 

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	if (sa->sa_family == AF_INET6) {
	  	return &(((struct sockaddr_in6*)sa)->sin6_addr);
	}
	printf("Unknown FAMILY!!!!\n");
	return(0); 
}

int main(int argc, char *argv[])
{
    int sockfd, numbytes;  
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];
    char trunk[]="[Trunkated]\0"; 
      

    
    if (argc != 2) {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }
	printf("Connection successfull.\n");
        break;
    }
    /* Why are we here??? p == NULL, Looped no match (socket &| connect).. 
       Success?? */
    
    if (p == NULL) {
        fprintf(stderr, "client: failed to socket||connect\n");
	/* Clear servinfo */
	freeaddrinfo(servinfo); // all done with this structure
        return 2;
    }

    int q=inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),s, sizeof s);
    /* q ok or not */
    printf("client: connected to %s:%s\n", s,PORT);

    freeaddrinfo(servinfo); // all done with this structure
    int totalBytes=0;

    /* -------------------------------------------- */
    numbytes=send(sockfd,trunk,sizeof(trunk),0);
    printf("Sent %d bytes to server\n",numbytes);
    while(1){
      numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0);
      if(numbytes == -1 ){
        perror("recv");
	/* close socket */
        exit(1);
      }
      if (numbytes == 0) {
	printf("Server closed.\n");
	break;
      }
      
      totalBytes+=numbytes;
      if (numbytes>10) {
	memcpy(&buf[10],trunk,strlen(trunk)+1);
      } else {
	buf[numbytes] = '\0';
      }
      
      printf("client (%d/%d) : received '%s'\n",numbytes, totalBytes, buf);
      /*      sleep(2); */
      numbytes=send(sockfd,trunk,sizeof(trunk),0);
    }
    close(sockfd);

    return 0;
}
