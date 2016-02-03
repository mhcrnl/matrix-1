#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

int main(int argc, char* args[]) {

  int a = random_uint(10);
  for(int i = 0; i < 10; i++) {
    printf("Int: %d\n", a);
  }

  return 0;
}

