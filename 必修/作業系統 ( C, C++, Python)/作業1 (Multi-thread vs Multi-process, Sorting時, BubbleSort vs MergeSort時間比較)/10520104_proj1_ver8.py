# 電資四 10520104 莊東翰
import os
import datetime
import time
import threading
from multiprocessing import Pool  # pool, apply_async, pool_close, join, result[i].get
import multiprocessing
import queue

k_pieces = 0
k_processes = 0
k_threads = 0

def BubbleSort(array, start, end): # 每一回合決定好最大值該在的位置
  count = 0
  while count < end - start + 1:
    i = start
    while i + 1 <= end:
      if array[i] > array[i+1]:  # swap
        temp = array[i]
        array[i] = array[i+1]
        array[i+1] = temp
      i += 1
    # print("\n", end='')
    count += 1

def BubbleSort_Process(array): # 每一回合決定好最大值該在的位置
  count = 0
  while count < len(array):
    i = 0
    while i + 1 < len(array):
      if array[i] > array[i+1]:  # swap
        temp = array[i]
        array[i] = array[i+1]
        array[i+1] = temp
      i += 1
    # print("\n", end='')
    count += 1
  return array


def Merge(arr1, arr2):  # 合併兩個array, 輸出合併後的array
  arr = []
  while len(arr1) != 0 or len(arr2) != 0:
    if len(arr1) == 0:
      arr.insert(len(arr), arr2[0])
      arr2.pop(0)
    elif len(arr2) == 0:
      arr.insert(len(arr), arr1[0])
      arr1.pop(0)
    else:  # len(arr1) != 0 and len(arr2) != 0
      if arr1[0] <= arr2[0]:
        arr.insert(len(arr), arr1[0])
        arr1.pop(0)
      else:
        arr.insert(len(arr), arr2[0])
        arr2.pop(0)

  return arr


def Start_Process(piece_size):
  in_file = dir + '\\' + file_name + ".txt"
  with open(in_file) as f:
    arr = f.read().split()
    arr = list(map(int, arr))  # 轉為int list
  ###############processes#######################
  timer_start = time.time()
  jobs = []
  pool = Pool()
  # bubblesort
  start = 0
  end = piece_size
  while end <= len(arr):  # k份先做bubblesort
    sub_arr = arr[:]
    sub_arr = sub_arr[start:end]
    jobs.append(pool.apply_async(BubbleSort_Process, (sub_arr,)))
    start += piece_size
    end += piece_size

  if len(arr) - (end - piece_size) > 0:  # 最後多出來不到n/k的那份
    sub_arr = arr[:]
    sub_arr = sub_arr[end:]
    jobs.append(pool.apply_async(BubbleSort_Process, (sub_arr,)))

  pool.close()
  pool.join()


  proc = 0
  start = 0
  end = piece_size
  while end <= len(arr):  # k份bubblesort get結果
    # print("jobs[proc].get(): ", jobs[proc].get())
    # arr[start:end] = jobs[proc].get()
    # print("start: ", start, " end: ", end, " len(array): ", len(arr))
    if end == len(arr):
      arr = arr[:start] + jobs[proc].get()
    else:
      arr = arr[:start] + jobs[proc].get() + arr[end:]

    start += piece_size
    end += piece_size
    proc += 1

  if len(arr) - (end - piece_size) > 0:  # 最後多出來不到n/k的那份 的結果
    arr = arr[:start] + jobs[proc].get()

  # merge
  n_round = 1  # 第幾回合
  while piece_size * pow(2, n_round - 1) < len(arr):
    jobs = []
    pool = Pool()
    start = 0
    end = piece_size * pow(2, n_round - 1)
    while end + piece_size * pow(2, n_round - 1) <= len(arr):
      sub_arr1 = arr[start:end]
      start += piece_size * pow(2, n_round - 1)
      end += piece_size * pow(2, n_round - 1)

      sub_arr2 = arr[start:end]
      start += piece_size * pow(2, n_round - 1)
      end += piece_size * pow(2, n_round - 1)

      jobs.append(pool.apply_async(Merge, (sub_arr1, sub_arr2,)))

    if len(arr) - end > 0:  # 最後多出來有2份可合併, 但不到2*piece_size
      sub_arr1 = arr[start:end]
      sub_arr2 = arr[end:]
      jobs.append(pool.apply_async(Merge, (sub_arr1, sub_arr2,)))
    #  else: 最後多出來不到2份可合併 啥都不用做

    pool.close()
    pool.join()

    proc = 0
    start = 0
    end = piece_size * pow(2, n_round)
    while proc < len(jobs):  # k份先做bubblesort
      if end <= len(arr):
        if end == len(arr):
          arr = arr[:start] + jobs[proc].get()
        else:
          arr = arr[:start] + jobs[proc].get() + arr[end:]
        start += piece_size * pow(2, n_round)
        end += piece_size * pow(2, n_round)
      elif len(arr) - (end - piece_size * pow(2, n_round - 1)) > 0:  # 最後多出來有2份可合併, 但不到2*piece_size
        arr = arr[:start] + jobs[proc].get()
      proc += 1

    # print("threads: ", threads)
    # print("arr: ", arr)
    # print("\n", end='')
    n_round += 1

  timer_end = time.time()
  arr = list(map(str, arr))  # 轉回str list
  with open(dir + '\\' + file_name + '_output3.txt', "w") as output:  # mode='wt', encoding='utf-8'
    for i in range(len(arr)):
      output.write(arr[i] + "\n")
      # output.write('\n')
    output.write("CPU Time : " + str(timer_end - timer_start) + "\n")
    output.write("Output Time : " + str(datetime.datetime.now()) + "+08:00")
  #############################################



