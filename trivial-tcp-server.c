#include<stdio.h>
#include<sys/types.h>//socket
#include<sys/socket.h>//socket
#include<string.h>//memset
#include<stdlib.h>//sizeof
#include<netinet/in.h>//INADDR_ANY
#include <errno.h> // errno
#include <sys/time.h>
#include <unistd.h> //MAc close
#include <arpa/inet.h> // MAC inet_ntop

#define PORT 4950
#define MAXSZ 1400
int childCnt;

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

int main()
{
 int listenfd;//to create socket
 int connfd;//to accept connection
 childCnt=0;
 
 struct sockaddr_in serverAddress;//server receive on this address
 struct sockaddr_in clientAddress;//server sends to client on this address
 // struct sockaddr_storage their_addr;
 
 // int n;
 // char msg[MAXSZ];
 socklen_t *clientAddressLength;
 int pid;

 char cli[INET6_ADDRSTRLEN]; 
 // char s[INET6_ADDRSTRLEN];

 struct timeval then, now,diff;
 
 //create socket
 listenfd=socket(AF_INET,SOCK_STREAM,0);  // IPv4, SOCK-steam + TCP. 
 /* what if socket failed?? */
 
 //initialize the socket addresses
 memset(&serverAddress,0,sizeof(serverAddress));
 serverAddress.sin_family=AF_INET;
 serverAddress.sin_addr.s_addr=htonl(INADDR_ANY); // =INADDR_ANY
 serverAddress.sin_port=htons(PORT);

 //bind the socket with the server address and port
 int rc=bind(listenfd,(struct sockaddr *)&serverAddress, sizeof(serverAddress));
 if (rc!=0){
   printf("Cannot bind, Got problems, %d. \n", errno);
   printf("Issue being: %s \n", strerror(errno) );
   return -1;
 } 

 //listen for connection from client
 rc=listen(listenfd,1);
 if(rc!=0){
     printf("Cannot listen, Got problems, %d. \n", errno);
     return -1;
 }
 
 printf("Listening on port %d \n",PORT);

 /*----------------------------- */ 
 
 while(1) {
   //parent process waiting to accept a new connection
   printf("\n*****server waiting for new client connection:*****\n");
   clientAddressLength=(socklen_t*)sizeof(clientAddress);
   connfd=accept(listenfd,(struct sockaddr*)&clientAddress,clientAddressLength);
   /* Check rv of accept */ 
   printf("accept = %d listenfd = %d \n", connfd, listenfd );
   childCnt++;
   
   
   const char *q=inet_ntop(clientAddress.sin_family, get_in_addr((struct sockaddr *)&clientAddress), cli, sizeof(cli));
   if (q == NULL ){
     printf("issues with inet_ntop.\n");
     return -1;
   }

   printf("listener: got packet from %s:%d\n", cli,ntohs(clientAddress.sin_port));

   char *rndString;

   int rv;
   //child process is created for serving each new clients
   pid=fork();
   if(pid==0)//child process rec and send
     {
       close(listenfd);//sock is closed BY child
       printf("Im childm (PID=%d)  %d.\n", pid, childCnt);
       //rceive from client
       get_ip_str((struct sockaddr*)&clientAddress,cli,(size_t)clientAddressLength);
       
       while(1){
	rndString=randstring(10);
	gettimeofday(&then, NULL);
        rv=send(connfd,rndString,10,0);
	gettimeofday(&now, NULL);
	timersub(&now,&then,&diff);
	printf("Sent=%d in %ld.%06ld \n", rv, diff.tv_sec,(long int)diff.tv_usec);
	if(rv==-1){
	  fprintf(stderr, "Child[%d](%s:%d): Send issues, %s .\n", childCnt,cli,ntohs(clientAddress.sin_port),strerror(errno));
	  free(rndString);
	  exit(0);
	}
	
        printf("Child[%d](%s:%d) sent:%s\n",childCnt,cli, ntohs(clientAddress.sin_port),rndString);
	free(rndString);
	sleep(1);
       }//close interior while
       exit(0);
     }
   else {
     printf("Parent, close connfd().\n");
     close(connfd);//sock is closed BY PARENT
     /*
     if(childCnt>0){
       printf("Being a bad parent.\n");
       sleep(100);
       printf("done sleeping.\n");
     }
     */
   }
 }//close exterior while
 
 return 0;
}
