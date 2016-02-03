#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define KRED "\033[0m"
#define MAXLEN 20

struct Trail {
  int x;
  int y;
  int len;
  char seq[MAXLEN];
};

void delay(int millis);
void getWindow(int *row, int *col);
void createTrail(struct Trail *new_trail, int x, int y, int len);

int main(int argc, char *args[]) {
  struct Trail trail1, trail2;

  createTrail(&trail1, 5, 10, 15);
  createTrail(&trail2, 1, 4, 9);

  printf("Trail 1:\nx: %d, y: %d, len: %d\n\n", trail1.x, trail1.y, trail1.len);
  printf("Trail 2:\nx: %d, y: %d, len: %d", trail2.x, trail2.y, trail2.len);

  return 0;
}

void delay(int millis) {
  /* Haven't dived into this function, pulled it off
   * the internet */
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

void createTrail(struct Trail *new_trail, int x, int len) {
  /* Create new trail */
  int i;

  new_trail->x = x;
  new_trail->y = 0; /* y always starts out at 0 */
  new_trail->len = len;

  for (i = 0; i < len; i++) {
    new_trail->seq[i] = 'a';
  }
}

