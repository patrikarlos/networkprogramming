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
#include <sys/time.h> 


/* Subtract the ‘struct timeval’ values X and Y,
   storing the result in RESULT.
   Return 1 if the difference is negative, otherwise 0. */

int timeval_subtract (struct timeval *result, struct timeval *x, struct timeval *y){
  /* Perform the carry for the later subtraction by updating y. */
  if (x->tv_usec < y->tv_usec) {
    int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
    y->tv_usec -= 1000000 * nsec;
    y->tv_sec += nsec;
  }
  if (x->tv_usec - y->tv_usec > 1000000) {
    int nsec = (x->tv_usec - y->tv_usec) / 1000000;
    y->tv_usec += 1000000 * nsec;
    y->tv_sec -= nsec;
  }

  /* Compute the time remaining to wait.
     tv_usec is certainly positive. */
  result->tv_sec = x->tv_sec - y->tv_sec;
  result->tv_usec = x->tv_usec - y->tv_usec;

  /* Return 1 if result is negative. */
  return x->tv_sec < y->tv_sec;
}


int main(int argc, char *argv[]){
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes,numread;
	char buffer[1450];
	
	struct timeval GTOD_before, GTOD_after, difference; 
	
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

	printf(" %s sending to %s:%s \n",filename,Desthost,Destport);

//Do something
	
	gettimeofday(&GTOD_before,NULL);
// Do something

	gettimeofday(&GTOD_after,NULL); 
	
	timeval_subtract(&difference,&GTOD_after,&GTOD_before);

	if (difference.tv_sec > 0) {
		printf("%ld.%06ld [s]\n", difference.tv_sec, difference.tv_usec);
	} else {
		printf("%6ld [us]\n", difference.tv_usec);
	}

	return 0;
}
