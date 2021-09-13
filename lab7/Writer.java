public class Writer extends Thread {

  private int id;
  private Shared shared;

  public Writer(int id, Shared shared) {
    this.id = id;
    this.shared = shared;
  }

  public void run() {
    // Trava a classe shared e escreve o valor do ID.
    this.shared.joinWriter(this.id);
    this.shared.setCount(this.id);
    this.shared.leaveWriter(this.id);
  }
}
