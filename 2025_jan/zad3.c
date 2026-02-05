
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define N 10

typedef struct {
  long type;
  int data;
} msg_buf;

int main() {
  int msgid = msgget(10104, 0666 | IPC_CREAT);
  msg_buf msg;

  if (fork() == 0) {
    for (int count = N; count > 0; count--) {
      msgrcv(msgid, &msg, sizeof(int), 0, 0);

      int num = msg.data;
      int sum = 0;
      while (num > 0) {
        sum += num % 10;
        num /= 10;
      }

      printf("Suma, cifara je = %d\n", sum);
      fflush(stdout);
    }
    printf("Zavrsilo se dete\n");
    return 0;
  } else {
    for (int count = N; count > 0; count--) {
      scanf("%d", &msg.data);
      msg.type = 1;
      msgsnd(msgid, &msg, sizeof(int), 0);
    }
    printf("Zavrsio cale\n");
    wait(NULL);
    printf("Zavrsilo program\n");
  }
}
