#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>

/* 
  Um produtor e um consumidor
*/

#define BUFFER_SIZE 100

sem_t slotVazio, slotCheio;

int currentElement = 0;
int buffer[BUFFER_SIZE];

int randomNumber() {
  return rand() % BUFFER_SIZE;
}

void insert(int element) {
  static int in = 0;
  sem_wait(&slotVazio);
  buffer[in] = element;
  in = (in + 1) % BUFFER_SIZE;
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
  while(1) {
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
  pthread_t threads[2];
  int *threadsIds;
  threadsIds = malloc(sizeof(int) * 2);

  sem_init(&slotVazio, 0, BUFFER_SIZE);
  sem_init(&slotCheio, 0, 0);
  // sem_init(&sem, 0, 0);

  for(int i = 0; i < 4; i++) {
    threadsIds[i] = i;
  }

  // Criando os threads.
  pthread_create(&threads[0], NULL, producer, (void *) &threadsIds[0]);
  pthread_create(&threads[1], NULL, consumer, (void *) &threadsIds[1]);

  for (int i = 0; i < 4; i++) {
    pthread_join(threads[i], NULL);
  }

  return 0;
}

