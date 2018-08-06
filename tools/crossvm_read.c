#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include "crossvm.h"

int main(int argc,char ** argv) {
  int i = 0;
  char buf[4096];
  int len = 0;
  int fd = -1;
  uint8_t * mem = NULL;
  char tmp;
  
  if(argc != 1) {
    fprintf(stderr, "%s takes exactly one string.\n",argv[0]);
    return 1;
  }
  
  /* Open memory. */
  fd = open("/dev/mem", O_RDWR | O_SYNC);

  /* Map the page that will indicate when a reversed string is ready. */
  mem = (uint8_t *) mmap
    ( 0,
      1,
      PROT_WRITE, MAP_SHARED, 
      fd,
      GNIRTS_READY
      );
  
  /* Wait for data to be ready. */
  while ( 1 ) {
    uint8_t tmp = mem[0];
    if(tmp == 1) { break; }
    usleep(100000);
  }

  /* Map reversed string storage and read its contents. */  
  mem = (uint8_t *) mmap
    ( 0,
      1,
      PROT_WRITE, MAP_SHARED, 
      fd,
      GNIRTS_STORAGE
      );

  printf("Received string \"");
  i = 0;
  while(1) {
    tmp = ((char*)mem)[i];
    if(tmp == '\0') { break; }
    printf("%c",tmp);
    i++;
  }
  printf("\".\n");

  /* Done reading. */  
  mem = (uint8_t *) mmap
    ( 0,
      1,
      PROT_WRITE, MAP_SHARED, 
      fd,
      GNIRTS_DONE
      );

  mem[0] = 1;  

}
