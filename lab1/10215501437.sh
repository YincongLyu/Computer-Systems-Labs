#!/bash/bin
#生成随机字符串
for i in `seq 1000`;do echo $RANDOM | md5sum | cut -c 1-9 ;done > test.txt
#按照字母顺序排列，输出unique行
sort -u test.txt | uniq
#按照数字顺序排列，输出unique行
sort -u -n test.txt | uniq
#以覆盖写的方式重定向到一个文件
sort -u test.txt | uniq > ans1
sort -u -n test.txt | uniq > ans1
#以追加写的方式重定向到一个文件
sort -u test.txt | uniq >> ans1
sort -u -n test.txt | uniq >> ans1
#寻找to
grep -wo "to" test2.txt | wc-l
