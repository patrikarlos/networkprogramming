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

int
main(int argc, char **argv)
{
  char			*ptr, **pptr;
  char			str[INET_ADDRSTRLEN];
  struct hostent	*hptr;
  
  while (--argc > 0) {
    ptr = *++argv;
    if ( (hptr = gethostbyname(ptr)) == NULL) {
      fprintf(stderr,"gethostbyname error for host: %s: %d",ptr, h_errno);
      continue;
    }
    printf("official hostname: %s\n", hptr->h_name);
    
    for (pptr = hptr->h_aliases; *pptr != NULL; pptr++)
      printf("\talias: %s\n", *pptr);
    
    switch (hptr->h_addrtype) {
    case AF_INET:
      pptr = hptr->h_addr_list;
      for ( ; *pptr != NULL; pptr++)
	printf("\taddress: %s\n",
	       inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
      break;
      
    default:
      perror("unknown address type");
      break;
    }
  }
  exit(0);
}
