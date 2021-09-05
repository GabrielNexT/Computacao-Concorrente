public class Worker extends Thread {
  private int id;
  private Shared shared;
  private int start;
  private int end;

  public Worker(int id, int start, int end, Shared shared) {
    this.id = id;
    this.start = start;
    this.end = end;
    this.shared = shared;
  }

  /* 
    Percorre a posição entre o start e end.
    Caso o valor na posição seja par, incrementa o valor no shared.
  */
  public void run() {
    for(int i = start; i < end; i++) {
      if(shared.getPosition(i) % 2 == 0) {
        shared.increment();
      }
    }
  }
}
