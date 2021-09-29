#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

sem_t sem, sem2, sem3;
int control = 0; // Variável utilizada para controlar o fluxo de execucução.

// Primeiro thread.
void *T1 (void *params) {
  sem_wait(&sem2);
  printf("Fique a vontade.\n");
  if(control == 1) {
    printf("Controle 4!\n");
    sem_post(&sem3);
  } else {
    control++;
    sem_post(&sem2);
  }
  pthread_exit(NULL);
}

// Segundo thread
void *T2 (void *params) {
  printf("Seja bem-vindo!\n");
  sem_post(&sem2);
  pthread_exit(NULL);
}

// Terceiro thread.
void *T3 (void *params) {
  sem_wait(&sem3);
  printf("Volte sempre!\n");
  pthread_exit(NULL);
}

// Quarto thread.
void *T4 (void *params) {
  sem_wait(&sem2);
  printf("Sente-se por favor.\n");
  if(control == 1) {
    sem_post(&sem3);
  } else {
    control++;
    sem_post(&sem2);
  }
  pthread_exit(NULL);
}


int main(void) {
  pthread_t threads[4];
  int *threadsIds;
  threadsIds = malloc(sizeof(int) * 4);

  sem_init(&sem2, 0, 0);
  sem_init(&sem3, 0, 0);

  for(int i = 0; i < 4; i++) {
    threadsIds[i] = i;
  }

  // Criando os threads.
  pthread_create(&threads[0], NULL, T2, (void *) &threadsIds[0]);
  pthread_create(&threads[1], NULL, T1, (void *) &threadsIds[1]);
  pthread_create(&threads[2], NULL, T4, (void *) &threadsIds[2]);
  pthread_create(&threads[3], NULL, T3, (void *) &threadsIds[3]);

  for (int i = 0; i < 4; i++) {
    pthread_join(threads[i], NULL);
  }

  return 0;
}
