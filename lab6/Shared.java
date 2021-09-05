import java.util.List;

public class Shared {

  private Integer count = 0;
  private List<Integer> list;

  /* 
    Construtor, inica a lista que será compartilhada.
  */
  public Shared(List<Integer> list) {
    this.list = list;
  }

  /* 
    Incrementa o valor da variável, o uso do synchronized
    evita que dois threads executem ao mesmo tempo.
  */
  public synchronized void increment() {
    this.count++;
  }

  public Integer getCount() {
    return this.count;
  }

  public Integer getPosition(int index) {
    return this.list.get(index);
  }
  
}
