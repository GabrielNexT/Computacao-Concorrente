#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex;

/* 
  Foram necessárias duas condições, uma que será compartilhada entre os threads
  1 e 4, e uma para o Thread 3, que marca o fim da execução do algoritmo.
*/
pthread_cond_t cond, condFinish;

int control = 0; // Variável utilizada para controlar o fluxo de execucução.

// Primeiro thread.
void *T1 (void *params) {

  pthread_mutex_lock(&mutex);
  
  if(control != 1 && control != 2) {
    pthread_cond_wait(&cond, &mutex);
  }

  printf("Fique a vontade.\n");
  control++;
  
  // Caso o controle seja 3, quer dizer que falta apenas o ultimo thread para ser executado.
  if(control == 3) pthread_cond_signal(&condFinish);
  else pthread_cond_signal(&cond);

  pthread_mutex_unlock(&mutex);

  pthread_exit(NULL);
}

// Segundo thread
void *T2 (void *params) {
  pthread_mutex_lock(&mutex);
  if(control == 0) {
    printf("Seja bem-vindo!\n");
    control++;
    pthread_cond_signal(&cond);
  }
  pthread_mutex_unlock(&mutex);
  pthread_exit(NULL);
}

// Terceiro thread.
void *T3 (void *params) {
  pthread_mutex_lock(&mutex);
  if(control != 3) {
    pthread_cond_wait(&condFinish, &mutex);
  }
  printf("Volte sempre!\n");
  control++;
  pthread_mutex_unlock(&mutex);
  pthread_exit(NULL);
}

// Quarto thread.
void *T4 (void *params) {

  pthread_mutex_lock(&mutex);
  
  if(control != 1 && control != 2) {
    pthread_cond_wait(&cond, &mutex);
  }

  printf("Sente-se por favor.\n");
  control++;
  if(control == 3) pthread_cond_signal(&condFinish);
  else pthread_cond_signal(&cond);

  pthread_mutex_unlock(&mutex);

  pthread_exit(NULL);
}


int main(void) {
  pthread_t threads[4];
  int *threadsIds;
  threadsIds = malloc(sizeof(int) * 4);

  // Criando as condições e o lock.
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init (&cond, NULL);
  pthread_cond_init (&condFinish, NULL);

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

  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);

  return 0;
}
