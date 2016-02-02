#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define KRED "\x1B[0m"

void delay(int millis);

int main(int argc, char *args[]) {
  /* Initialize some variables */
  struct winsize size;
  int len, i;

  /* Get window size */
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

  /* Calculate total length of string */
  len = size.ws_row * size.ws_col;
  char* filler = malloc(len+1);
  char* bla = malloc(len+1);

  /* Create string of proper length */
  for (i = 0; i < len; i++) {
    filler[i] = '#';
    bla[i] = '.';
  }

  /* Add terminating null character */
  filler[len+1] = '\0';
  bla[len+1] = '\0';

  printf("%sreddddddd\n\n\n", KRED);

  /* Print the string */
  while(1) {
    printf("\033[0;0H");
    printf("\033[31m");
    printf("%s", filler);
    fflush(stdout);
    delay(500);
    printf("\033[0;0H");
    printf("\033[34m");
    printf("%s", bla);
    fflush(stdout);
    delay(500);
  }

  return 0;
}

void delay(int millis) {
  long pause;
  clock_t now, then;

  pause = millis*(CLOCKS_PER_SEC/1000);
  now = then = clock();
  while( (now-then) < pause ) {
    now = clock();
  }
}
