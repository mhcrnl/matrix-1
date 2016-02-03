#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define KRED "\x1B[0m"

void delay(int millis);
void getWindow(int *row, int *col);

int main(int argc, char *args[]) {


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

void getWindow(int *row, int *col) {
  /* Initialize some variables */
  struct winsize size;

  /* Get window size */
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

  /* Stash them variables */
  *row = size.ws_row;
  *col = size.ws_col;

}
