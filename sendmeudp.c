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
	int numbytes,numread;
	char buffer[1450];
	
	char** tokens;
	FILE *fptr;
	
	if (argc != 3) {
		fprintf(stderr,"usage: %s <HOSTNAME:PORT> <FILENAME>\n",argv[0]);
		exit(1);
	}
	char delim[]=":";
	char *Desthost=strtok(argv[1],delim);
	char *Destport=strtok(NULL,delim);
	char *filename=argv[2];

	printf("Sending %s to Dest=%s Port=%s \n",filename,Desthost,Destport);

	memset(&hints, 0, sizeof hints);
	memset(&buffer,0, sizeof(buffer));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	
	
	
	if ((rv = getaddrinfo(Desthost, Destport, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}
	printf("servinfo..\n");
	// loop through all the results and make a socket
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("socket");
			continue;
		}

		break;
	}
	printf("servinfo prt2..\n");
	if (p == NULL) {
		fprintf(stderr, "%s: failed to create socket\n",argv[0]);
		return 2;
	}
	fptr = fopen(filename,"r");
	if (fptr == NULL) {
		fprintf(stderr, "%s: failed to open file.\n",argv[0]);
		close(sockfd);
		exit(0);
	}
	numread=fread(buffer,1,sizeof(buffer),fptr);
	int readTotByte=0;
	int sentTotByte=0;
	while( numread>0){
	printf("Read %d bytes :",numread);		
		if ((numbytes = sendto(sockfd, buffer, numread, 0,
				 p->ai_addr, p->ai_addrlen)) == -1) {
			perror("talker: sendto");
			exit(1);
		}
//		printf("%s",buffer);

		printf("'");
		for(int k=0;k<20;k++){
			printf("%c",buffer[k]);
		}
		printf("'");

		printf(" => Sent %d bytes \n",numbytes);
		readTotByte+=numread;
		sentTotByte+=numbytes;
		numread=fread(buffer,1,sizeof(buffer),fptr);

	}
	freeaddrinfo(servinfo);

	printf("talker: read %d and sent %d bytes\n", readTotByte,sentTotByte);
	close(sockfd);

	return 0;
}
