#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <assert.h>
#include <string.h>


// Defines usados na execução do problema. 
#define ARRAY_SIZE (int) 1e5
#define MAX_VALUE (int) 1e5

typedef long long int lli;

lli array[ARRAY_SIZE];

/* 
  Essa função é chamada por ambas as threads
  A primeira thread fica responsável de executar da esquerda p direita, enquanto a segunda executa da direita p esquerda.
*/
void *pow2(void* arg) {
  int idThread = * (int*) arg;
  printf("Executando thread %d...\n", idThread);

  if(idThread % 2) {
    for(int i = ARRAY_SIZE - 1; i >= ARRAY_SIZE/2; i--) {
      array[i] *= array[i];
    }
  } else {
    for(int i = 0; i < ARRAY_SIZE/2; i++) {
      array[i] *= array[i];
    }
  }

  pthread_exit(NULL);
}

/*
  Esse função é responsável por gerar um vetor inicial com valores aleatórios.
*/
void genArray() {
  for(int i = 0; i < ARRAY_SIZE; i++) {
    array[i] = abs(rand() % MAX_VALUE) + 1;
  }
}

/* 
  Apenas imprime o vetor.
*/
void printArray() {
  for(int i = 0; i < ARRAY_SIZE; i++) {
    printf("%lld ", array[i]);
    if(i > 0 && i % 10 == 0) printf("\n");
  }
  printf("\n");
}

int main(void){
  srand(time(NULL)); // Reseta o seed para melhorar a entropia dos números aleatórios.

  pthread_t idOs[2]; // Id dos threads do SO.
  int idLocal[2]; // Id local dos threads.
  lli generatedArray[ARRAY_SIZE]; // Uma cópia do vetor original, para que a gente possa comparar se o resultado está realmente correto.

  genArray();
  printArray();
  
  memcpy(generatedArray, array, sizeof(generatedArray));

  // Inicia os threads chamando a função pow2.
  for(int i = 0; i < 2; i++) {
    idLocal[i] = i + 1;
    if (pthread_create(&idOs[i], NULL, pow2, (void*) &idLocal[i])) {
      printf("Erro ao criar thread com id %d\n", idLocal[i]); exit(-1);
    }
  }

  // Aguarda o retorno das threads.
  for (int i = 0; i < 2; i++) {
    if (pthread_join(idOs[i], NULL)) {
      printf("Erro ao executar pthread_join\n"); exit(-1); 
    }
  }

  // Verifica se o programa está executando com os valores corretos, caso algum valor esteja errado, o print não será executado. 
  for(int i = 0; i < ARRAY_SIZE; i++) {
    assert(generatedArray[i] * generatedArray[i] == array[i]);
  }
  printArray();

  return 0;
}

