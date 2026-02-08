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
    msgrcv(msgid, &MSG, sizeof(MSG.buf), 1, 0);
    suma += MSG.buf;
    printf("printujem: %d\n", MSG.buf);
  }
  MSG.buf = 2;
  MSG.type = 2;
  msgsnd(msgid, &MSG, sizeof(MSG.buf), 0);
  return 0;
}
