setwd("D:/大數據/作業/報告1")   ##--所有R程式的第一行，用來設定工作目錄／數據目錄等
# X = read.csv("superstore_dataset2011-2015_fixed.csv", encoding = "UTF-8")
X = read.csv("superstore_dataset2011-2015_fixed2.csv", encoding = "UTF-8")
colnames(X)[1] <- c("Row.ID") # 改第一個 
names(X)
dim(X)


# KDD1 數據擷取
head(X,2)


# KDD2數據探索
#日期探索
X$Ship.Date = as.Date(X$Ship.Date,format="%d-%m-%Y")
X$Order.Date = as.Date(X$Order.Date,format="%d-%m-%Y")
range(X$Ship.Date) # 運送到達的日期範圍為: 2011-01-03到2015-01-07
range(X$Order.Date) # 下單的日期範圍為: 2011-01-01到2014-12-31"
#客戶探索
length(unique(X$Customer.ID)) # 客戶人數共有: 1590
library(plyr)
customers = count(X, 'Customer.ID')
range(customers$freq) # 客戶的下單頻次範圍為: 1–97
length(unique(X$Order.ID))
customers[which(customers$freq > 70), 1&2] # 下單紀錄數大於70次的客戶有:
#商品探索
length(unique(X$Product.ID)) # 商品品項共有: 10292
length(unique(X$Sub.Category))# 商品種類共有: 17
count(X, 'Sub.Category') # 商品品類的分布為:



#KDD3 數據轉換
X$date = as.Date(X$Order.Date)
start.time = Sys.time()
setDT(X, key=c("Customer.ID","date"))
Cv = X[, .(D0=min(date), Df=max(date), DD=length(unique(date)),
             FF=length(unique(Order.ID)), MM=sum(Profit), TT=sum(Quantity)), by=Customer.ID ]
Cv
end.time = Sys.time()
duration.all.data.table = end.time - start.time
print("duration.all.data.table is");  print(duration.all.data.table)     #-- 1.7575 mins
dim(Cv)
head(Cv,3)
# Customer.ID         D0         Df    DD FF   MM         TT
# 1:    AA-10315 2011-03-31 2014-12-23 19 19   447.6905   145
# 2:    AA-10375 2011-04-21 2014-12-25 23 23   677.4774   139
# 3:    AA-10480 2011-05-04 2014-08-28 20 20   1516.4752  150
#                首次來店日 最近來店日 頻次    金額       客單件
Cv$UU = Cv$MM / Cv$FF; # 總獲利/總下單數
Cv$NN = Cv$TT / Cv$FF; # 總購買件數/總下單數 
Cv$BB = (Cv$Df-Cv$D0)/Cv$DD # 回購週期 = (最近來店日-首次來店日)/下單日期次數
#range(Cv$DD)   #--     1     40
#range(Cv$FF)   #--     1     41
#range(Cv$MM)   #--     -6437.368  8787.475
#range(Cv$BB)   #--     0.0 599.5
head(Cv,3)
Cv$DD0 = cut( Cv$DD, breaks=c(0,1,10,20,30,40,50,99999));
table(Cv$DD0) # 下單日期次數
# (0,1]         (1,9]        (9,99]      (99,999]   (999,1e+04] (1e+04,1e+05] 
# 10           706           874             0             0             0 
Cv$FF0 = cut( Cv$FF, breaks=c(0,1,10,20,30,40,50,99999));
table(Cv$FF0) # 總下單數
# (0,1]         (1,9]        (9,99]      (99,999]   (999,1e+04] (1e+04,1e+05] 
# 10           705           875             0             0             0 
Cv$MM0 = cut( Cv$MM, breaks=c(-99999, -9999,-999,-99,-9,0,9,99,999,9999,99999));
table(Cv$MM0) # 總獲利
# (-1e+05,0]     (0,1e+04] (1e+04,1e+05] (1e+05,1e+06] (1e+06,1e+07] (1e+07,1e+08] (1e+08,2e+09] 
#   298          1292             0             0             0             0             0 
Cv$BB0 = cut( as.numeric(Cv$BB), breaks=c(-1,30,50,70,90,110,130,150,170,190,210,230,250,300,600)); # 回購週期
table(Cv$BB0)
# (-1,0]     (0,7]    (7,30]  (30,100] (100,999]
# 10         0         2       871       707
Cv$Q0 = paste0(substr(Cv$D0,3,4), cut(as.integer(substr(Cv$D0,6,7)),breaks=c(0,3,6,9,12),labels=c("Q1","Q2","Q3","Q4")))
table(Cv$Q0)
Cv$Qf = paste0(substr(Cv$Df,3,4), cut(as.integer(substr(Cv$Df,6,7)),breaks=c(0,3,6,9,12),labels=c("Q1","Q2","Q3","Q4")))
table(Cv$Qf)




# (4.1) (KDD4) 客戶價值模型 (Cv.FF0,Cv.MM0)
addmargins(table(Cv$FF0,Cv$MM0)) # 元素單位:人數 , 直:總下單數, 橫:總獲利
# 造成虧損的客群比例佔了(37+205+51+5)/1590 = 18.7%, 應設法讓這些客人讓公司獲利(減少退貨狀況, 降低運送成本)

# (4.2) (KDD4) 客戶佇留模型 (Cv.Q0,Cv.Qf)
addmargins(table(Cv$Q0,Cv$Qf)) # 元素單位:人數 , 直:初次下單日期, 橫:最近下單日期
# 主要營收都是靠老客戶支撐, 自從11Q4開始就大幅減少新客戶的加入, 到了13Q1減少更多,甚至到了14幾乎沒有新客戶加入。
# 應想辦法做促銷活動讓新客戶加入, 或者是打廣告增加曝光度。

# (4.3) (KDD4) 客戶流失模型 (Cv.Qf,Cv.BB`)–整體
addmargins(table(Cv$Qf,Cv$BB0)) # 元素單位:人數 , 直:最近下單日期, 橫:回購週期
#大部分的購買週期為30-70，也就是說客戶約1至2個月內會下單一次，老客戶直到現在都還會支持。
