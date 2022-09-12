#!/bin/bash

for i in `seq 1000`;do echo $RANDOM | md5sum | cut -c 1-9 ;done > test.txt

sort -u test.txt
#按字母排序，并输出unique行

echo -e "\n"
#美观的空行

sort -n -u test.txt
#按数字排序，并输出unique行

sort -u test.txt > result1.txt
#以覆盖写的方式重定向

sort -u test.txt >> result2.txt
#以追加写的方式重定向

sort -n -u test.txt > resultn1.txt
#以覆盖写的方式重定向按数字排序

sort -n -u test.txt >> resultn2.txt
#以追加写的方式重定向按数字排序


#在这里已经手动创建了一个article.txt，内含一段文字

grep -wo "the" article.txt |wc -l
#查找the并输出总数