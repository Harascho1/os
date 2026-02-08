#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define LOG(text)                                                              \
  printf("%s\n", text);                                                        \
  fflush(stdout)

#define FATAL(text)                                                            \
  LOG(text);                                                                   \
  exit(1)

#define BUFF_SIZE 100

int main(int argc, char *argv[]) {
  FILE *f;
  char buff[BUFF_SIZE];
  int pd1[2];
  int pd2[2];
  pipe(pd1);
  pipe(pd2);
  if (fork() == 0) {
    f = fopen(argv[2], "w");
    int i_xor = 0;
    if (f == NULL) {
      FATAL("1. fajl ne moze da se ucita");
    }
    close(pd1[1]);
    close(pd2[0]);
    while (true) {
      int read_bytes = read(pd1[0], &buff, BUFF_SIZE);
      if (read_bytes == 0)
        break;

      printf("procitano:\n%s\n\n", buff);
      fflush(stdout);
      for (int i = 0; i < read_bytes; i++) {
        buff[i] ^= i_xor++;
      }

      fwrite(buff, sizeof(char), read_bytes, f);
    }
    close(pd1[0]);
    close(pd2[1]);
    fclose(f);
  }

  f = fopen(argv[1], "r");
  close(pd1[0]);
  close(pd2[1]);
  if (f == NULL) {
    FATAL("1. fajl ne moze da se ucita");
  }
  int bytes_read = 0;
  while ((bytes_read = fread(buff, sizeof(char), BUFF_SIZE, f))) {
    printf("bytes_read: %d\n", bytes_read);
    if (bytes_read != 100) {
      LOG("promeni buff");
      buff[bytes_read++] = '\0';
    }
    write(pd1[1], buff, bytes_read);
  }
  close(pd1[1]);
  close(pd2[0]);
  wait(NULL);
  return 0;
}
