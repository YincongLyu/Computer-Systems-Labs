# 第1题
for i in `seq 1000`
	do echo $RANDOM | md5sum | cut -c 1-9 >> data.txt
done
sort data.txt -u
sort data.txt -n -u
sort data.txt -u > test1.txt
sort data.txt -n -u >> test2.txt

# 第2题
grep -o world  random.txt |wc -l



