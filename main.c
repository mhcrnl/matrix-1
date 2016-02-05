#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* Some ANSI values */
#define ESC "\033"
#define TRM_GREEN "\033[32m"
#define TRM_WHITE "\033[37m"

#define MINLEN 20
#define MAXLEN 35 /* The maximum length for the trails */


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

int debug = 0;

/* Function prototypes */
void delay(int millis);
void getWindow(struct Window *window);
void createTrail(trail_t *trails, int x, int len);
void clearScreen(struct Window *window);
void printTrail(trail_t *trail, struct Window *window);
void printAll(trail_t *trails, struct Window *window, int incr);
int getRand(int n, int m);
void tryNewTrail(trail_t *trails, struct Window *window);
void deleteTrail(trail_t **trails);

int main(int argc, char *args[]) {
  if(argc >= 2) {
    if(strcmp(args[1],"-d") == 0) {
      debug = 1;
    }
  }
  /* First get the window info and store it
   * in the struct */
  struct Window *window = malloc(sizeof(struct Window));
  getWindow(window);

  /* Set up randomness */
  srand(time(NULL));

  /* Set up the trails linked list */
  trail_t *trails = NULL;
  trails = malloc(sizeof(trail_t));
  trails->next = NULL;

  while(1) {
    /* if(getRand(1,3) == 1) {*/
      /* printf("yes");*/
      /* fflush(stdout);*/
      /* delay(500);*/
      tryNewTrail(trails, window);
    /* }*/

    printAll(trails, window, 1);
    delay(40);
  }

  /* Testing down here */
  /* for(int i = 0; i < window->col; i++) {*/
  /*   createTrail(trails, i, getRand(4,9));*/
  /* }*/
  /* while(1) {*/
  /*   printAll(trails, window, 1);*/
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
  /* Create new trail to store in the trails
   * linked list. */
  int i;
  trail_t *current = trails;
  while(current->next != NULL) {
    current = current->next;
  }

  current->next = malloc(sizeof(trail_t));

  current->next->x = x;
  current->next->y = -len; /* y always starts out at 0 */
  current->next->len = len;
  current->next->next = NULL;

  for (i = 0; i < len; i++) {
    current->next->seq[i] = getRand(33, 126);
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
    if(print_y < 0) {
      continue;
    }

    /* If the trail leaves the screen, stop printing */
    if(print_y > window->row) {
      break;
    }

     /* Trail is green, except for the head of the*/
     /* trail. That character is white */
    printf("%s", TRM_GREEN);
    if(i == trail->len-1) {
      printf("%s", TRM_WHITE);
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
  /* This function returns a random integer
   * between n and m */
  return n + rand() % (m-n);
}

void printAll(trail_t *trails, struct Window *window, int incr) {
  /* This function prints all the exisiting trails */
  int i;
  trail_t *current = trails->next;
  trail_t *previous = trails;

  /* First clear the whole screen */
  clearScreen(window);

  /* Go through the linked list, printing
   * each trail */
  while(current != NULL) {
    printTrail(current, window);
    current->y += incr; /* Increment y afterwards, so it moves down */
    if(current->y > window->row) {
      deleteTrail(&previous);
    }
    previous = current;
    current = current->next;
  }


  if(debug == 1) {
    printf("\033[%d;0H", window->row);
    current = trails->next;
    while(current != NULL) {
      printf("%d->", current->x);
      current = current->next;
    }
    printf("#");
  } else {
    printf("\033[%d;%dH", window->row, window->col);
  }

  fflush(stdout);

}

void tryNewTrail(trail_t *trails, struct Window *window) {
  int *free = malloc(sizeof(int) * window->col);
  int n = 0;
  trail_t *current = trails->next;

  while(current != NULL) {
    free[current->x] = 1;
    n++;
    printf("#");
    current = current->next;
  }

  for(int i = 0; i < window->col; i++) {
    if(free[i] == 0 && getRand(0,(window->col-n)) == 1) {
    /* if(free[i] == 0 ) {*/
      createTrail(trails, i, getRand(MINLEN, MAXLEN));
      /* break;*/
    }
  }
}

void deleteTrail(trail_t **node) {
  trail_t *temp = NULL;
  temp = (*node)->next; /* the node to remove */
  if(temp->next != NULL) {
    (*node)->next = temp->next;
    /* free(temp);*/
  } else {
    /* free(temp);*/
    (*node)->next = NULL;
  }
  if(temp == NULL) {
    printf("ERRORRR!");
    exit(1);
  }
  /* free(temp);*/
}
