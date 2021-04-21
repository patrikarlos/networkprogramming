#include<stdio.h>
#include<sys/types.h>//socket
#include<sys/socket.h>//socket
#include<string.h>//memset
#include<stdlib.h>//sizeof
#include<netinet/in.h>//INADDR_ANY
#include<errno.h>
#define PORT 4950
#define MAXSZ 1400
int childCnt;

void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
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
 struct sockaddr_storage their_addr;
 
 int n;
 char msg[MAXSZ];
 int clientAddressLength;
 int pid;

 char cli[INET6_ADDRSTRLEN];
 char s[INET6_ADDRSTRLEN];

 int rv;
 memset(msg,'A',MAXSZ);
 msg[MAXSZ-1]='\n';
 
 printf("My port = %d \n", PORT);
 //create socket
 listenfd=socket(AF_INET,SOCK_STREAM,0);
 //initialize the socket addresses
 memset(&serverAddress,0,sizeof(serverAddress));
 serverAddress.sin_family=AF_INET;
 serverAddress.sin_addr.s_addr=htonl(INADDR_ANY);
 serverAddress.sin_port=htons(PORT);

 //bind the socket with the server address and port
 rv = bind(listenfd,(struct sockaddr *)&serverAddress, sizeof(serverAddress));

 if (rv == -1 ){
   fprintf(stderr, "bind: %s \n",strerror(errno));
   exit(1);
 }


 printf("Bob\n");
 char myAddress[INET6_ADDRSTRLEN];
 char *myAdd=&myAddress;

 struct sockaddr_in local_sin;
 socklen_t local_sinlen = sizeof(local_sin);
 int pb=getsockname(listenfd,(struct sockaddr*)&local_sin, &local_sinlen);

 printf("pb = %d \n",pb);
 
 myAdd=inet_ntop(local_sin.sin_family,&local_sin.sin_addr,myAddress,sizeof(myAddress));

 printf("pb = %d myAdd = %p\n",pb,myAddress);
 printf("Listening on %s:%d \n", myAddress, ntohs(local_sin.sin_port)); 

	

  
 //listen for connection from client
 rv=listen(listenfd,1);
 if(rv == -1 )
   {
     fprintf(stderr, "Issues %s.\n", strerror(errno));
   }
 while(1) {
   //parent process waiting to accept a new connection
   printf("\n*****server waiting for new client connection:*****\n");
   clientAddressLength=sizeof(clientAddress);
   connfd=accept(listenfd,(struct sockaddr*)&clientAddress,&clientAddressLength);
   printf("accept = %d \n", connfd );

   childCnt++;
   int q=inet_ntop(clientAddress.sin_family, get_in_addr((struct sockaddr *)&clientAddress), s, sizeof(s));
   printf("q = %d \n",q); 
   
   printf("listener: got packet from %s:%d\n", s,ntohs(clientAddress.sin_port));
   

   //rceive from client
   get_ip_str((struct sockaddr*)&clientAddress,&cli,&clientAddressLength);
   
   printf("Child[%d] (%s:%d): recv(%d) .\n", childCnt,cli,ntohs(clientAddress.sin_port),n);
   int dropcnt=5000;
   char msgChar='A';
   while(dropcnt>0){
     memset(msg,msgChar,MAXSZ);      
     n=send(connfd,msg,MAXSZ,0);	 
     printf("Child[%d](%d) (%s:%d): Sent (%d) .\n", childCnt,dropcnt,cli,ntohs(clientAddress.sin_port),n);
     sleep(1);
     msgChar++;
     if (msgChar > 'Z') {
       msgChar='A';
     }
     dropcnt--;     
   }
   shutdown(connfd, SHUT_RDWR);
   close(connfd);
 }
 
 return 0;
}
