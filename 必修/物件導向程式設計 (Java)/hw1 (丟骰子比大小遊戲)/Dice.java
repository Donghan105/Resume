abstract class Dice
{
    public int faces;

    abstract void setFaces(); // 根據骰子的面數不同做設定 

    public int throwDice()     //骰骰子
    {
        int points = (int)(Math.random()*faces) + 1;
        return points;
    }
}