# 演算法分析機測
# 學號: 10520104/10520136/10620101
# 姓名: 莊東翰/黃少麒/許藝馨

import os

def Check(TheList) :
    for i in range(len(TheList)) :
        if TheList[i] == False :
            return False
    return True

def Check_Exist(TheList, Num) :
    for i in range(len(TheList)) :
        if TheList[i] == Num :
            return True

    return False

TheNum = []
TempData = []
AllData = []
TheMin = []
temp = ""
The_Now_Min = 0
The_Place1 = 0
The_Place2 = 0
Colect = []
while (1) :
    AllData.clear()  # 將資料初始化
    Colect.clear()
    TheNum = []
    TempFirst = input() #讀入前兩個數字

    for i in range(len(TempFirst)) : #分析前兩個數字
        if TempFirst[i] != ' ' :
            temp = temp + TempFirst[i]
        else :
            TheNum.append(int(temp))
            temp = ""

    TheNum.append(int(temp))

    if TheNum[0] == 0 and TheNum[1] == 0: #若讀到的兩個數字都是0就離開
        break
    temp = ""

    Check_List = [False]*TheNum[0] #製造都是False的List來判定有沒有全部都通知到
    Check_List2 = [False]*TheNum[1]#製造都是False的List來看連線狀態

    for i in range(TheNum[1]) :#整理後面幾行的資料
        tempdata = input()
        for j in range(len(tempdata)) :
            if tempdata[j] !=  ' ':
                temp = temp + tempdata[j]
            else :
                TempData.append(int(temp))
                temp = ""

        TempData.append(int(temp))
        temp = ""
        tempinsert = TempData.copy()
        AllData.append(tempinsert)
        TempData.clear()


    AllData.sort(key= lambda x:x[2]) #將資料用權重排序

    run = 0
    Min = 0
    Temp_Run_List = 0
    while(1) : #計算Minimum Spanning Tree

        if  Check(Check_List) == True:
            break

        if run == 0 :
            Colect.append(AllData[run][0])
            Colect.append(AllData[run][1])
            Check_List[AllData[run][0]-1] = True
            Check_List[AllData[run][1]-1] = True
            Check_List2[run] = True
            Min = Min + AllData[run][2]
        else :
            run_List = 0
            while ( run_List != len(Check_List2)) :
                if Check_List2[run_List] == False :#確認這個路徑沒有走過
                    if Check_Exist(Colect,AllData[run_List][0]) == True or Check_Exist(Colect,AllData[run_List][1]) == True :#這路徑中的數字有沒到過的可以使用這個路徑.
                        if Check_List[AllData[run_List][0]-1] == False or Check_List[AllData[run_List][1]-1] == False :
                            if The_Now_Min == 0 :
                                The_Now_Min = AllData[run_List][2]
                                The_Place1 = AllData[run_List][0]
                                The_Place2 = AllData[run_List][1]
                                Temp_Run_List = run_List
                            elif( The_Now_Min > AllData[run_List][2]) :
                                The_Now_Min = AllData[run_List][2]
                                The_Place1 = AllData[run_List][0]
                                The_Place2 = AllData[run_List][1]
                                Temp_Run_List = run_List

                run_List = run_List + 1

            Check_List2[Temp_Run_List] = True
            if Check_Exist(Colect,The_Place1) == False :
                Colect.append(The_Place1)

            if Check_Exist(Colect,The_Place2) == False :
                Colect.append(The_Place2)

            Check_List[The_Place1-1] = True
            Check_List[The_Place2-1] = True

            Min = Min + The_Now_Min
            The_Now_Min = 0

            # print(The_Place1)
            # print(The_Place2)
            # print(The_Now_Min)
            # print(Min)
            # print(Colect)
            # print(Check_List)
            # print(Check_List2)
            # os.system("pause")

        run = run + 1

    print("這個case算完了")
    TheMin.append(Min)


for i in range(len(TheMin)):
    print("Case " + str(i+1) )
    print("Minimum cost = " + str(TheMin[i]) + "\n")