if __name__ == '__main__':
  # Load the input
  dir = os.getcwd()
  file_name = input('請輸入檔案名稱:')
  k_pieces = int(input('請輸入切成幾等分:'))
  in_file = dir + '\\' + file_name + ".txt"
  with open(in_file) as f:
    arr = f.read().split()
    arr = list(map(int, arr))  # 轉為int list

  if len(arr) % k_pieces == 0.0:
    piece_size = len(arr) / k_pieces
  else:
    piece_size = (len(arr) / (k_pieces)) + 1
  piece_size = int(piece_size)



  ############### 單純bubblesort#######################
  print("starting method 1")
  timer_start = time.time()
  arr = BubbleSort_Process(arr)
  timer_end = time.time()
  arr = list(map(str, arr))  # 轉回str list
  with open(dir + '\\' + file_name + '_output1.txt', "w") as output:  # mode='wt', encoding='utf-8'
    for i in range(len(arr)):
      output.write(arr[i] + "\n")
    output.write("CPU Time : " + str(timer_end - timer_start) + "\n")
    output.write("Output Time : " + str(datetime.datetime.now()) + "+08:00")
  #####################################################



  in_file = dir + '\\' + file_name + ".txt"
  with open(in_file) as f:
    arr = f.read().split()
    arr = list(map(int, arr))  # 轉為int list
  print("starting method 2")
  threads = []
  # bubblesort
  timer_start = time.time()
  start = 0
  end = piece_size
  while end <= len(arr):  # k份先做bubblesort
    thread = threading.Thread(target=BubbleSort, args=(arr, start, end - 1,))
    thread.start()
    threads.append(thread)
    start += piece_size
    end += piece_size

  if len(arr) - (end - piece_size) > 0:  # 最後多出來不到n/k的那份
    thread = threading.Thread(target=BubbleSort, args=(arr, start, len(arr) - 1,))
    # (target=lambda q, arg1: q.put(BubbleSort(arg1)), args=(que, arr[start:]))
    thread.start()
    threads.append(thread)

  for thread in threads:
    thread.join()

  # merge
  que = queue.Queue()
  n_round = 1  # 第幾回合
  while piece_size * pow(2, n_round - 1) < len(arr):
    start = 0
    end = piece_size * pow(2, n_round - 1)
    while end + piece_size * pow(2, n_round - 1) <= len(arr):
      sub_arr1 = arr[start:end]
      start += piece_size * pow(2, n_round - 1)
      end += piece_size * pow(2, n_round - 1)

      sub_arr2 = arr[start:end]
      start += piece_size * pow(2, n_round - 1)
      end += piece_size * pow(2, n_round - 1)

      thread = threading.Thread(target=lambda q, arg1, arg2: q.put(Merge(arg1, arg2)), args=(que, sub_arr1, sub_arr2))
      thread.start()
      threads.append(thread)
      # arr[merge_start:merge_end] = Merge(sub_arr1, sub_arr2)

    if len(arr) - end > 0:  # 最後多出來有2份可合併, 但不到2*piece_size
      sub_arr1 = arr[start:end]
      sub_arr2 = arr[end:]
      thread = threading.Thread(target=lambda q, arg1, arg2: q.put(Merge(arg1, arg2)), args=(que, sub_arr1, sub_arr2))
      thread.start()
      threads.append(thread)
      # arr[start:] = Merge(sub_arr1, sub_arr2)
    #  else: 最後多出來不到2份可合併 啥都不用做

    for thread in threads:
      thread.join()
    start = 0
    end = piece_size * pow(2, n_round)
    while not que.empty():
      if end <= len(arr):
        arr[start:end] = que.get()
        start += piece_size * pow(2, n_round)
        end += piece_size * pow(2, n_round)
      elif len(arr) - (end - piece_size * pow(2, n_round - 1)) > 0:  # 最後多出來有2份可合併, 但不到2*piece_size
        arr[start:] = que.get()
    n_round += 1

  timer_end = time.time()
  arr = list(map(str, arr))  # 轉回str list
  with open(dir + '\\' + file_name + '_output2.txt', "w") as output:  # mode='wt', encoding='utf-8'
    for i in range(len(arr)):
      output.write(arr[i] + "\n")
      # output.write('\n')
    output.write("CPU Time : " + str(timer_end - timer_start) + "\n")
    output.write("Output Time : " + str(datetime.datetime.now()) + "+08:00")
  #############################################




  #######process #######
  print("starting method 3")
  Start_Process(piece_size)
  ######################




  ####### 1-process #######
  in_file = dir + '\\' + file_name + ".txt"
  with open(in_file) as f:
    arr = f.read().split()

  arr = list(map(int, arr))  # 轉為int list
  # bubblesort
  print("starting method 4")
  timer_start = time.time()
  start = 0
  end = piece_size
  while end <= len(arr):  # k份先做bubblesort
    sub_arr = arr[start:end]
    arr[start:end] = BubbleSort_Process(sub_arr)
    start += piece_size
    end += piece_size

  if len(arr) - (end - piece_size) > 0:  # 最後多出來不到n/k的那份
    sub_arr = arr[start:]
    arr[start:] = BubbleSort_Process(sub_arr)

  # merge
  n_round = 1  # 第幾回合
  while piece_size * pow(2, n_round - 1) < len(arr):
    start = 0
    end = piece_size * pow(2, n_round - 1)
    while end + piece_size * pow(2, n_round - 1) <= len(arr):
      merge_start = start
      sub_arr1 = arr[start:end]
      start += piece_size * pow(2, n_round - 1)
      end += piece_size * pow(2, n_round - 1)
      merge_end = end

      sub_arr2 = arr[start:end]
      start += piece_size * pow(2, n_round - 1)
      end += piece_size * pow(2, n_round - 1)

      arr[merge_start:merge_end] = Merge(sub_arr1, sub_arr2)

    if len(arr) - end > 0:  # 最後多出來有2份可合併, 但不到2*piece_size
      sub_arr1 = arr[start:end]
      sub_arr2 = arr[end:]
      arr[start:] = Merge(sub_arr1, sub_arr2)
    #  else: 最後多出來不到2份可合併 啥都不用做
    n_round += 1

  timer_end = time.time()
  arr = list(map(str, arr))  # 轉回str list
  with open(dir + '\\' + file_name + '_output4.txt', "w") as output:  # mode='wt', encoding='utf-8'
    for i in range(len(arr)):
      output.write(arr[i] + "\n")
      # output.write('\n')
    output.write("CPU Time : " + str(timer_end - timer_start) + "\n")
    output.write("Output Time : " + str(datetime.datetime.now()) + "+08:00")
  ########################
