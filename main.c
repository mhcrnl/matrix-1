#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *args[]) {
  /* Initialize some variables */
  struct winsize size;
  int len, i;

  /* Get window size */
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

  /* Calculate total length of string */
  len = size.ws_row * size.ws_col;
  char* filler = malloc(len+1);

  /* Create string of proper length */
  for (i = 0; i < len; i++) {
    filler[i] = '#';
  }

  /* Add terminating null character */
  filler[len+1] = '\0';

  /* Print the string */
  printf("%s", filler);

  return 0;
}

