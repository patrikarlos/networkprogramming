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
int rems;

void  ALARMhandler(int sig)
{
  signal(SIGALRM, SIG_IGN);          /* ignore this signal       */
  rems=alarm(2);
  printf("Hello Alarm, run = %d rems = %d \n", run, rems);

  signal(SIGALRM, ALARMhandler);     /* reinstall the handler    */
}

void  INThandler(int sig)
{
  signal(SIGINT, SIG_IGN);          /* ignore this signal       */
  printf("Hello Int \n");
  run++;
  signal(SIGINT, INThandler);     /* reinstall the handler    */
}

void  QUIThandler(int sig)
{
  signal(SIGQUIT, SIG_IGN);          /* ignore this signal       */
  printf("Hello Quit.\n");
  signal(SIGQUIT, QUIThandler);     /* reinstall the handler    */
}

void  STPhandler(int sig)
{
  signal(SIGTSTP, SIG_IGN);          /* ignore this signal       */
  printf("Hello STP, %d\n", sig);
  signal(SIGTSTP, STPhandler);     /* reinstall the handler    */
}

void  PIPEhandler(int sig)
{
  signal(SIGPIPE, SIG_IGN);          /* ignore this signal       */
  printf("Hello PIPE, %d\n", sig);
  signal(SIGPIPE, PIPEhandler);     /* reinstall the handler    */
}

void HANGUP(int sig){
  signal(SIGHUP, SIG_IGN);          /* ignore this signal       */
  printf("HELLO HANGUP!\n");
  signal(SIGHUP, HANGUP);
}

void KILLED(int sig){
  signal(SIGKILL, SIG_IGN);          /* ignore this signal       */
  printf("SIG KILL\n");
  signal(SIGKILL, KILLED);
}



int main(int argc, char *argv[]){
  /*
  if (argc != 3) {
    fprintf(stderr,"usage: %s <PORT> <FILENAME>\n",argv[0]);
    exit(1);
  }
  */
  
  run=0;

  sig_t q1, q2;
  /* Test various combinations to see what signals can be caught */
  signal(SIGINT, INThandler);
  signal(SIGALRM, ALARMhandler);
  signal(SIGQUIT, QUIThandler);
  signal(SIGTSTP, STPhandler);
  signal(SIGPIPE, PIPEhandler);
  q1=signal(SIGHUP, HANGUP);
  q2=signal(SIGKILL, KILLED);
  printf("q1=%d , q2=%d \n", q1, q2);
  
  
  rems=alarm(2);
  printf("We got %d rems.\n", rems);
  
  
  while(run<10){
    printf("Im in the loop, run=%d.\n",run);
    sleep(1);
  }

  printf("Leaving.\n");
  
}
