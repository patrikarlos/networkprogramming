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

	if (argc < 3) {
		fprintf(stderr,"usage: talker hostname message [message2] ...\n");
		exit(1);
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;// AF_INET , AF_INET6
	//hints.ai_family = AF_INET; // , AF_INET6
	hints.ai_socktype = SOCK_DGRAM;

	char myBuffer[1000];
	FILE *myFile=fopen("dumper","w+");
	
	printf("Getting info.\n");
	if ((rv = getaddrinfo(argv[1], SERVERPORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));

		//		cout << "Hej World" << endl;
		printf("Hej World.\n");
		printf("Hej World, %d asd %g.\n", (int)4, (double)4.54); //=> STDOUT| stdout
		fprintf(myFile,"Hej World, %d asd %g.\n", (int)4, (double)4.54); //=> <fd>
		fprintf(stderr,"ERROR: Hej World, %d asd %g.\n", (int)4, (double)4.54); //=> stderr
		fprintf(stdout,"OUT: Hej World, %d asd %g.\n", (int)4, (double)4.54); //=> stdout
		sprintf(&myBuffer, "Hej World, %d asd %g.\n", (int)4, (double)4.54); //=> myBuffer		
		return 1;
	}
	printf("Got info.\n");
	
	// loop through all the results and make a socket
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("talker: socket");
			continue;
		}

		break;
	}
	
	/* HERE */ 
	if (p == NULL) {
		fprintf(stderr, "talker: failed to create socket\n");
		return 2;
	}

	printf("sockfd == %d \n",sockfd );
	/* We have a socket! */
	
	char myAddress[30];
	char *myAdd=&myAddress;

	struct sockaddr_in local_sin;
	socklen_t local_sinlen = sizeof(local_sin);
	getsockname(sockfd,(struct sockaddr*)&local_sin, &local_sinlen);

	myAdd=inet_ntop(local_sin.sin_family,&local_sin.sin_addr,myAddress,sizeof(myAddress));
	printf("Sending from  %s:%d \n", myAdd, ntohs(local_sin.sin_port));


	for(int q=2;q<argc;q++){
	  if ((numbytes = sendto(sockfd, argv[q], strlen(argv[q]), 0,
				 p->ai_addr, p->ai_addrlen)) == -1) {
	    perror("talker: sendto");
	    exit(1);
	  }

	  getsockname(sockfd,(struct sockaddr*)&local_sin, &local_sinlen);
	  
	  myAdd=inet_ntop(local_sin.sin_family,&local_sin.sin_addr,myAddress,sizeof(myAddress));
	  printf("Sending from  %s:%d, sent %d bytes ('%s'). \n", myAdd, ntohs(local_sin.sin_port),numbytes,argv[q]);
	  
	}
	
	freeaddrinfo(servinfo);


	close(sockfd);

	return 0;
}
