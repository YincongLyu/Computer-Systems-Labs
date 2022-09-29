for i in 'seq 50000'
> do
> echo $RANDOM | md5sum | cut -c 1-9
> done >>hw1.txt
//按字母排序
sort hw1.txt | uniq;
//按数字排序
sort -n hw1.txt | uniq;
//覆盖
sort hw1.txt | uniq >>file1.txt;
//追加
sort -n hw1.txt >>file2.txt;

for i in 'seq 1000'
> do
> echo $RANDOM | md5sum | cut -c 1-9
> done >>question2.txt
grep -o "computer" question2.txt | wc -w;