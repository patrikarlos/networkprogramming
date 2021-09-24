#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#define TESTS 10
#define MAXDATASIZE 100
int doit;

char *randstring(size_t length) {

    static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";        
    char *randomString = NULL;

    if (length) {
        randomString = malloc(sizeof(char) * (length +1));

        if (randomString) {            
            for (int n = 0;n < length;n++) {            
                int key = rand() % (int)(sizeof(charset) -1);
                randomString[n] = charset[key];
            }

            randomString[length] = '\0';
        }
    }

    return randomString;
}

void  INThandler(int sig)
{
  printf("\nCTRL-C received \n");
}

void ALARMhandler(int sig)
{
  printf("\nALARM raised.\n");
  raise(SIGINT);
  printf("\nLeaving ALARM.\n");
  return;
}

int main(void)
{

  signal(SIGINT, INThandler);
  signal(SIGALRM, ALARMhandler);


  struct timeval then, now,diff;
  


  /* 
     This part was intended to be an example of how a write call could be interrupted. 
     The probability that that happens on a multi-core system isnt zero, but pretty damn 
     close to it. Hence, its not a good practical example. 
  */
  /* 
  FILE *filedes;  
  filedes = fopen("remove_me_im_junk", "w+");

  int size=10000000;
  char *buf=randstring(size);
  
  int written = 0;
  doit=3;
  filedes = fopen("remove_me_im_junk", "w+");
  long int avgDuration=0;
  int avgCount=0;

  
  for(int i=0; i<TESTS; i++){
    gettimeofday(&then, NULL);
    written = write(fileno(filedes), buf, size);
    gettimeofday(&now, NULL);
    if (written == -1) {
      printf("Written  = %d, errno= %d \n", written, errno);
      if (errno == EINTR){
	printf("EINTR\n");
	doit=0;
      }
    }
    fseek(filedes,0, SEEK_SET);
    timersub(&now,&then,&diff);
    avgDuration+=diff.tv_usec;
    avgCount++;       
  }

  printf("Executed %d tests, and found an average of 0.%06ld [s] (broken if something >1s). \n", TESTS, (avgDuration/avgCount) );
  printf("Setting ualarm to %06ld with interval of %06ld us.\n", (avgDuration/avgCount)/10, (useconds_t)(avgDuration/avgCount)/20 );

  sleep(5);
  ualarm( (useconds_t)(avgDuration/avgCount)/10, (useconds_t)(avgDuration/avgCount)/20 );
  
  while(doit>0){
    //    *buf=randstring(size);
    gettimeofday(&then, NULL);
    written = write(fileno(filedes), buf, size);
    gettimeofday(&now, NULL);
    if (written == -1) {
      printf("Written  = %d, errno= %d \n", written, errno);
      if (errno == EINTR){
	printf("EINTR\n");
	doit=0;
      }
    }
    fseek(filedes,0, SEEK_SET);
    timersub(&now,&then,&diff);
    printf("Written %d in %ld.%06ld, and moved back to the start, doit = %d.\r", written, diff.tv_sec,diff.tv_usec, doit);
    doit--;

  }
  fclose(filedes);

  */

  /* End of write example */

  /* 
     This part will be using a tcp socket to connect, but it will hopefully be 
     aborted before it can complete. 
  */
  
  int sockfd; //, numbytes;  
  //char buffer[MAXDATASIZE];
  struct addrinfo hints, *servinfo, *p;
  int rv;
  //char s[INET6_ADDRSTRLEN];
  //  char trunk[]="[Trunkated]\0"; 
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  /* May require adaptation, to pick a port that isnt used. AFAIK helicon does not use port 65000. */
  if ((rv = getaddrinfo("helicon.nplab.bth.se", "222", &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  p=servinfo; 
  if ((sockfd = socket(p->ai_family, p->ai_socktype,
		       p->ai_protocol)) == -1) {
    perror("client: socket");
    return 0;
  }

  printf("Socket created, setting alarm on 3 ms.\n");
  
  ualarm(3000,0); // 3 ms
  gettimeofday(&then, NULL);
  rv=connect(sockfd, p->ai_addr, p->ai_addrlen);
  gettimeofday(&now, NULL);
  timersub(&now,&then,&diff);
  if (rv == -1 ){
    printf("Connect: %d , errno = %d, duration =%ld.%06ld  \n",rv, errno, diff.tv_sec,diff.tv_usec );
    printf("Error: %s \n", strerror(errno));
      
  } else {
    printf("Shit that was fast, change destination.\n");
  }
  close(sockfd);
  
    
  


  return 0;







}
