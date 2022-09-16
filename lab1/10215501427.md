# 第一题

## 按字母（ASCII码）排序

<img src="C:\Users\NaOH\AppData\Roaming\Typora\typora-user-images\image-20220910202629446.png" style="zoom: 67%;" />

```linux
s10215501427@JupyterHub:~$ ls
DaSE101_2022  lab1-2.txt  lab_sorted_1  lab_sorted_2  lab.txt  Stat_ML_2022  test.txt  text.txt
s10215501427@JupyterHub:~$ sort lab.txt |uniq -u
```

数字开头的字符串会出现在最前面，因为他们的ascii码更小。



## 按数字排序

<img src="C:\Users\NaOH\AppData\Roaming\Typora\typora-user-images\image-20220910202931523.png" alt="按数字输出unique" style="zoom:67%;" />

```
sort -n lab.txt | uniq -u
```

在数字和字母混合的字符串中，按数值排序没有意义。



## 排序后写入文件

```
s10215501427@JupyterHub:~$ sort lab.txt | uniq -u >> lab_sorted_1
s10215501427@JupyterHub:~$ sort -n lab.txt |uniq -u >> lab_sorted_2
```



# 第二题 统计指定字符的个数

### 方法1

```
s10215501427@JupyterHub:~$ grep -c "hello" lab1-2.txt
```

![](C:\Users\NaOH\AppData\Roaming\Typora\typora-user-images\image-20220910203546966.png)

### 方法2

```
s10215501427@JupyterHub:~$   grep -o "hello" lab1-2.txt | wc -l
```

![](C:\Users\NaOH\AppData\Roaming\Typora\typora-user-images\image-20220910205021916.png)

