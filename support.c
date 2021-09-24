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


int main(int argc, char *argv[]){


  printf("sizeof(sockaddr_in) = %ld \n", sizeof(struct sockaddr_in));
  printf("struct sockaddr_in { \n");
  int sum=sizeof(uint8_t);
  printf("\tsizeof(sin_len) = %ld bytes\t%d\n", sizeof(uint8_t),sum);
  sum+=sizeof(sa_family_t);
  printf("\tsizeof(sin_family) = %ld bytes\t%d\n", sizeof(sa_family_t),sum);
  sum+=sizeof(in_port_t);
  printf("\tsizeof(sin_port) = %ld bytes\t%d\n", sizeof(in_port_t),sum);
  sum+=sizeof(struct in_addr);
  printf("\tsizeof(struct in_addr) = %ld bytes\t%d\n", sizeof(struct in_addr),sum);
  sum+=8;
  printf("\tsizeof(sin_zero[8]) = %d bytes\t%d\n", 8,sum);
  printf("}");
  printf("Sum = %ld \n", sizeof(uint8_t) + sizeof(sa_family_t) + sizeof(in_port_t) + sizeof(struct in_addr) + 8 );

  struct sockaddr_in bob;
  struct sockaddr_in *ptr;
  
  printf("bob %ld \n ", sizeof(bob));
  printf("bob %p \n",&bob);
  //  printf("bob.sin_len = %p  ==> %ld \n", &bob.sin_len,(unsigned char*)ptr->sin_len-(unsigned char*)ptr );
  printf("bob.sin_family = %p  ==> %d \n", &bob.sin_family,(unsigned char*)ptr->sin_family-(unsigned char*)ptr );
  printf("bob.sin_port   = %p  ==> %d \n", &bob.sin_port,(unsigned char*)ptr->sin_port-(unsigned char*)ptr );
  printf("bob.sin_addr   = %p  ==> %ld \n",&bob.sin_addr,(unsigned char*)ptr->sin_addr-(unsigned char*)ptr );
  printf("bob.sin_addr   = %p  ==>  \n", &bob.sin_addr);
  printf("bob.sin_zero   = %p  ==> %ld \n", &bob.sin_zero,(unsigned char*)ptr->sin_zero-(unsigned char*)ptr );
  
  return(0);
}
