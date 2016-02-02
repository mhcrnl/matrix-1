#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *args[]) {
  struct winsize size;
  int len, i;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
  
  printf("lines: %d\n", size.ws_row);
  printf("cols: %d\n", size.ws_col);

  len = size.ws_row * size.ws_col;
  char* filler = malloc(len+1);

  for (i = 0; i < len; i++) {
    filler[i] = '#';
  }

  filler[len+1] = '\0';

  printf("%s", filler);

  return 0;
}
