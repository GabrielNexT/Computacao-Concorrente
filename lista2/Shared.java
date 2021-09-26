public class Shared {

  private Integer count;
  private Integer readerCount;
  private Integer writerCount;
  private Integer writerWaiting;

  public static final String ANSI_RESET = "\u001B[0m";
  public static final String ANSI_BLACK = "\u001B[30m";
  public static final String ANSI_RED = "\u001B[31m";
  public static final String ANSI_GREEN = "\u001B[32m";
  public static final String ANSI_YELLOW = "\u001B[33m";
  public static final String ANSI_BLUE = "\u001B[34m";
  public static final String ANSI_PURPLE = "\u001B[35m";
  public static final String ANSI_CYAN = "\u001B[36m";
  public static final String ANSI_WHITE = "\u001B[37m";

  public Shared() {
    this.count = 0;
    this.readerCount = 0;
    this.writerCount = 0;
    this.writerWaiting = 0;
  }

  synchronized void joinReader(Integer id) {
    try {
      while(this.writerCount > 0 || this.writerWaiting > 0) {
        System.out.println(
          String.format(
            "%sLeitor %d aguardando entrada...%s",
            ANSI_YELLOW,
            id,
            ANSI_RESET
          )
        );
        wait();
      }
      this.readerCount++;
      System.out.println(String.format("Thread %d entrou para leitura.", id));
    } catch (InterruptedException e) {
      // TODO Auto-generated catch block
      e.printStackTrace();
    }
  }

  synchronized void leaveReader(Integer id) {
    this.readerCount--;
    System.out.println(String.format("Leitor %d saindo!", id));
    if (this.readerCount == 0) {
      this.notify();
    }
  }

  synchronized void joinWriter(Integer id) {
    boolean inQueue = false;
    try {
      while(this.readerCount > 0) {
        this.writerWaiting++;
        inQueue = true;
        System.out.println(
          String.format(
            "%sEscritor %d aguardando entrada, fila: %d %s",
            ANSI_YELLOW,
            id,
            this.writerWaiting,
            ANSI_RESET
          )
        );
        wait();
      }
      this.writerCount++;
      if(inQueue) this.writerWaiting--;
      System.out.println(String.format("Thread %d entrou para escrita!", id));
    } catch (InterruptedException e) {
      // TODO Auto-generated catch block
      e.printStackTrace();
    }
  }

  synchronized void leaveWriter(Integer id) {
    this.writerCount--;
    this.notifyAll();
    System.out.println(String.format("%sEscritor %d saindo, fila: %d %s", ANSI_RED, id, this.writerWaiting, ANSI_RESET));
  }

  synchronized Integer getCount() {
    return this.count;
  }

  synchronized void setCount(Integer count) {
    this.count = count;
  }
}
