#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <threads.h>
#include <unistd.h>

enum { stara = 1, nova = 2 };

typedef struct {
  long type;
  char buf[100];
} MSG;

int main() {
  int msgid = msgget(10104, 0666 | IPC_CREAT);
  MSG message;
  if (fork() == 0) {
    // NOTE: prvi program

    FILE *stara_akreditacija = fopen("stara.txt", "w");
    while (msgrcv(msgid, &message, sizeof(message.buf), stara, 0) > 0) {
      if (strlen(message.buf) == 0)
        break;
      fprintf(stara_akreditacija, "%s\n", message.buf);
    }
    fclose(stara_akreditacija);
    return 0;
  }
  if (fork() == 0) {
    // NOTE: drugi program

    FILE *nova_akreditacija = fopen("nova.txt", "w");
    while (msgrcv(msgid, &message, 100, nova, 0) > 0) {
      if (strlen(message.buf) == 0)
        break;
      fprintf(nova_akreditacija, "%s\n", message.buf);
    }
    fclose(nova_akreditacija);
    return 0;
  } else {
    // NOTE: Cale program

    while (fgets(message.buf, 100, stdin) != 0) {
      message.buf[strcspn(message.buf, "\n")] = '\0';
      if (strcmp(message.buf, "10000") == 0)
        break;

      if (atoi(message.buf) <= 17000) {
        message.type = stara;
      } else {
        message.type = nova;
      }

      msgsnd(msgid, &message, sizeof(message.buf), 0);
    }

    strcpy(message.buf, "");
    message.type = 1;
    msgsnd(msgid, &message, sizeof(message.buf), 0);
    message.type = 2;
    msgsnd(msgid, &message, sizeof(message.buf), 0);
  }
  wait(NULL);
  wait(NULL);
  return 0;
}
