#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

#define ERR_EXECL "Nije uspeo da pokrene drugi proces\n"

int main(int argc, char *argv[]) {
  if (fork() == 0) {
    execl(argv[1], argv[1], argv[2], NULL);
    printf(ERR_EXECL);
  }
  wait(NULL);
  printf("Proces dete je zavrsio\n");
  return 0;
}
