public class Game
{

  public int[] TossTwoTimesOfNormalDice() { //丟兩次六面骰 
    NormalDice dice1 = new NormalDice();
    int first = dice1.throwDice();
    int second = dice1.throwDice();
    int[] result = new int[2];
    result[0] = first;
    result[1] = second;
    return result; 
  }

  public int TossOctahedralDice() {  //丟一次八面骰 
    OctahedralDice dice2 = new OctahedralDice();
    return dice2.throwDice();
  }

  public String Versus(Player tom, Player mary) { //決定誰贏了 
    if (tom.sum == mary.sum){   //6面骰總和一樣 
      // 兩人再丟八面骰比大小 
      tom.SetresultOfOctahedralDice( TossOctahedralDice() );
      mary.SetresultOfOctahedralDice( TossOctahedralDice() );
      System.out.println("Tom's 8-sides dices: " + tom.resultOfOctahedralDice);
      System.out.println("Mary's 8-sides dices: " + mary.resultOfOctahedralDice);
      if (tom.resultOfOctahedralDice == mary.resultOfOctahedralDice){
        System.out.println("draw");
        return "draw";
      }
      else if (tom.resultOfOctahedralDice > mary.resultOfOctahedralDice){
        System.out.println("Tom wins");
        return "Tom wins";
      }
      else{
        System.out.println("Mary wins");
        return "Mary wins";
      }
    }
    else if ( tom.sum > mary.sum ) {
      return "Tom wins";
    }
    else {
      return "Mary wins";
    }
  }


}