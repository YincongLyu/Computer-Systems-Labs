for((i=1;i<=1000;i++))
> do
> pass=`echo $RANDOM | md5sum | cut -c11-20`
> echo -e "\t ${pass}" >> happy.txt
> done

sort -u happy.txt
sort -n -u happy.txt
sort -u happy.txt > rrw.txt
sort -u -n happy.txt >> rrw.txt

#随机生成的文件
for((i=1;i<=102;i++))
> do
> echo I love cats >>test.txt
> echo I love dogs >>tset.txt
> done

grep cats test.txt
grep -c "love" test.txt
