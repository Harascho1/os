#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

#define SIZE 64

typedef struct {
  long type;
  int buf;
} msg_buf;

#define LOG(text) printf("%s\n", text)

int main() {
  int msgid = msgget(10104, 0666 | IPC_CREAT);
  msg_buf MSG;
  char buf[10];
  sprintf(buf, "%d", msgid);

  if (fork() == 0) {
    execl("zad3a", "zad3a", buf, NULL);
    LOG("Nisam uspeo");
  }
  if (fork() == 0) {
    execl("zad3b", "zad3b", buf, NULL);
    LOG("Nisam uspeo2");
  }

  msgrcv(msgid, &MSG, sizeof(MSG.buf), 2, 0);
  if (MSG.buf == 1) {
    LOG("prvi proces je zavrsio");
  }
  msgrcv(msgid, &MSG, sizeof(MSG.buf), 2, 0);
  if (MSG.buf == 2) {
    LOG("drugi proces je zavrsio");
  }
  return 0;
}
