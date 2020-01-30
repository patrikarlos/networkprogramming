/*
** listener.c -- a datagram sockets "server" demo
** From http://beej.us/guide/bgnet/examples/listener.c
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
#include  <signal.h>

#define MYPORT "4950"	// the port users will be connecting to

#define MAXBUFLEN 1450

// Helper function you can use:

//Convert a struct sockaddr address to a string, IPv4 and IPv6:

char *get_ip_str(const struct sockaddr *sa, char *s, size_t maxlen)
{
    switch(sa->sa_family) {
        case AF_INET:
            inet_ntop(AF_INET, &(((struct sockaddr_in *)sa)->sin_addr),
                    s, maxlen);
            break;

        case AF_INET6:
            inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)sa)->sin6_addr),
                    s, maxlen);
            break;

        default:
            strncpy(s, "Unknown AF", maxlen);
            return NULL;
    }

    return s;
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

	FILE *fptr;
	int sockfd;
		
void  ALARMhandler(int sig)
{
  signal(SIGALRM, SIG_IGN);          /* ignore this signal       */
  printf("Hello\n");
  fflush(fptr);
  close(sockfd);
  sockfd=NULL;
  fclose(fptr);
  signal(SIGALRM, ALARMhandler);     /* reinstall the handler    */
}


int main(int argc, char *argv[])
{

	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes,numwritten;
	struct sockaddr_storage their_addr;
	struct sockaddr_in *the_addr;
	char buf[MAXBUFLEN];
	socklen_t addr_len;

	int readTotByte=0;
	int sentTotByte=0;
	
	if (argc != 3) {
		fprintf(stderr,"usage: %s <PORT> <FILENAME>\n",argv[0]);
		exit(1);
	}
	char s[INET6_ADDRSTRLEN];
	char *filename=argv[2];
	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET; // set to AF_INET to force IPv4
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, argv[1], &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("listener: socket");
			continue;
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("listener: bind");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "listener: failed to bind socket\n");
		return 2;
	}

	freeaddrinfo(servinfo);

	printf("listener: waiting to recvfrom...\n");
	printf("bob,\n");
	printf("family = %d \n",p->ai_family);
	printf("AF_INET = %d and AF_INET6 = %d and AF_UNSPEC  = %d  and AF_UNIX = %d \n", AF_INET, AF_INET6, AF_UNSPEC, AF_UNIX);
	
	signal(SIGALRM, ALARMhandler);
	
	fptr = fopen(filename,"w");
	if (fptr == NULL) {
		fprintf(stderr, "%s: failed to open file.\n",argv[0]);
		close(sockfd);
		exit(0);
	}
	
	while(1){
		addr_len = sizeof their_addr;
		if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,
			(struct sockaddr *)&their_addr, &addr_len)) == -1) {
			if(sockfd!=NULL){
				perror("recvfrom");
			}
			exit(1);
		}

		the_addr=(struct sockaddr_in*)&their_addr;
		printf("listener: from %s:%d ",
			inet_ntop(their_addr.ss_family,
				get_in_addr((struct sockaddr *)&their_addr),
				  s, sizeof s),ntohs(the_addr->sin_port));
		printf(" %d bytes : ", numbytes);

		numwritten=fwrite(buf,1,numbytes,fptr);

		printf("'");
		for(int k=0;k<20;k++){
			printf("%c",buf[k]);
		}
		printf("'");
		readTotByte+=numbytes;
		sentTotByte+=numwritten;
		fflush(fptr);
		printf("\nWritten %d bytes \n",sentTotByte);
		alarm(1);
	}
	close(sockfd);
		fclose(fptr);
	return 0;
}
