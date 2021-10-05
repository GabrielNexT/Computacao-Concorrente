#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>

/* 
  Muitos produtores e um consumidor
*/

#define BUFFER_SIZE 1000
#define NUM_THREAD 20

sem_t slotVazio, slotCheio;
sem_t mutexProd;

int currentElement = 0;
int buffer[BUFFER_SIZE];

int randomNumber() {
  return rand() % BUFFER_SIZE;
}

void insert(int element) {
  static int in = 0;
  sem_wait(&slotVazio);
  sem_wait(&mutexProd);
  buffer[in] = element;
  in = (in + 1) % BUFFER_SIZE;
  sem_post(&mutexProd);
  sem_post(&slotCheio);
}

int pop() {
  int item;
  static int out = 0;
  sem_wait(&slotCheio);
  item = buffer[out];
  out = (out + 1) % BUFFER_SIZE;
  sem_post(&slotVazio);
  return item;
}

void *producer(void *params) {
  int elemento;
  int qtd = 1;
  while(qtd--) {
    elemento = randomNumber();
    insert(elemento);
    printf("Elemento inserido! %d\n", elemento);
  }
  pthread_exit(NULL);
}

void *consumer(void *params) {
  int elemento;
  while(1) {
    elemento = pop();
    printf("Elemento removido! %d\n", elemento);
  }
  pthread_exit(NULL);
}

int main(int argc, char const *argv[]) {
  srand(time(NULL));
  pthread_t threads[NUM_THREAD];
  int *threadsIds;
  threadsIds = malloc(sizeof(int) * NUM_THREAD);

  sem_init(&mutexProd, 0, 1);
  sem_init(&slotVazio, 0, BUFFER_SIZE);
  sem_init(&slotCheio, 0, 0);
  // sem_init(&sem, 0, 0);

  for(int i = 0; i < NUM_THREAD; i++) {
    threadsIds[i] = i;
  }

  // Criando os threads.
  pthread_create(&threads[0], NULL, consumer, (void *) &threadsIds[0]);
  for(int i = 1; i < NUM_THREAD; i++) {
    pthread_create(&threads[i], NULL, producer, (void *) &threadsIds[i]);
  }

  for (int i = 0; i < NUM_THREAD; i++) {
    pthread_join(threads[i], NULL);
  }

  return 0;
}

