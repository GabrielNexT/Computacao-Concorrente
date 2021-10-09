import random
import os
import time

def isSorted(l):
  for i in range(1, len(l) - 1):
    if(l[i-1] > l[i]):
      return False
  return True

def generateTest(id: int):
  blockSize = random.randint(5, 10) 
  consumers = random.randint(1, 50)

  inputElements = []
  outputElement = []

  with open("input.txt", "w+") as inputFile:
    totalElements = random.randint(1, 2) * 10 * blockSize
    current = 0
    inputFile.write(str(totalElements)+"\n")
    while current < totalElements:
      dado = random.randint(1, 1e8)
      inputFile.write(str(dado)+"\n")
      inputElements.append(dado)
      current += 1

  print(f"Testando com {blockSize} bloco(s) e {consumers} consumidores e {totalElements} elementos.")

  os.system(f"gcc main.c -pthread -lm && ./a.out {23} {5} input.txt output.txt")

  with open("output.txt", "r") as outputFile:
    for line in outputFile.readlines():
      numbers = line.split(" ")
      numbers.pop()
      numbers = [int(i) for i in numbers]
      if not isSorted(numbers): 
        print("Linha não ordenada!")
        exit()
      for i in numbers: 
        outputElement.append(i)    

  inputElements.sort()
  outputElement.sort()

  if(inputElements == outputElement):
    print(f"\033[92mTeste {id} concluído, todas as linhas ordenadas e elementos iguais. \033[00m")

for i in range(1, 101):
  generateTest(i)
