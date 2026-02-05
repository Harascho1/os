#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main() {
  int pd1[2];
  pipe(pd1);
  if (fork() == 0) {
    close(pd1[1]);
    while (true) {
      int tmp;
      int read_bytes = read(pd1[0], &tmp, sizeof(int));
      printf("bytes = %d\n", read_bytes);
      if (read_bytes == 0) {
        return 0;
      }
      printf("broj %d\n", tmp);
      if (tmp % 2 == 0) {
        printf("broj je paran %d\n", tmp);
      }
    }
    return 0;
  }
  int count = 0;
  srand(time(NULL));
  close(pd1[0]);
  while (count < 3) {
    int tmp = rand() % 100;
    write(pd1[1], &tmp, sizeof(int));
    if (tmp % 2 == 1) {
      count++;
    } else {
      count = 0;
    }
  }
  close(pd1[1]);
  wait(NULL);
}
