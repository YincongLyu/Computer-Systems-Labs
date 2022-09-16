#1. 生成 50000 行的文件，每一行可以包含字母、数字。长度不限制。
#* 按照字母顺序排序这个文件，输出所有的 unique 行。
	sort ran_string.txt | uniq
#* 按照数字顺序排序这个文件，输出所有的 unique 行。
	sort -n ran_string.txt | uniq
#* 以覆盖写的方式将上述的结果重定向到一个文件中
	sort ran_string.txt | uniq >res1.txt
#* 以追加写的方式将上述的结果重定向到一个文件中
	sort ran_string.txt | uniq >>res2.txt

#2. 随便生成一个文件，我们想要在这个文件中做以下几件事情：
#* 找一个内容确定的字符串，比如 “computer”
	grep computer ran_.txt
#* 统计其中包含多少 computer
	grep -c computer ran_.txt