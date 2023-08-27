public class TestDice
{
    public static void MathRandom() {
        Player testPlayer = new Player();
        int one = 0, two = 0, three = 0, four = 0, five = 0, six = 0;   //統計次數 
        int error = 0;
        for (int i = 0; i < 10000; i++){
            int num = testPlayer.TestSingleToss();   //Math.random隨機骰1~6
            if (num == 1){
                one++;
            }
            else if (num == 2){
                two++;
            }
            else if (num == 3){         
                three++;
            }
            else if (num == 4){
                four++;
            }
            else if (num == 5){
                five++;
            }
            else if (num == 6){
                six++;
            }
            else{
                error++;
            }
        }
        
        System.out.println("MathRandom, times of one: " + one);
        System.out.println("MathRandom, times of two: " + two);
        System.out.println("MathRandom, times of three: " + three);
        System.out.println("MathRandom, times of four: " + four);
        System.out.println("MathRandom, times of five: " + five);
        System.out.println("MathRandom, times of six: " + six);
        System.out.println("MathRandom, times of error: " + error);
    }
    
    public static void shuffle() {  //Fisher–Yates shuffle演算法 
        int one=0, two=0, three=0, four=0, five=0, six=0;   //統計次數 
        int error = 0;
        
        for (int i = 0; i < 10000; i++){
            int[] dice = {1, 2, 3, 4, 5, 6};    //指定骰子的所有點數 
    
            for (int j = dice.length-1; j >= 0; j--){
              int zeroToN = (int)Math.floor(Math.random()*(j + 1)); //第n個元素與arr[0~n]中隨機擇一交換 
              int temp = dice[j];
              dice[j] = dice[zeroToN];
              dice[zeroToN] = temp;
            }
            
            int result = dice[0];   //統一取shuffle完陣列中最前元素  
            
            if (result == 1){
                one++;
            }
            else if (result == 2){
                two++;
            }
            else if (result == 3){         
                three++;
            }
            else if (result == 4){
                four++;
            }
            else if (result == 5){
                five++;
            }
            else if (result == 6){
                six++;
            }
            else{
                error++;
            }
        }

        System.out.println("Fisher–Yates shuffle, times of one: " + one);
        System.out.println("Fisher–Yates shuffle, times of two: " + two);
        System.out.println("Fisher–Yates shuffle, times of three: " + three);
        System.out.println("Fisher–Yates shuffle, times of four: " + four);
        System.out.println("Fisher–Yates shuffle, times of five: " + five);
        System.out.println("Fisher–Yates shuffle, times of six: " + six);
        System.out.println("Fisher–Yates shuffle, times of error: " + error);
    }
    
    public static void main(String[] args) {
        MathRandom();   //math.random產生 
        shuffle();      //Fisher–Yates shuffle演算法產生 
        System.out.println(); 
    }
}
