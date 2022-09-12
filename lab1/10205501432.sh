sort random.txt | uniq #按照字母顺序对文件中的字符串进行排序，并输出unique行
sort -n random.txt | uniq #按照数字顺序对文件中的字符串进行排序，并输出unique行
cat random.txt>ecnu #以覆盖写的方式将random中的内容重定向到ecnu文件中
cat random.txt>>unce #以追加写的方式将random中的内容重定向到ecnu文件中
grep 'da' random.txt#在文件中查找特定的字符串（"da"）
grep -0 'da' <test.txt|wc -l#统计文件中特定字符串("da")的个数
