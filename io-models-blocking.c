/*
** io-models-blocking.c
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <sys/time.h>

#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// Enable if you want debugging to be printed, see examble below.
// Alternative, pass CFLAGS=-DDEBUG to make, make CFLAGS=-DDEBUG
#define DEBUG

#define MAXDATA 65000


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
    struct addrinfo hints, *servinfo, *p;
    int rv;

    char buffer[MAXDATA];
    char s[INET6_ADDRSTRLEN];
    
    if (argc != 2) {
      fprintf(stderr,"usage: client hostname:port\n");
      exit(1);
    }

    
 /*
    Read first input, assumes <ip>:<port> syntax, convert into one string (Desthost) and one integer (port). 
     Atm, works only on dotted notation, i.e. IPv4 and DNS. IPv6 does not work if its using ':'. 
  */
  char delim[]=":";
  char *Desthost=strtok(argv[1],delim);
  char *Destport=strtok(NULL,delim);
  // *Desthost now points to a sting holding whatever came before the delimiter, ':'.
  // *Dstport points to whatever string came after the delimiter. 

  /* Do magic */
  int DestportNum=atoi(Destport);
#ifdef DEBUG 
  printf("Host %s, and port %d.\n",Desthost,DestportNum);
#endif
    
 
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(Desthost, Destport, &hints, &servinfo)) != 0) {
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
    printf("client: connected to %s:%s\n", s,Destport);

    freeaddrinfo(servinfo); // all done with this structure
    int totalBytes=0;

    char outBuffer[60];
    char outASCIIBuffer[60];
    struct timeval then, now,diff;
     
    while(1){
      memset(&buffer, 0, MAXDATA);
      memset(&outASCIIBuffer, 0, 60);
      memset(&outBuffer, 0, 60);
      gettimeofday(&then, NULL);
      numbytes = recv(sockfd, buffer, MAXDATA-1, 0);
      gettimeofday(&now, NULL);
      timersub(&now,&then,&diff);
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
      memcpy(&outBuffer,&buffer,20);
      
      printf("client (%d/%d)  %ld.%06ld : received \n",numbytes, totalBytes, diff.tv_sec,diff.tv_usec );
      for(int i=0; i<10;i++){
	if( isprint(outBuffer[i])) {
	  printf("%c    ",outBuffer[i]);
	} else {
	  printf("-    ");
	}
	if(i==0){
	  sprintf(&outASCIIBuffer,"0x%02x",outBuffer[i]);
	} else {
	  sprintf(&outASCIIBuffer,"%s 0x%02x",outASCIIBuffer,outBuffer[i]);
	}
      }
      
      printf("\n%s \n",outASCIIBuffer);
      
      /*      sleep(2); */
    }
    close(sockfd);

    return 0;
}
