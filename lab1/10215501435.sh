#!/bin/bash
#生成50行随机字符串
for i in {1..50}
do
         echo $RANDOM | md5sum | cut -c 1-9 >> random_50.txt
done
#将上述50行随机的字符串循环100次，得到5000行有规律的字符串
times=1
while((times<=100))
do
	head -n 50 random_50.txt >> random_5000.txt
	times=`expr $times + 1`
done
#以覆盖写的方式，字母排序，输出unique行
sort -u  random_5000.txt > alph.txt
#以覆盖写的方式，数字排序，输出unique行
sort -u -n random_5000.txt > number.txt
#以追加写的方式，字母排序，输出unique行
sort -u random_5000.txt >> alph.txt
#以追加写的方式，数字排序，输出unique行
sort -u -n random_5000.txt >> number.txt
#已经生成一个含有60个Computer的letter文件
grep -c "Computer" letter.txt 
