/*
** server.c -- a stream socket server demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT "4950"  // the port users will be connecting to
#define BACKLOG 2	 // how many pending connections queue will hold
#define SECRETSTRING "gimboid"

void sigchld_handler(int s)
{
	(void)s; // quiet unused variable warning

	// waitpid() might overwrite errno, so we save and restore it:
	int saved_errno = errno;

	while(waitpid(-1, NULL, WNOHANG) > 0);

	errno = saved_errno;
}


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
	int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	struct sigaction sa;
	int yes=1;
	char s[INET6_ADDRSTRLEN];
	int rv;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
				sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}

		break;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (p == NULL)  {
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}


	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}


	printf("server: waiting for connections BINJER...\n");
	char msg[1500];
	int MAXSZ=sizeof(msg)-1;
	
	int childCnt=0;
	int readSize;
	char command[10];
	char optionstring[128];
	int optionint1;
	int optionint2;
	
	while(1) {  // main accept() loop
	  sin_size = sizeof(their_addr);
	  new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
	  if (new_fd == -1) {
	    perror("accept");
	    continue;
	  }
	  /* ------------- */
	  inet_ntop(their_addr.ss_family,
		    get_in_addr((struct sockaddr *)&their_addr),
		    s, sizeof s);
	  printf("server: Connection %d from %s\n",childCnt, s);
	  
	  printf("server: Sending welcome \n");
	  struct sockaddr_in *local_sin=(struct sockaddr_in*)&their_addr;
	  if (send(new_fd, "Hello, world!", 13, 0) == -1){
	    perror("send");
	    close(new_fd);
	    continue; //leave loop execution, go back to the while, main accept() loop. 
	  }

	  while(1){
	    readSize=recv(new_fd,&msg,MAXSZ,0);
	    printf("Child[%d] (%s:%d): recv(%d) .\n", childCnt,s,ntohs(local_sin->sin_port),readSize);
	    if(readSize==0){
	      printf("Child [%d] died.\n",childCnt);
	      close(new_fd);
	      break;
	    }
	    msg[readSize]=0;

	    int rv=sscanf(msg,"%s ",command);
	    printf("rv=%d Decoded command as: %s \n",rv,command);
	    
	    if(strcmp(command,"close")==0){
		    // Client sent 'command', check that it provided the correct magic word.\n");
		    rv=sscanf(msg,"%s %s",command,optionstring);
		    printf("rv=%d Decoded command + option as: %s %s\n",rv,command,optionstring);
		    if(strcmp(optionstring,SECRETSTRING)==0) {
		      printf("Yes, master I'll close.\n");
		      sprintf(msg,"Bye bye client of mine.\0");
		      send(new_fd,&msg,strlen(msg),0);
		      shutdown(new_fd, SHUT_RDWR);
		      close(new_fd);
		      break;
		    }
		  }
		  
		  send(new_fd,&msg,readSize,0);	 
		  printf("Child[%d]: sent => %s\n",childCnt,msg);
		}//close interior while
		printf("Socket closed()\n");
		
	}

	return 0;
}
