public class Player
{
  private int firstTossOfNormalDice, secondTossOfNormalDice;
  public  int resultOfOctahedralDice;
  public  int sum;

  public void play() {
    Game game = new Game();
    int[] result = new int[2];
    result = game.TossTwoTimesOfNormalDice();
    Set1stTossOfNormalDice( result[0] ); //  第一次投擲 
    Set1stTossOfNormalDice( result[1] ); //  第二次投擲 
    sum = result[0] + result[1];
  }

  public void Set1stTossOfNormalDice( int firstToss ) {
     firstTossOfNormalDice = firstToss;
  }

  public void Set2ndTossOfNormalDice( int secondToss ) {
     secondTossOfNormalDice = secondToss;
  }

  public void SetresultOfOctahedralDice( int toss ) {
     resultOfOctahedralDice = toss;
  }

  public int TestSingleToss() {
    NormalDice dice = new NormalDice();
    return dice.throwDice();
  }
}