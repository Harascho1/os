#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

#define SIZE 64

typedef struct {
  long type;
  int buf;
} msg_buf;

int main(int argc, char *argv[]) {
  int msgid = atoi(argv[1]);
  msg_buf MSG;
  int suma = 0;
  while (suma < 1000) {

    if (scanf("%d", &MSG.buf) == 0) {
      printf("Ne radi komso\n");
    }
    suma += MSG.buf;
    MSG.type = 1;
    msgsnd(msgid, &MSG, sizeof(MSG.buf), 0);
  }
  MSG.buf = 1;
  MSG.type = 2;
  msgsnd(msgid, &MSG, sizeof(MSG.buf), 0);

  return 0;
}
