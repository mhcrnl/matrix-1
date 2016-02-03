#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define ESC "\033"
#define KRED "\033[0m"
#define TRM_GREEN "\033[32m"
#define TRM_WHITE "\033[37m"
#define MAXLEN 20

struct Trail {
  int x;
  int y;
  int len;
  char seq[MAXLEN];
};

struct Window {
  int row;
  int col;
};

void delay(int millis);
void getWindow(struct Window *window);
void createTrail(struct Trail *new_trail, int x, int len);
void clearScreen(struct Window *window);
void printTrail(struct Trail *trail);

int main(int argc, char *args[]) {
  struct Window window;
  getWindow(&window);

  struct Trail trail1, trail2, trail3, trail4;

  createTrail(&trail1, 5, 15);
  createTrail(&trail2, 1, 9);
  createTrail(&trail3, 10, 7);
  createTrail(&trail4, 14, 13);

  trail1.y = 7;
  trail2.y = 10;
  trail3.y = 2;
  trail4.y = 20;

  clearScreen(&window);
  /* printTrails(&trail1, &trail2, &trail3, &trail4);*/
  printTrail(&trail1);
  printTrail(&trail2);
  printTrail(&trail3);
  printTrail(&trail4);

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

void getWindow(struct Window *window) {
  /* Initialize some variables */
  struct winsize size;

  /* Get window size */
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

  /* Stash them variables */
  window->row = size.ws_row;
  window->col = size.ws_col;
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

void printTrail(struct Trail *trail) {
  int i, print_y;
  for(i = 0; i < trail->len; i++) {
    print_y = trail->y + i;
    printf("%s", TRM_GREEN);
    if(i == trail->len-1) {
      printf("%s", TRM_WHITE);
    }
    printf("\033[%d;%dH%c", print_y, trail->x, trail->seq[i]);
  }
}

void clearScreen(struct Window *window) {
  int i;
  printf("\033[0;0H");
  for(i = 0; i < window->col * window->row; i++) {
    printf(" ");
  }
  fflush(stdout);
}

