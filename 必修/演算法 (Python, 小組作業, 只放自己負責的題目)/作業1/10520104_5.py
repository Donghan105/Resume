# 演算法分析機測
# 學號: 10520104/10520136/10620101
# 姓名: 莊東翰/黃少麒/許藝馨

total_costs = []
total_costs = list(map(int, total_costs))   # assign as an int list, 記錄所有path與cost

def Find_smallest_cost():
    min_cost = 1000
    min_path = []
    for i in range(len(total_costs)):
        if(total_costs[i][1] < min_cost):
            min_cost = total_costs[i][1]
            min_path = total_costs[i][0]

    print("\n", end='')
    print("Minimum Sum of Costs = ", min_cost)

    ###debugging###
    # print("Path: ", min_path)

def Copy_puzzle(copy_list, puzzle):   # 複製puzzle (遞迴時 '=', '.copy' 有reference複製問題)
    for i in range(len(puzzle)):
        x = []
        for j in range(len(puzzle[i])):
            x.insert(len(x), puzzle[i][j])
        copy_list.insert(len(copy_list), x)

    return copy_list

def Check_0_to_8(puzzle):   # 不重複的0到8數字
    detect = [0, 1, 2, 3, 4, 5, 6, 7, 8]
    for i in range(len(puzzle)):
        for j in range(len(puzzle[i])):
            k = 0
            while(k<len(detect)):
                if (puzzle[i][j] == detect[k]): # 若找到相同元素
                    detect.pop(k)   # 刪除
                    k = len(detect)  # 跳出while
                k+=1

    if(len(detect)==0):  # 正確, list中的值皆找到
        return True
    else:
        return False

def Swap(puzzle, ai,aj,bi,bj):  # 交換puzzle中兩元素
    temp1 = puzzle[ai][aj]
    puzzle[ai][aj] = puzzle[bi][bj]
    puzzle[bi][bj] = temp1
    return puzzle

def Get_zero_moving_steps(start_i, start_j, end_i, end_j):  # 取得移動0所需步數 (ex: 1要往上移 => 0移至1上方 => 0向下1次向左2次)
    up = 0
    right = 0
    down = 0
    left = 0

    if(start_i==end_i or start_j==end_j): #同直行 或 橫列 需繞遠路 C字形
        if(start_i==end_i): #同橫列
            start_j -= end_j
            if (start_j < 0):
                right = -(start_j)
            else:
                left = start_j
            up = 1
            down = 1

        if(start_j==end_j):  #同直行
            start_i -= end_i
            if (start_i < 0):
                down = -(start_i)
            else:
                up = start_i
            left = 1
            right = 1

    else:
        start_i -= end_i
        start_j -= end_j
        if(start_i < 0):
            down = -(start_i)
        else:
            up = start_i

        if(start_j < 0):
            right = -(start_j)
        else:
            left = start_j

    return(up, right, down, left)


