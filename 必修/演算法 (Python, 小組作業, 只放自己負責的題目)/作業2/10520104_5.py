# 演算法分析機測
# 學號: 10520104/10520136/10620101
# 姓名: 莊東翰/黃少麒/許藝馨

maze = []
max_count = 0

def PrintMaze():
    for i in range(len(maze)):
        for j in range(len(maze[i])):
            if j == len(maze[i]) - 1:
                print(maze[i][j])
            else:
                print(maze[i][j], end='')
    print("\n", end='')

def FindStart():    # 尋找'P' 回傳P所在位置
    index_i = 0
    index_j = 0
    for i in range(len(maze)):
        for j in range(len(maze[i])):
            if maze[i][j] == 'P':
                index_i = i
                index_j = j
                break
    return (index_i, index_j)

def FindTrap(i, j):     # 檢查此點上下左右是否有trap
    foundtrap = False
    if i > 0:                   # 可往上
        # print("maze[i - 1][j]: ", maze[i - 1][j])
        if maze[i - 1][j] == 'T':
            foundtrap = True

    if i < len(maze) - 1:       # 可往下
        # print("maze[i + 1][j]: ", maze[i + 1][j])
        if maze[i + 1][j] == 'T':
            foundtrap = True

    if j > 0:                  # 可往左
        # print("maze[i][j - 1]: ", maze[i][j - 1])
        if maze[i][j - 1] == 'T':
            foundtrap = True

    if j < len(maze[i]) - 1:  # 可往右
        # print("maze[i][j + 1]: ", maze[i][j + 1])
        if maze[i][j + 1] == 'T':
            foundtrap = True

    return foundtrap

def Visit(i, j, count, last_move):  # 開始走訪
    global max_count
    # print("i: ", i, " j: ", j)

    if maze[i][j] == 'G':
        max_count += 1

    maze[i][j] = 'X'    # 此次走訪
    # PrintMaze()

    if FindTrap(i, j) is False:     # 檢查此點上下左右是否有trap
        if last_move != 'L' and j < len(maze[i]) - 1 and maze[i][j + 1] != 'T' and maze[i][j + 1] != '#' and maze[i][j + 1] != 'X':  # 可往右
            # PrintMaze()
            Visit(i, j + 1, count, 'R')     # 遞迴
            maze[i][j + 1] = '.'  # 還原

        if last_move != 'U' and i < len(maze) - 1 and maze[i + 1][j] != 'T' and maze[i + 1][j] != '#' and maze[i + 1][j] != 'X':  # 可往下
            # PrintMaze()
            Visit(i + 1, j, count, 'D')     # 遞迴
            maze[i + 1][j] = '.'  # 還原

        if last_move != 'R' and j > 0 and maze[i][j - 1] != 'T' and maze[i][j - 1] != '#' and maze[i][j - 1] != 'X':  # 可往左
            # PrintMaze()
            Visit(i, j - 1, count, 'L')     # 遞迴
            maze[i][j - 1] = '.'  # 還原

        if last_move != 'D' and i > 0 and maze[i - 1][j] != 'T' and maze[i - 1][j] != '#' and maze[i - 1][j] != 'X':   # 可往上
            # PrintMaze()
            Visit(i - 1, j, count, 'U')     # 遞迴
            maze[i - 1][j] = '.'    # 還原

    maze[i][j] = '.'    # 還原


def main():
    global max_count, maze
    print("Program Starts...")
    print("input:")
    while(1):
        maze.clear()
        rows = 0  # input 節點數
        columns = 0  # input 邊數

        (rows, columns) = map(int, input().split())

        if rows == 0 and columns == 0:  # end of input
            break
        else:
            for i in range(columns):
                line = input().split()  # whitespace切割
                for j in range(len(line)):
                    arr = []
                    for k in range(len(line[j])):
                        arr.insert(len(arr), line[j][k])
                    maze.insert(len(maze), arr)
            i, j = FindStart()      # 尋找'P'
            # PrintMaze()
            Visit(i, j, 0, "NULL")  # 開始走訪

        print(max_count)
        max_count = 0
        maze.clear()


main()
