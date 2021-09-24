#include <stdio.h>

#define DEBUG_LEVEL 2

void main(void){
  printf("Hello World.\n");

  printf("Absolute Debugg Levels.\n");
#if DEBUG_LEVEL==2
  printf("DEBUG LEVEL = %d \n", DEBUG_LEVEL);
#elif DEBUG_LEVEL==1
  printf("DEBUG LEVEL = %d \n", DEBUG_LEVEL);
#else
  printf("No Debugging.\n");
#endif

  printf("Bye bye!.\n");

  printf("Scaling Debugg Levels.\n");
#if DEBUG_LEVEL>=2
  printf("S2DEBUG LEVEL = %d \n", DEBUG_LEVEL);
#endif
#if DEBUG_LEVEL>=1
  printf("S1DEBUG LEVEL = %d \n", DEBUG_LEVEL);
#endif

#if DEBUG_LEVEL==0
  printf("No Debugging.\n");
#endif

 #ifdef myDEFINE
  printf("My Define!!\n");
#else
  printf("not present.\n");
#endif
  
}
    
  