def Start_zero(puzzle, zero_i, zero_j, one_i, one_j, u0_times, r0_times, d0_times, l0_times, cost, u1_times, l1_times, path, last_step):    # 移動0    # 開始步驟3

    ###debugging###
    # for i in range(len(puzzle)):            # PRINT
    #     for j in range(len(puzzle[i])):
    #         if (j == len(puzzle[i]) - 1):
    #             print(puzzle[i][j], end='\n')
    #         else:
    #             print(puzzle[i][j], end=' ')
    # print("\n", end='')

    if(u1_times==0 and l1_times==0 and zero_i==0 and zero_j==0):   # 遞迴終點 插入每次所有cost到list中
         path = path + " 1"
         cost += 1
         total_costs.insert(len(total_costs), [path, cost])

         ###debugging###
         # print([path, cost])
         # print("////////////////////////////END/////////////////////////////////")  # 0到左上角 終點

    elif(u0_times==0 and r0_times==0 and d0_times==0 and l0_times==0):    #每一回合最後步驟(與1交換)
        path = path + " 1"
        cost += 1
        puzzle = Swap(puzzle, zero_i, zero_j, one_i, one_j)     # 0與1交換
        tempi = zero_i  # 交換i j
        tempj = zero_j
        zero_i = one_i
        zero_j = one_j
        one_i = tempi
        one_j = tempj

        Start_one(puzzle, u1_times, l1_times, zero_i, zero_j, one_i, one_j, cost, path)     # 繼續下一回合遞迴

    else:   # 0每次與其他元素交換
        puzzle1 = []
        puzzle2 = []
        puzzle3 = []
        puzzle4 = []

        puzzle1 = Copy_puzzle(puzzle1, puzzle)  # 往上後 遞迴還原puzzle (退至前一步)
        puzzle2 = Copy_puzzle(puzzle2, puzzle)  # 往下後 遞迴還原puzzle (退至前一步)
        puzzle3 = Copy_puzzle(puzzle3, puzzle)  # 往右後 遞迴還原puzzle (退至前一步)
        puzzle4 = Copy_puzzle(puzzle4, puzzle)  # 往左後 遞迴還原puzzle (退至前一步)

        temp_path = path    # 遞迴還原path (退至前一步)
        temp_cost = cost    # 遞迴還原cost (退至前一步)

        if (u0_times > 0 and last_step!="D" and zero_i!= 0 and puzzle1[zero_i-1][zero_j] != 1):  # 0往上 # 持續遞迴
            path = path + " " + str(puzzle1[zero_i-1][zero_j])
            cost += puzzle1[zero_i-1][zero_j]
            puzzle1 = Swap(puzzle1, zero_i, zero_j, zero_i-1, zero_j)   # 元素交換
            Start_zero(puzzle1, zero_i-1, zero_j, one_i, one_j, u0_times-1, r0_times, d0_times, l0_times, cost, u1_times, l1_times, path, "U")  # 持續遞迴
            path = temp_path    # 退至前一步 還原
            cost = temp_cost    # 退至前一步 還原
        if (d0_times > 0 and last_step!="U" and zero_i != 2 and puzzle2[zero_i+1][zero_j] != 1):  # 0往下
            path = path + " " + str(puzzle2[zero_i+1][zero_j])
            cost += puzzle2[zero_i+1][zero_j]
            puzzle2 = Swap(puzzle2, zero_i, zero_j, zero_i+1, zero_j)   # 元素交換
            Start_zero(puzzle2, zero_i+1, zero_j, one_i, one_j, u0_times, r0_times, d0_times-1, l0_times, cost, u1_times, l1_times, path, "D")  # 持續遞迴
            path = temp_path    # 退至前一步 還原
            cost = temp_cost    # 退至前一步 還原
        if (l0_times > 0 and last_step!="R" and zero_j != 0 and puzzle3[zero_i][zero_j-1] != 1):  # 0往左
            path = path + " " + str(puzzle3[zero_i][zero_j-1])
            cost += puzzle3[zero_i][zero_j-1]
            puzzle3 = Swap(puzzle3, zero_i, zero_j, zero_i, zero_j-1)   # 元素交換
            Start_zero(puzzle3, zero_i, zero_j-1, one_i, one_j, u0_times, r0_times, d0_times, l0_times-1, cost, u1_times, l1_times, path, "L")  # 持續遞迴
            path = temp_path    # 退至前一步 還原
            cost = temp_cost    # 退至前一步 還原
        if (r0_times > 0 and last_step!="L" and zero_j != 2 and puzzle4[zero_i][zero_j+1] != 1):  # 0往右
            path = path + " " + str(puzzle4[zero_i][zero_j+1])
            cost += puzzle4[zero_i][zero_j+1]
            puzzle4 = Swap(puzzle4, zero_i, zero_j, zero_i, zero_j+1)   # 元素交換
            Start_zero(puzzle4, zero_i, zero_j+1, one_i, one_j, u0_times, r0_times-1, d0_times, l0_times, cost, u1_times, l1_times, path, "R")  # 持續遞迴
            path = temp_path    # 退至前一步 還原
            cost = temp_cost    # 退至前一步 還原


def Start_one(puzzle, u1_times, l1_times, zero_i, zero_j, one_i, one_j, cost, path): # 開始步驟2 先決定1要往哪(上or左)
    temp_puzzle = []
    temp_puzzle = Copy_puzzle(temp_puzzle, puzzle)

    if(u1_times>0): #試著讓1往上
        temp_puzzle = puzzle.copy()

        (u0_times, r0_times, d0_times, l0_times) = Get_zero_moving_steps(zero_i, zero_j, one_i-1, one_j)    # 取得0移動次數
        Start_zero(puzzle, zero_i, zero_j, one_i, one_j, u0_times, r0_times, d0_times, l0_times, cost, u1_times-1, l1_times, path, " ")  # 開始步驟3 移動0 清出空間


    if(l1_times>0): #試著讓1往左
        (u0_times, r0_times, d0_times, l0_times) = Get_zero_moving_steps(zero_i, zero_j, one_i, one_j-1)    # 取得0移動次數
        Start_zero(temp_puzzle, zero_i, zero_j, one_i, one_j, u0_times, r0_times, d0_times, l0_times, cost, u1_times, l1_times-1, path, " ")  # 開始步驟3 移動0 清出空間



def Start(puzzle):  # 開始步驟1
    u1_times = 2    # 1能往上走2次
    l1_times = 2    # 1能往左走2次

    zero_i = 0
    zero_j = 0
    one_i = 2
    one_j = 2
    cost = 0
    path = ""
    Start_one(puzzle, u1_times, l1_times, zero_i, zero_j, one_i, one_j, cost, path)  # 開始步驟2 試著移動1

def main():
    puzzle = []
    flag = 0

    try:
        row1 = list(map(int, input("1st row: ").split()))   # whitespace切割
        row2 = list(map(int, input("2nd row: ").split()))
        row3 = list(map(int, input("3rd row: ").split()))
        flag = 1
    except:
        print("error, input不全為數字")

    if(flag==1):     # input 皆為數字
        puzzle = [row1, row2, row3]
        if (len(row1) != 3 or len(row2) != 3 or len(row3) != 3):
            print("error, input不為3x3")
        elif(puzzle[0][0] != 0 or puzzle[2][2] != 1):
            print("error, 0不在左上角 or 1不在右下角")
        else:
            if(Check_0_to_8(puzzle)==True):  # 不重複的0到8數字
                Start(puzzle)   # 開始遞迴

                ###debugging###
                # for i in range(len(total_costs)):
                #     print(total_costs[i])

                Find_smallest_cost()
            else:
                print("error, 需不重複的0到8數字")


main()
