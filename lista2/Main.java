import java.util.Arrays;
import java.util.Collections;
import java.util.List;

public class Main {

  public static void main(String[] args) {
    if (args.length < 2) {
      throw new RuntimeException(
        "Insira o número de leitores, escritores e leitorEscritor no argv!"
      );
    }

    Integer readerNum = Integer.parseInt(args[0]);/* Número de leitores */
    Integer writerNum = Integer.parseInt(args[1]);/* Número de escritor */

    // Inicia os vetores de Thread.
    Thread[] threads = new Thread[readerNum + writerNum];

    /* Classe compartilhada entre os threads, a mesma também faz o papel de monitor. */
    Shared shared = new Shared();
    String exetutionOrder = "";/* String que guardar a ordem de execução */

    for (int i = 0; i < readerNum; i++) exetutionOrder += "R";
    for (int i = 0; i < writerNum; i++) exetutionOrder += "W";

    /*
      Optei por embaralhar a ordem de execução dos threads, para obter resultados um pouco mais aleatórios.
      Acho que dessa forma ficou mais fácil de ver o funcionamento do monitor.
      As 5 linhas adiante, pega o vetor exetutionOrder e embaralha os elementos.
    */
    List<String> letters = Arrays.asList(exetutionOrder.split(""));
    Collections.shuffle(letters);
    String tmp = "";
    for (String letter : letters) tmp += letter;
    exetutionOrder = tmp;

    System.out.println("Ordem de execução: " + exetutionOrder);

    /*
      Cria as clases dos threads, note que:
        R representa Reader, de leitura;
        W representa Writer, de escrita;
        X representa ReaderWriter, de leitura e escrita.
    */
    for (int i = 0; i < exetutionOrder.length(); i++) {
      char threadType = exetutionOrder.charAt(i);
      Integer id = i + 1;
      switch (threadType) {
        case 'R':
          threads[i] = new Reader(id, shared);
          break;
        case 'W':
          threads[i] = new Writer(id, shared);
          break;
      }
    }

    /* 
      Inícia todos os threads.
    */
    for (int i = 0; i < threads.length; i++) {
      threads[i].start();
    }

    /* 
      Aguarda o fim da execução.
    */
    for (int i = 0; i < threads.length; i++) {
      try {
        threads[i].join();
      } catch (InterruptedException e) {
        e.printStackTrace();
        return;
      }
    }
  }
}
