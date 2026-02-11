#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define SEM_KEY1 10101
#define SEM_KEY2 10102
#define SHM_MEM 10103

typedef struct {
  int arr[5];
  int sum;
} shared;

typedef union {
  int val;
} semun;

#define LOG(text) printf("%s\n", text)
#define FATAL(text)                                                            \
  LOG(text);                                                                   \
  exit(-1)

int main() {
  // NOTE: semafori
  struct sembuf sem_lock = {0, -1, 0};
  struct sembuf sem_unlock = {0, 1, 0};

  int sem_print = semget((key_t)SEM_KEY1, 1, IPC_CREAT | 0666);
  if (sem_print < 0) {
    FATAL("nece sem_print");
  }
  int sem_acc = semget((key_t)SEM_KEY2, 1, IPC_CREAT | 0666);
  if (sem_acc < 0) {
    FATAL("nece sem_acc");
  }

  semun opts;
  opts.val = 1;
  semctl(sem_print, 0, SETVAL, opts);
  opts.val = 0;
  semctl(sem_acc, 0, SETVAL, opts);

  // NOTE: memorija
  int memid = shmget((key_t)SHM_MEM, sizeof(shared), IPC_CREAT | 0666);
  if (memid < 0) {
    FATAL("nece deljena");
  }
  shared *ptr = (shared *)shmat(memid, NULL, 0);

  if (fork()) {
    while (1) {
      semop(sem_acc, &sem_lock, 1);
      ptr->sum = 0;
      for (int i = 0; i < 5; i++) {
        ptr->sum += ptr->arr[i];
      }
      semop(sem_print, &sem_unlock, 1);
      if (ptr->sum == 0) {
        break;
      }
    }
    return 0;
  } else {
    while (1) {
      semop(sem_print, &sem_lock, 1);
      for (int i = 0; i < 5; i++) {
        scanf("%d", &ptr->arr[i]);
      }
      semop(sem_acc, &sem_unlock, 1);
      semop(sem_print, &sem_lock, 1);
      printf("suma je: %d\n", ptr->sum);
      fflush(stdout);
      semop(sem_print, &sem_unlock, 1);
      if (ptr->sum == 0) {
        break;
      }
    }
  }
  wait(NULL);
  semctl(sem_print, 0, IPC_RMID, 0);
  semctl(sem_acc, 0, IPC_RMID, 0);
  shmctl(memid, IPC_RMID, 0);
  return 0;
}
