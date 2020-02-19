#include<stdio.h>
#include<sys/types.h>//socket
#include<sys/socket.h>//socket
#include<string.h>//memset
#include<stdlib.h>//sizeof
#include<netinet/in.h>//INADDR_ANY

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
	
 //create socket
 listenfd=socket(AF_INET,SOCK_STREAM,0);
 //initialize the socket addresses
 memset(&serverAddress,0,sizeof(serverAddress));
 serverAddress.sin_family=AF_INET;
 serverAddress.sin_addr.s_addr=htonl(INADDR_ANY);
 serverAddress.sin_port=htons(PORT);

 //bind the socket with the server address and port
 bind(listenfd,(struct sockaddr *)&serverAddress, sizeof(serverAddress));

 //listen for connection from client
 listen(listenfd,5);
 printf("Listening on port %d \n",PORT);
 while(1) {
   //parent process waiting to accept a new connection
   printf("\n*****server waiting for new client connection:*****\n");
   clientAddressLength=sizeof(clientAddress);
   connfd=accept(listenfd,(struct sockaddr*)&clientAddress,&clientAddressLength);
   printf("accept = %d \n", connfd );
   childCnt++;
   
   printf("listener: got packet from %s:%d\n",
	  inet_ntop(clientAddress.sin_family,
		    get_in_addr((struct sockaddr *)&clientAddress),
		    s, sizeof s),ntohs(clientAddress.sin_port));
   
   //child process is created for serving each new clients
   pid=fork();
   if(pid==0)//child process rec and send
     {
       printf("Im child %d.\n", childCnt);
       //rceive from client
       get_ip_str((struct sockaddr*)&clientAddress,&cli,&clientAddressLength);
       
       while(1){
	 n=recv(connfd,msg,MAXSZ,0);
	 printf("Child[%d] (%s:%d): recv(%d) .\n", childCnt,cli,ntohs(clientAddress.sin_port),n);
	 if(n==0){
	   printf("Child [%d]: Im dying...",childCnt);
	   close(connfd);
	   break;
	 }
	 msg[n]=0;
	 send(connfd,msg,n,0);	 
	 printf("Child[%d]: (x:y) Receive and set:%s\n",childCnt,msg);
       }//close interior while
       exit(0);
     }
   else {
     printf("Parent, close connfd().\n");
	 sprintf(msg,"Hello World\n");
	 send(connfd,msg,strlen(msg),0);
     close(connfd);//sock is closed BY PARENT
	 if(childCnt>5){
		 printf("Being a bad parent.\n");
		 sleep(10);
	 }
   }
 }//close exterior while
 
 return 0;
}
