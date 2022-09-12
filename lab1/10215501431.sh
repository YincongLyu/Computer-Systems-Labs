for i in `seq 1000 `; do echo $RANDOM | md5sum | cut -c 1-9;done >> test.txt
sort -d -u test.txt # -d为按字典顺序 -u为unique
sort -n -u test.txt # -u为按数值顺序
sort -d -u test.txt > test1.txt #将按覆盖写入
sort -n -u test.txt >> test1.txt #追加写
grep --fixed-strings "ab" test.txt #在test.txt中查找字符串`ab`出现位置
grep -o ab test.txt|wc -l #统计test.txt中字符串`ab`出现次数







