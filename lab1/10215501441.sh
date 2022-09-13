#!/bin/bash
for i in `seq 10`;do echo $RANDOM | md5sum | cut -c 1-9 ;done >> 1.txt
#生成10个随机数

sort -u 1.txt
#按字母顺序排序，输出unique行

echo -e "\n"
echo -e "\n"
echo -e "\n"

sort -n -u 1.txt
#按数字顺序排序，输出unique行

sort -u 1.txt > result_letter1.txt
#按字母顺序排序,以覆盖写的方式重定向

sort -u 1.txt >> result_letter2.txt
#按字母顺序排序,以追加写的方式重定向

sort -n -u 1.txt > result_number1.txt
#按数字顺序排序,以覆盖写的方式重定向

sort -n -u 1.txt >> result_number2.txt
#按数字顺序排序,以追加写的方式重定向



#之前已经生成了一个2.txt文件
grep -o "Demosthenes" 2.txt | wc -l
#查找并统计文件中单词Demosthenes的出现次数