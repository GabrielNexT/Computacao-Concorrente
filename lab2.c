#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <assert.h>
#include <string.h>
#include "timer.h"

#define MAX_RANDOM_SIZE 9

typedef long long int lli;
typedef struct {
  int id;
  int start;
  int end;
} tArgs;

/* 
  Variaveis necessária para a execução do código.
*/
int matSize, numThreads;
lli *matrix, *matrixAux, *matrixResult;
pthread_t *osId, *localId;
tArgs *params;

/* 
  Função responsável por ler o input do usuário de uma forma amigável.
*/
void readInput() {
  printf("\n");
  printf("Insira o número de threads: ");
  scanf("%d", &numThreads);
  printf("Insira o tamanho da matriz: ");
  scanf("%d", &matSize);
  printf("\n");
}

/* 
  Função para reservar toda a memória necessária, baseada no input do usuário.
*/
void reserveMemory () {
  matrix = malloc(sizeof(lli) * matSize * matSize);
  matrixAux = malloc(sizeof(lli) * matSize * matSize);
  matrixResult = malloc(sizeof(lli) * matSize * matSize);
  osId = malloc(sizeof(pthread_t) * numThreads);
  localId = malloc(sizeof(pthread_t) * numThreads);
  params = malloc(sizeof(tArgs) * numThreads);
  if(matrix == NULL || matrixAux == NULL || matrixResult == NULL) {
    printf("Falha ao alocar memoria");
    exit(1);
  }
}

/* 
  Libera a memória alocada.
*/
void freeMemory() {
  free(matrix);
  free(matrixAux);
  free(matrixResult);
  free(osId);
  free(localId);
  free(params);
}

/* 
  Retorna um número inteiro aleatório, foi usado um número pequeno para evitar overflows.
*/
int getRandomInt() {
  return (rand() % MAX_RANDOM_SIZE) + 1;
}

/* 
  Gera as duas matrizes aleatórias.
*/
void genMatrix() {
  for(int i = 0; i < matSize; i++) {
    for(int j = 0; j < matSize; j++) {
      matrix[i*matSize+j] = getRandomInt();
      matrixAux[i*matSize+j] = getRandomInt();
    }
  }
}

/* 
  Imprime uma matriz específica.
*/
void printMatrix(lli *matrixTmp) {
  for(int i = 0; i < matSize; i++) {
    for(int j = 0; j < matSize; j++) {
      printf("%lld ", matrixTmp[i*matSize+j]);
    }
    printf("\n");
  }
  printf("\n");
}

/* 
  Imprime as duas matrizes e a matriz resultante.
*/
void printMatrixs() {
  printMatrix(matrix);
  printMatrix(matrixAux);
  printMatrix(matrixResult);
}

/* 
  Função que será chamada pelos Threads.
*/
void *cellMultiplication(void* arg) {
  tArgs *args = (tArgs*) arg;

  for(int i = args->start; i < args->end; i++) {
    int count = 0;
    int line = i/matSize;
    int col = i - (line*matSize);
    for(int k = 0; k < matSize; k++) {
      count += matrix[line*matSize+k]*matrixAux[k*matSize+col];
    }
    matrixResult[line*matSize+col] = count;
  }
}

/* 
  Função responsável por criar os Threads e passar seus respectivos parâmetros.
*/
void matrixMultiplication() {
  int totalCells = (matSize * matSize);
  int qtdThread = totalCells / numThreads;
  for(int i = 0; i < numThreads; i++) {
    localId[i] = i;
    (params+i)->id = i;
    (params+i)->start = qtdThread * i;
    (params+i)->end = (i == numThreads - 1 ? totalCells: (params+i)->start + qtdThread); 

    if (pthread_create(&osId[i], NULL, cellMultiplication, (void*) (params+i))) {
      printf("Erro ao criar thread com id %d\n", i); exit(-1);
    }
  }

  for (int i = 0; i < numThreads; i++) {
    if (pthread_join(osId[i], NULL)) {
      printf("Erro ao executar pthread_join\n"); exit(-1); 
    }
  }
}

/* 
  Função que verifica se o resultado da anteior está correto, usando o algoritmo n^3.
*/
void verifyResult() {
  for(int i = 0; i < matSize; i++) {
    for(int j = 0; j < matSize; j++) {
      int count = 0;
      for(int k = 0; k < matSize; k++) {
        count += matrix[i*matSize+k]*matrixAux[k*matSize+j];
      }
      assert(matrixResult[i*matSize+j] == count);
    }
  }
}

/* 
  A main
*/
int main(int argc, char const *argv[]) {
  srand(time(NULL));
  int running = 1;
  double begin, end;
  while(running) {
    readInput();

    GET_TIME(begin);
    reserveMemory();
    GET_TIME(end);
    printf("Tempo para reservar a memoria: %lf\n", end-begin);

    GET_TIME(begin);
    genMatrix();
    GET_TIME(end);
    printf("Tempo para gerar as matrizes: %lf\n", end-begin);

    GET_TIME(begin);
    matrixMultiplication();
    GET_TIME(end);
    printf("Tempo para multiplicar usando os threads: %lf\n", end-begin);

    GET_TIME(begin);
    verifyResult();
    GET_TIME(end);
    printf("Tempo para verificar o resultado: %lf\n", end-begin);

    GET_TIME(begin);
    freeMemory();
    GET_TIME(end);
    printf("Tempo para liberar a memoria: %lf\n", end-begin);
  }

}