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

// Helper function you can use:
int run;

void  ALARMhandler(int sig)
{
  signal(SIGALRM, SIG_IGN);          /* ignore this signal       */
  printf("Hello Alarm\n");
  signal(SIGALRM, ALARMhandler);     /* reinstall the handler    */
}

void  INThandler(int sig)
{
  signal(SIGINT, SIG_IGN);          /* ignore this signal       */
  printf("Hello Int\n");
  run++;
  signal(SIGINT, INThandler);     /* reinstall the handler    */
}

void  QUIThandler(int sig)
{
  signal(SIGQUIT, SIG_IGN);          /* ignore this signal       */
  printf("Hello Quit.\n");
  signal(SIGQUIT, INThandler);     /* reinstall the handler    */
}

void  STPhandler(int sig)
{
  signal(SIGTSTP, SIG_IGN);          /* ignore this signal       */
  printf("Hello STP\n");
  signal(SIGTSTP, INThandler);     /* reinstall the handler    */
}


int main(int argc, char *argv[]){
  /*
  if (argc != 3) {
    fprintf(stderr,"usage: %s <PORT> <FILENAME>\n",argv[0]);
    exit(1);
  }
  */
  
  run=0;
  
  signal(SIGINT, INThandler);
  //  signal(SIGALRM, ALARMhandler);
  //  signal(SIGQUIT, QUIThandler);
  //  signal(SIGTSTP, STPhandler);  
  
  
  while(run<10){
    printf("Im in the loop, run=%d.\n",run);
    sleep(1);
  }

  printf("Leaving.\n");
  
}
