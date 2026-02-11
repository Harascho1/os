#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

sem_t sem_other;
sem_t sem_7;

void *print_other(void *args) {
  int *broj = (int *)args;
  for (int i = 1; i <= *broj; i++) {
    if (i % 7 == 0) {
      sem_post(&sem_7);
      sem_wait(&sem_other);
      continue;
    }
    printf("1. print: %d\n", i);
    fflush(stdout);
    sleep(1);
  }
  sem_post(&sem_7);
  return NULL;
}

void *print_7(void *args) {
  int *broj = (int *)args;
  sem_wait(&sem_7);
  for (int i = 7; i <= *broj; i += 7) {
    printf("2. print: %d\n", i);
    fflush(stdout);
    sleep(1);
    sem_post(&sem_other);
    sem_wait(&sem_7);
  }
  return NULL;
}
int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Fali argument [program koji pozivas] [Koliko brojeva da "
           "odstampa]\nnpr. ./a.out 10\nili ./zad1 10\n");
    return -1;
  }

  pthread_t thread;
  pthread_t thread_7;

  sem_init(&sem_other, 0, 0);
  sem_init(&sem_7, 0, 0);

  int num = atoi(argv[1]);
  pthread_create(&thread, NULL, print_other, (void *)&num);
  pthread_create(&thread_7, NULL, print_7, (void *)&num);

  pthread_join(thread, NULL);
  pthread_join(thread_7, NULL);

  sem_destroy(&sem_other);
  sem_destroy(&sem_7);

  printf("successfully exited\n");

  return 0;
}
