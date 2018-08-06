#include <camkes.h>
#include <stdio.h>

int run(void) {

  char rev[4096];
  
  while (1) {
    give_string_wait();

    ((char*)str)[4095]=0;

    strncpy(rev,str,4096);

    got_string_emit();

    printf("Rev: Received string %s\n", (char*)rev);

    strrev(rev);

    strncpy(rts,rev,4096);

    printf("Rev: Prepared reversed string %s\n", (char*)rts);

    give_gnirts_emit();

    got_gnirts_wait();
    
  }

  return 0;
}



void strrev(char *str)
{
    int i = strlen(str) - 1, j = 0;

    char ch;
    while (i > j)
    {
        ch = str[i];
        str[i] = str[j];
        str[j] = ch;
        i--;
        j++;
    }
    return;
}
