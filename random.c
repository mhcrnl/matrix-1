/* Return integer in range [0,n) */
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <openssl/rand.h>

/* Random integer in [0, limit) */
unsigned int random_uint(unsigned int limit) {
  union {
    unsigned int i;
    unsigned char c[sizeof(unsigned int)];
  } u;

  do {
    if (!RAND_bytes(u.c, sizeof(u.c))) {
      fprintf(stderr, "Can't get random bytes!\n");
      exit(1);
    }
  } while (u.i < (-limit % limit)); /* u.i < (2**size % limit) */
  return u.i % limit;
}

/* int randint(int n) {*/
/*   if ((n-1) == RAND_MAX) {*/
/*     return rand();*/
/*   } else {*/
/*     long end = RAND_MAX / n;*/
/*     assert(end > 0L);*/
/*     end *= n;*/

/*     int r;*/
/*     while((r = rand()) >= end);*/

/*     return r % n;*/
/*   }*/
/* }*/

/* int firstTime = 1;*/

/* int randint(int n) {*/
/*   if(firstTime == 1) {*/
/*     srand(time(NULL));*/
/*     firstTime = 0;*/
/*   }*/
/*   return (rand());*/
/* }*/
