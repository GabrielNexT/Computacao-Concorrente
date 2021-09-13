public class ReaderWriter extends Thread {

  private int id;
  private Shared shared;

  public static final String ANSI_RESET = "\u001B[0m";
  public static final String ANSI_GREEN = "\u001B[32m";

  public ReaderWriter(int id, Shared shared) {
    this.id = id;
    this.shared = shared;
  }

  public void run() {
    /* 
      Trava a classe shared para ler o valor do contador.
    */
    this.shared.joinReader(this.id);
    Integer tmpCounter = this.shared.getCount();
    this.shared.leaveReader(this.id);

    /* 
      Verifica a paridade do número
    */
    if (tmpCounter % 2 == 0) {
      System.out.println(
        String.format(
          "%sO número %d é par! %s",
          ANSI_GREEN,
          tmpCounter,
          ANSI_RESET
        )
      );
    } else {
      System.out.println(
        String.format(
          "%sO número %d é ímpar! %s",
          ANSI_GREEN,
          tmpCounter,
          ANSI_RESET
        )
      );
    }

    /* 
      Escreve o dobro do valor lido na classe shared, note
      que é possível que a classe tenha um valor diferente de tmpCounter,
      pois a partir do momento em que a gente libera o reader, outros threads podem ter escritos na classe.
    */
    this.shared.joinWriter(this.id);
    this.shared.setCount(2 * tmpCounter);
    this.shared.leaveWriter(this.id);
  }
}
