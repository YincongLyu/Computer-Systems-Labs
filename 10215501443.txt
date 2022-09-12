第一题
随机生成50000行的文件
for i in {1..50000}
>do 
>echo $RANDOM | md5sum | cut -c 1-9 >> hw1.txt
>done 

按数字排序 追加写的方式重定向
sort -n hw1.txt -u >> 3.txt

按数字排序 覆盖写的方式重定向
sort -n hw1.txt -u > 4.txt

按字母排序 追加写的方式重定向
sort -f hw1.txt -u >> 5.txt

按字母排序 覆盖写的方式重定向
sort -f hw1.txt -u > 6.txt


第二题
随机生成一个文件
for i in {1..500}
> do
> echo $RANDOM | md5sum | cut -c 1-9 >> hw2.txt
> done

查找指定字符串fe
grep 'fe' hw2.txt

统计字符串fe出现次数
grep -o "fe" hw2.txt | wc -l