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
  int i;
  char buf[4096];
  int len = 0;
  int fd = -1;
  uint8_t * mem = NULL;
  
  if(argc != 2) {
    fprintf(stderr, "%s takes exactly one string.\n",argv[0]);
    return 1;
  }

  /* Open memory. */
  fd = open("/dev/mem", O_RDWR | O_SYNC);

  /* Map the page that will store the string. */
  mem = (uint8_t *) mmap
    ( 0,
      1,
      PROT_WRITE, MAP_SHARED, 
      fd,
      STRING_STORAGE
    );

  /* Write the string to that page. */
  len = strlen(argv[1]);
  if(len >= 4096) { 
    len = 4095;
  }
  memcpy(buf,argv[1],len);
  buf[len] = 0;

  for(i = 0; i <=len ; i++) {
    mem[i] = buf[i];
  }
  
  /* Indicate that writing the string is done. */
  mem = (uint8_t *) mmap
    ( 0,
      1,
      PROT_READ | PROT_WRITE,
      MAP_SHARED,
      fd,
      STRING_READY
    );
  mem[0] = 1;

}
