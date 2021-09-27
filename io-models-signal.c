/*
** io-models-blocking.c
** ref; https://www.cs.tau.ac.il/~eddiea/samples/Signal-Driven/udp-signal-driven-server.c.html
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

#include <fcntl.h>
#include <signal.h>

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

/* Needs to be global as to be reached by handler + main */
int sockfd, numbytes; 
int totalBytes;
char buffer[MAXDATA];


int io_handler(int sig){
  printf("io_handler %d \n", sig);


  char outBuffer[60];
  char outASCIIBuffer[60];
    
  memset(&buffer, 0, MAXDATA);
  memset(&outASCIIBuffer, 0, 60);
  memset(&outBuffer, 0, 60);
  numbytes = recv(sockfd, buffer, MAXDATA-1, 0);
  if(numbytes == -1 ){
    fprintf(stderr,"recv, %d -- %s \n", errno, strerror(errno));
    exit(1);
  }
  totalBytes+=numbytes;
  memcpy(&outBuffer,&buffer,20);
  
  printf("client (%d/%d) : received \n",numbytes, totalBytes);
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
}




int main(int argc, char *argv[])
{

    struct addrinfo hints, *servinfo, *p;
    int rv;
    totalBytes=0;
    

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

    signal(SIGIO, io_handler);
    
    // second: set the process id or process group id that is to receive
    // notification of pending input to its own process id or process 
    // group id
    if (fcntl(sockfd,F_SETOWN, getpid()) < 0){
      perror("fcntl F_SETOWN");
      exit(1);
    }
    
    // third: allow receipt of asynchronous I/O signals
    if (fcntl(sockfd,F_SETFL,FASYNC) <0 ){
      perror("fcntl F_SETFL, FASYNC");
      exit(1);
    }


    /* Do nothing in the main... */
    for(;;){
      printf(" 'I know nothing /Manuel' \n");
      sleep(1);
    }
    

    
    close(sockfd);

    return 0;
}
