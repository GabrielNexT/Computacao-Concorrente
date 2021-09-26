
public class Reader extends Thread {

  private int id;
  private Shared shared;

  public static final String ANSI_RESET = "\u001B[0m";
  public static final String ANSI_GREEN = "\u001B[32m";

  public Reader(int id, Shared shared) {
    this.id = id;
    this.shared = shared;
  }

  public void run() {
    /*
      Le de forma segura a variável atual e faz os outros threads aguardarem a verificação da primalidade.
      Note que não é necessário que os outros threads aguardem o print, logo o leaveReader foi executado logo após o fim da leitura.
    */
    this.shared.joinReader(this.id);
    Integer count = this.shared.getCount();
    this.shared.leaveReader(this.id);
  }
}
