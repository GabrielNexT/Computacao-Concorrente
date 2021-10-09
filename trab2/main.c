#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <math.h>

#ifdef DEBUG
# define DEBUG_PRINT(x) printf x
#else
# define DEBUG_PRINT
#endif

sem_t emptySlot, fullBlock, wakeConsumer, lockConsumer, lockWriter;

int *buffer, *threadsIds;
int threadsNum, blockSize, totalElements, totalCompleted = 0, bufferSize;
char inputFileName[255], outputFileName[255];
pthread_t* threads;

void readInput(int argc, char const *argv[]) {
  if(argc == 5) { 
    threadsNum = atoi(argv[1]);
    blockSize = atoi(argv[2]);
    strcpy(inputFileName, argv[3]);
    strcpy(outputFileName, argv[4]);
  } else {
    printf("Insira o número de threads: ");
    scanf("%d", &threadsNum);
    printf("Insira o tamanho da entrada: ");
    scanf("%d", &blockSize);
    printf("Insira o nome do arquivo de entrada: ");
    scanf("%s", inputFileName);
    printf("Insira o nome do arquivo de saída: ");
    scanf("%s", outputFileName);
  }

  bufferSize = 10 * blockSize;
  // DEBUG_PRINT(("%d %d %s %s\n", threadsNum, blockSize, inputFileName, outputFileName));
}

void cleanFile() {

}

void reserveMemory() {
  threads = malloc(sizeof(pthread_t) * threadsNum + 1);
  buffer = malloc(sizeof(int) * bufferSize);
  threadsIds = malloc(sizeof(int) * threadsNum + 1);

  if(threads == NULL || buffer == NULL || threadsIds == NULL) {
    printf("Não foi possível alocar memória\n");
    exit(1);
  }

  fclose(fopen("output.txt", "w"));

  sem_init(&emptySlot, 0, bufferSize);
  sem_init(&fullBlock, 0, 0);
  sem_init(&wakeConsumer, 0, 0);
  sem_init(&lockConsumer, 0, 1);
  sem_init(&lockWriter, 0, 1);
}

void freeMemory() {
  free(threads);
  free(threadsIds);
  free(buffer);
}

void insert(int element) {
  static int in = 0;
  static int count = 0;
  sem_wait(&emptySlot);
  buffer[in] = element;
  in = (in + 1) % bufferSize;
  count++;
  if(count == blockSize) {
    sem_post(&fullBlock);
    sem_post(&wakeConsumer);
    count = 0;
  }
}

int* pop(int id) {
  int *itens = malloc(sizeof(int) * blockSize);

  if(itens == NULL) {
    printf("Falha ao alocar memoria\n");
    exit(1);
  }

  static int out = 0;
  sem_wait(&fullBlock);
  sem_wait(&lockConsumer);
  for(int i = 0; i < blockSize; i++) {
    itens[i] = buffer[(out+i) % bufferSize];
    sem_post(&emptySlot);
  }
  out = (out + blockSize) % bufferSize;
  sem_post(&lockConsumer);
  return itens;
}

void *producer(void *params) {
  FILE *inputFile = fopen(inputFileName, "r");

  if(inputFile == NULL) {
    printf("ERROR: Nao foi possivel abrir o arquivo %s.\n", inputFileName);
    exit(1);
  }

  fscanf(inputFile, "%d", &totalElements);

  for(int i = 0; i < totalElements; i++) {
    int tmp;
    fscanf(inputFile, "%d", &tmp);
    insert(tmp);
  }
  fclose(inputFile);
  pthread_exit(NULL);
}

int comparator(const void *a, const void *b) {
  return ( *(int*)a - *(int*)b );
}

void writeBlock(int *vector, int threadId) {
  sem_wait(&lockWriter);

  FILE *outputFile = fopen(outputFileName, "a");

  if(outputFile == NULL) {
    printf("ERROR: Nao foi possivel abrir o arquivo %s.\n", outputFileName);
    exit(1);
  }

  for(int i = 0; i < blockSize; i++) {
    fprintf(outputFile, "%d ", vector[i]);
  }
  fprintf(outputFile, "\n");

  totalCompleted += blockSize;

  if(totalCompleted == totalElements) {
    for(int i = 0; i < threadsNum+1; i++) {
      sem_post(&wakeConsumer);
    }
  }

  fclose(outputFile);
  sem_post(&lockWriter);
}

void *consumer(void *params) {
  int consumerId = *(int*) params;
  DEBUG_PRINT(("Entrei no consumidor %d!\n", consumerId));

  int *localBuffer = malloc(sizeof(int) * blockSize);

  if(localBuffer == NULL) {
    printf("Não foi possível alocar memória para o consumidor %d\n", consumerId);
    exit(1);
  }

  int elemento;
  while (1) {
    DEBUG_PRINT(("Consumer %d dormindo...\n", consumerId));
    sem_wait(&wakeConsumer);
    DEBUG_PRINT(("Consumer %d acordando...\n", consumerId));
    if(totalCompleted == totalElements) break;
    localBuffer = pop(consumerId);
    qsort(localBuffer, blockSize, sizeof(int), comparator);
    writeBlock(localBuffer, consumerId);
  }


  DEBUG_PRINT(("%d morreu\n", consumerId));
  pthread_exit(NULL);
}

void creatingThreads() {

  for(int i = 0; i < threadsNum + 1; i++) {
    threadsIds[i] = i;
  }

  pthread_create(&threads[0], NULL, producer, (void *) &threadsIds[0]);
  for(int i = 1; i < threadsNum + 1; i++) {
    pthread_create(&threads[0], NULL, consumer, (void *) &threadsIds[i]);
  }

  for (int i = 0; i < threadsNum + 1; i++) {
    pthread_join(threads[i], NULL);
  }
}

int main(int argc, char const *argv[]) {
  srand(time(NULL));
  readInput(argc, argv);
  reserveMemory();
  creatingThreads();
  freeMemory();
  return 0;
}

