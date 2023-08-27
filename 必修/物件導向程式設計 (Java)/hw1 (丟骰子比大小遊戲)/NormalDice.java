
public class NormalDice extends Dice
{
    private int sum;
    public NormalDice() { // constructor 
      setFaces();
    }
    
    void setFaces(){
       faces = 6; 
    }

    public void SetSumOfTwoRolls( int first, int second ) {
      sum = first + second;
    }

    public int GetSum() {
      return sum;
    }
}
