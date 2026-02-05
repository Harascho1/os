#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int buff[4] = {0};
sem_t sem_gen;
sem_t sem_sum;

void *gen(void *args) {
  int a = *(int *)args;
  while (sem_wait(&sem_gen) == 0 && buff[0] < 250) {
    for (int i = 0; i < 4; i++) {
      buff[i] = rand() % 80;
    }

    sem_post(&sem_sum);
  }
  sem_post(&sem_sum);
  return NULL;
}

int main() {
  srand(time(NULL));

  pthread_t nit;
  sem_init(&sem_gen, 0, 1);
  sem_init(&sem_sum, 0, 0);

  pthread_create(&nit, NULL, gen, NULL);

  int sum = 0;
  while (sem_wait(&sem_sum) == 0 && buff[0] < 250) {
    for (int i = 0; i < 4; i++) {
      sum += buff[i];
    }
    printf("Sum :%d\n", sum);
    fflush(stdout);
    if (sum < 50) {
      printf("Sum je manja od 50");
      fflush(stdout);
    }
    buff[0] = sum;
    sem_post(&sem_gen);
  }

  pthread_join(nit, NULL);

  sem_destroy(&sem_sum);
  sem_destroy(&sem_gen);

  return 0;
}
