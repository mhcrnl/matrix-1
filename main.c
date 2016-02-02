#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

int main(int argc, char *args[]) {
  struct winsize size;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
  
  printf("lines: %d\n", size.ws_row);
  printf("cols: %d\n", size.ws_col);

  return 0;
}
