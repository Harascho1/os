#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#define ERR_SCANF "Err: Scanf vraca 0\n"

sem_t sem_read, sem_write;

void *thread_func(void *arg) {
  int *brojac = (int *)arg;
  while (true) {
    sem_wait(&sem_write);
    if (*brojac == 0) {
      break;
    }
    for (; *brojac >= 0; (*brojac)--) {
      printf("%d\n", *brojac);
      fflush(stdout);
      sleep(4);
    }
    sem_post(&sem_read);
  }
  return NULL;
}

int main() {
  int broj;
  pthread_t nit;

  sem_init(&sem_read, 0, 1);
  sem_init(&sem_write, 0, 0);

  pthread_create(&nit, NULL, thread_func, (void *)&broj);
  while (broj != 0) {
    sem_wait(&sem_read);
    printf("Unesite broj:");
    if (scanf("%d", &broj) == 0) {
      printf(ERR_SCANF);
    }
    sem_post(&sem_write);
  }
  printf("Brisemo sve\n");
  pthread_join(nit, NULL);
  sem_destroy(&sem_read);
  sem_destroy(&sem_write);
  printf("Uspesno se zavrsio program\n");
}
