#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define FATAL(text)                                                            \
  printf("%s\n", text);                                                        \
  fflush(stdout);                                                              \
  exit(-1)

sem_t sem_print, sem_read;

void *print_async(void *args) {
  while (sem_wait(&sem_print) == 0) {
    int num = *(int *)args;
    if (num == 999) {
      break;
    }
    printf("Stampanje:\n");
    for (int i = 0; i <= num; i++) {
      printf("%d\n", i);
      fflush(stdout);
      sleep(1);
    }
    sem_post(&sem_read);
  }
  return NULL;
}

int main() {
  sem_init(&sem_read, 1, 1);
  sem_init(&sem_print, 0, 0);

  int buf_num = 0;

  pthread_t nit;
  pthread_create(&nit, NULL, print_async, (void *)&buf_num);

  while (sem_wait(&sem_read) == 0) {
    printf("Unesi novi broj (999 terminira program): ");
    if (scanf("%d", &buf_num) == 0) {
      FATAL("los scanf");
    }
    sem_post(&sem_print);
    if (buf_num == 999) {
      break;
    }
  }

  pthread_join(nit, NULL);
  sem_destroy(&sem_read);
  sem_destroy(&sem_print);

  return 0;
}
