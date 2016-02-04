#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

/* Some ANSI values */
#define ESC "\033"
#define TRM_GREEN "\033[32m"
#define TRM_WHITE "\033[37m"

#define MAXLEN 20 /* The maximum length for the trails */


/* Defintion of the Trail struct */
struct Trail {
  int x;
  int y;
  int len;
  char seq[MAXLEN];
};

/* Definition of the window struct, used to
 * hold the row and col info */
struct Window {
  int row;
  int col;
};

/* Function prototypes */
void delay(int millis);
void getWindow(struct Window *window);
void createTrail(struct Trail *new_trail, int x, int len);
void clearScreen(struct Window *window);
void printTrail(struct Trail *trail, struct Window *window);
void printAll(struct Trail trailArray[], struct Window *window);
int getRand(int n, int m);

int main(int argc, char *args[]) {
  /* First get the window info and store it
   * in the struct */
  struct Window window;
  getWindow(&window);

  /* Set up randomness */
  srand(time(NULL));

  struct Trail trailArray[window.col*2];
  /* printf("struct: %lu\n", sizeof(struct Trail));*/
  /* printf("trailarray: %lu\n", sizeof(trailArray));*/
  /* printf("dus: %d, ofwel: %d\n", (int)( sizeof(trailArray)/sizeof(struct Trail)), window.col * 2);*/

  int x=0;
  while(1) {
    if(x < window.col) {
      createTrail(&trailArray[x], x, getRand(4, 15));
    }

    printAll(trailArray, &window);

    x++;
    if(x == window.row) {
      x = 0;
    }
    delay(40);
  }

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
  /* This function gets the window size and
   * stores it in the *window struct */

  /* Initialize some variables */
  struct winsize size;

  /* Get window size */
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

  /* Stash them variables */
  window->row = size.ws_row;
  window->col = size.ws_col;
}

void createTrail(struct Trail *new_trail, int x, int len) {
  /* Create new trail to store in the *new_trail
   * struct. */
  int i;

  new_trail->x = x;
  new_trail->y = 0; /* y always starts out at 0 */
  new_trail->len = len;

  for (i = 0; i < len; i++) {
    new_trail->seq[i] = 'a';
  }
}

void printTrail(struct Trail *trail, struct Window *window) {
  /* This function prints the trail at the
   * right position. */

  /* Initalize some variables */
  int i, print_y;

  /* Loop through the trail */
  for(i = 0; i < trail->len; i++) {
    /* y=0 means top of screen
     * Obviously we need to add i, because
     * we're moving down through the trail */
    print_y = trail->y + i;

    if(print_y > window->row) {
      break;
    }

    /* Trail is green, except for the head of the
     * trail. That character is white */
    printf("%s", TRM_GREEN);
    if(i == trail->len-1) {
      printf("%s", TRM_WHITE);
    }

    /* Print the character, using ANSI to
     * position the cursor */
    printf("\033[%d;%dH%c", print_y, trail->x, trail->seq[i]);
  }
}

void clearScreen(struct Window *window) {
  /* This function clears the whole screen */

  int i;
  printf("\033[0;0H");

  /* For the entire surface, cols*rows,
   * print spaces */
  for(i = 0; i < window->col * window->row; i++) {
    printf(" ");
  }

  /* Just to be sure, flush the stdout */
  fflush(stdout);
}

int getRand(int n, int m) {
  return n + rand() % (m-n);
}

void printAll(struct Trail trailArray[], struct Window *window) {
  /* printf("%d, %d\n", window->col, window->row);*/
  /* printf("Bijv: %d\n", trailArray[0].len);*/
  int i;

  printf("test");
  printf("struct: %lu", sizeof(struct Trail));

  /* while(trailArray[i] != NULL) {*/
  /*   printf("%d\n", trailArray[i].len);*/
  /*   i++;*/
  /* }*/

  /* clearScreen(window);*/
  for(i = 0; i < window->col * 2; i++) {
    printf("%d\n", trailArray[i].len);
    /* printTrail(&trailArray[i], &window);*/
  }
  fflush(stdout);

}

