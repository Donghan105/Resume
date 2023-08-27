public class Start
{
    public static void main(String[] args) {
        int draw = 0, tomwins = 0, marywins = 0;    //紀錄次數 
        String result; // 兩人比大小的結果 
        Player tom = new Player();
        Player mary = new Player();
        Game game = new Game();

        
        for (int i = 0; i < 10; i++){   //比10回合 
          System.out.println();

          tom.play();
          System.out.println("Sum of Tom's two 6-sides dices: " + tom.sum );

          mary.play();
          System.out.println("Sum of Mary's two 6-sides dices: " + mary.sum );

          result = game.Versus(tom, mary);

          if (result.equals("draw")){
              draw++;
          }
          else if (result.equals("Tom wins")){
              tomwins++;
          }
          else{
              marywins++;
          }

          System.out.println();
        }
        
        System.out.println("**************Total results**************");
        System.out.println("Tom wins = " + tomwins);
        System.out.println("Mary wins = " + marywins);
        System.out.println("draw = " + draw);
    }
}