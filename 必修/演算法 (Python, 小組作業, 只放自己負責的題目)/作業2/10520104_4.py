# 演算法分析機測
# 學號: 10520104/10520136/10620101
# 姓名: 莊東翰/黃少麒/許藝馨

matrix = []
visited_path = []

def AddVertices(v1, v2):
    if len(matrix) == 0:
        list1 = [v1, 1, [v2]]   # vertex, degree, edge
        list2 = [v2, 1, [v1]]
        matrix.insert(len(matrix), list1)
        matrix.insert(len(matrix), list2)
    else:
        found1 = 0
        found2 = 0
        for i in range(len(matrix)):
            if matrix[i][0] == v1:
                matrix[i][1] += 1
                matrix[i][2].insert(len(matrix[i][2]), v2)
                matrix[i][2].sort()
                found1 = 1
            if matrix[i][0] == v2:
                matrix[i][1] += 1
                matrix[i][2].insert(len(matrix[i][2]), v1)
                found2 = 1
                matrix[i][2].sort()

            if i == len(matrix) - 1:
                if found1 == 0:
                    list1 = [v1, 1, [v2]]  # vertex, num of edges, edge
                    matrix.insert(len(matrix), list1)
                if found2 == 0:
                    list2 = [v2, 1, [v1]]
                    matrix.insert(len(matrix), list2)
    matrix.sort()

def CheckDegree():    # 確定每個節點的degree是否為even
    result = 1
    for i in range(len(matrix)):
        if matrix[i][1] % 2 != 0:
            result = 0
            break
    return result

def MaxDegree():
    degree = -1
    for i in range(len(matrix)):
        if matrix[i][1] > degree:     # 取最高degree且數字最小
            degree = matrix[i][1]
    return degree

def FindHighestDegree():    # 第一次
    max = -1
    index = -1
    for i in range(len(matrix)-1, -1, -1):
        if matrix[i][1] >= max:     # 取最高degree且數字最小
            max = matrix[i][1]
            index = i

    return index

def FindNextPath(array):    # 每一回合尋找下個路徑
    max = -1
    index = -1  # 取最高degree且數字最小
    for i in range(len(array)-1, -1, -1):
        for j in range(len(matrix) - 1, -1, -1):
            if matrix[j][0] == array[i]:
                if matrix[j][1] >= max:  # 取最高degree且數字最小
                    max = matrix[j][1]
                    index = i
    return index

def Visit(v1, v2):  # 走訪 刪除走過的邊
    visited_path.insert(len(visited_path), v2)
    for i in range(len(matrix)):
        if matrix[i][0] == v1:
            j = 0
            while j < len(matrix[i][2]):    # 在v1的array中刪除v2
                if matrix[i][2][j] == v2:
                    matrix[i][1] -= 1
                    matrix[i][2].pop(j)
                    j = len(matrix[i][2])   # break
                j += 1
        if matrix[i][0] == v2:
            j = 0
            while j < len(matrix[i][2]):    # 在v2的array中刪除v1
                if matrix[i][2][j] == v1:
                    matrix[i][1] -= 1
                    matrix[i][2].pop(j)
                    j = len(matrix[i][2])   # break
                j += 1

def GoToNextVertex(v2):  # 此次終止點為下次起始點
    index = -1
    for i in range(len(matrix)):
        if matrix[i][0] == v2:
            index = i
            break

    return index

def EulerTour():    # 開始走訪
    start = FindHighestDegree()
    index1 = start
    # print("start: ", matrix[index1][0])
    visited_path.insert(len(visited_path), matrix[index1][0])

    while MaxDegree() != 0:     # 當還有任意點的degree > 0
        index2 = FindNextPath(matrix[index1][2])    # 優先走degree最高節點的邊
        vertex2 = matrix[index1][2][index2]
        Visit(matrix[index1][0], matrix[index1][2][index2])     # 走訪 刪除走過的邊
        index1 = GoToNextVertex(vertex2)    # 此次終止點為下次起始點

def PrintPath():
    print("< ", end='')
    for i in range(len(visited_path)):
        if i != len(visited_path) - 1:
            print(visited_path[i], end=', ')
        else:
            print(visited_path[i], end=' >\n')


def main():
    rnd = 1
    print("Program Starts...")
    print("input:")
    while(1):
        matrix.clear()
        num_of_vertices = 0  # input 節點數
        num_of_edges = 0  # input 邊數
        vertex1 = 0
        vertex2 = 0
        error = 0
        (num_of_vertices, num_of_edges) = map(int, input().split())

        if num_of_vertices == 0 and num_of_edges == 0:  # end of input
            break
        else:
            # print("num of vertices: ", num_of_vertices)
            # print("num of edges: ", num_of_edges)
            for i in range(num_of_edges):
                (vertex1, vertex2) = map(int, input().split())
                if((1 > vertex1 or vertex1 > num_of_vertices) or (1 > vertex2 and vertex2 > num_of_vertices)):
                    print("input的節點超過範圍")
                    error = 1
                    break
                else:
                    if vertex1 != vertex2:  # 防止自己連自己
                        AddVertices(vertex1, vertex2)   # 把此兩點(邊)加入

            if error != 1:
                result = CheckDegree()      # 確定每個節點的degree是否為even
                if result == 0:
                    print("Case ", rnd)
                    print("No Euler Tours")     # 此圖不為Euler Tour
                else:
                    EulerTour()
                    print("Case ", rnd)
                    PrintPath()             # 印出結果
                    matrix.clear()          # 重製
                    visited_path.clear()    # 重製
            rnd += 1

main()
