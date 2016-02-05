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
typedef struct Trail {
  int x;
  int y;
  int len;
  char seq[MAXLEN];
  struct Trail *next;
} trail_t;

/* Definition of the window struct, used to
 * hold the row and col info */
struct Window {
  int row;
  int col;
};

/* Function prototypes */
void delay(int millis);
void getWindow(struct Window *window);
void createTrail(trail_t *trails, int x, int len);
void clearScreen(struct Window *window);
void printTrail(trail_t *trail, struct Window *window);
void printAll(trail_t *trails, struct Window *window, int incr);
int getRand(int n, int m);

int main(int argc, char *args[]) {
  /* First get the window info and store it
   * in the struct */
  struct Window *window = malloc(sizeof(struct Window));
  getWindow(window);

  /* Set up randomness */
  srand(time(NULL));

  trail_t *trails = NULL;
  trails = malloc(sizeof(trail_t));
  trails->next = NULL;

  /* printf("struct: %lu\n", sizeof(struct Trail));*/
  /* printf("trailarray: %lu\n", sizeof(trailArray));*/
  /* printf("dus: %d, ofwel: %d\n", (int)( sizeof(trailArray)/sizeof(struct Trail)), window.col * 2);*/

  for(int i = 0; i < window->col; i++) {
    createTrail(trails, i, getRand(4,9));
  }
  while(1) {
    printAll(trails, window, 1);
    delay(40);
  }

  /* int x=0;*/
  /* while(1) {*/
  /*   if(x < window.col) {*/
  /*     [> createTrail(&trailArray[x], x, getRand(4, 15));<]*/
  /*   }*/

  /*   printAll(trailArray, &window);*/

  /*   x++;*/
  /*   if(x == window.row) {*/
  /*     x = 0;*/
  /*   }*/
  /*   delay(40);*/
  /* }*/

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

void createTrail(trail_t *trails, int x, int len) {
  /* Create new trail to store in the *new_trail
   * struct. */
  int i;
  trail_t *current = trails;
  while(current->next != NULL) {
    current = current->next;
  }

  current->next = malloc(sizeof(trail_t));

  current->next->x = x;
  current->next->y = 0; /* y always starts out at 0 */
  current->next->len = len;
  current->next->next = NULL;

  for (i = 0; i < len; i++) {
    current->next->seq[i] = 'a';
  }
}

void printTrail(trail_t *trail, struct Window *window) {
  /* This function prints the trail at the
   * right position. */

  /* Initalize some variables */
  int i, print_y;

  /* Loop through the trail */
  for(i = 0; i < trail->len; i++) {
     /* y=0 means top of screen*/
     /* Obviously we need to add i, because*/
     /* we're moving down through the trail */
    print_y = trail->y + i;

    if(print_y > window->row) {
      break;
    }

     /* Trail is green, except for the head of the*/
     /* trail. That character is white */
    printf("%s", TRM_GREEN);
      /* printf("Now at %d/%d\n", i, trail->len);*/
    if(i == trail->len-1) {
      printf("%s", TRM_WHITE);
      /* printf("END!");*/
    }

     /* Print the character, using ANSI to*/
     /* position the cursor */
    printf("\033[%d;%dH%c", print_y+1, trail->x+1, trail->seq[i]);
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

void printAll(trail_t *trails, struct Window *window, int incr) {
  /* printf("%d, %d\n", window->col, window->row);*/
  /* printf("Bijv: %d\n", trailArray[0].len);*/
  int i;
  trail_t *current = trails->next;

  clearScreen(window);

  while(current != NULL) {
    printTrail(current, window);
    current->y += incr;
    current = current->next;
  }
  /* printf("\033[%d;0H", window->row);*/
  fflush(stdout);

}

