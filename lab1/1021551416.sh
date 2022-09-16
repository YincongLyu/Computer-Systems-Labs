#1.先生成含有5000行随机代码的文件
for i in `seq 50000`;do echo $RANDOM | md5sum | cut -c 1-9 ;done >> test.txt；
#2 对文件进行具体的操作
sort -u test.txt;
#按字母给文件排序
sort -u -n test.txt;
#按数字给文件排序
sort -u test.txt>test1.txt;
#将字母排序的结果覆盖至test1文件中
sort -u -n test.txt>test2.txt;
#将数字排序的结果覆盖至test2文件中
grep "computer" test.txt;
#找内容含有”computer"字符串的句子并输出
grep -c "computer" test.txt;
#统计行数并输出