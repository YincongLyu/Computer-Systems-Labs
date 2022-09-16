for i in `seq 1000`
do echo $RANDOM | md5sum | cut -c 1-9 
done >> test.txt 

sort test.txt -u
sort test.txt -n -u
sort test.txt -u > test1.txt
sort test.txt -n -u >> test2.txt

grep -o new ramdom.txt |wc -1