import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class Main {
  
  // A Main
  public static void main(String[] args) {
    if(args.length < 2) {
      throw new RuntimeException("Insira o tamanho do vetor e o número de threads.");
    }
    
    Integer vectorSize = Integer.parseInt(args[0]); // Tamanho do vetor
    Integer numThreads = Integer.parseInt(args[1]); // Número de threads
    Thread[] threads = new Thread[numThreads]; // Threads
    List<Integer> randomList = new ArrayList<Integer>(); // Lista aleatória que será usada no problema.
    Random rand = new Random(); // Classe para gerar os números aleatórios.

    /* 
      Adicionando números aleatórios na lista, foi usado apenas 0 e 1 para facilitar.
    */
    for(int i = 0; i < vectorSize; i++) {
      randomList.add(rand.nextInt(2)); 
    }

    Shared shared = new Shared(randomList);
    int sizePerThread = vectorSize / numThreads;
    int offset = vectorSize % numThreads;
    
    /* 
      Criando classes que serão executadas pelos Threads.
    */
    for(int i = 0; i < numThreads; i++) {
      int start = i*sizePerThread;
      int end = (i+1)*sizePerThread;
      if(i == numThreads - 1) end += offset;
      threads[i] = new Worker(i, start, end, shared);
    }

    /* 
      Iniciando os Threads.
    */
    for(int i = 0; i < numThreads; i++) {
      threads[i].start();
    }

    /* 
      Aguardando o fim da execução de cada Thread.
    */
    for(int i = 0; i < numThreads; i++) {
      try {
        threads[i].join();
      } catch (InterruptedException e) {
        e.printStackTrace();
        return;
      }
    }
    
    // System.out.println(randomList.toString());
    System.out.println(String.format("Quantidade de pares %d.", shared.getCount()));
  }
}