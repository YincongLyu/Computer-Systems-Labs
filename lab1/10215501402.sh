for i in `seq 50000`;do random=`hexdump -n 2 -e '/2 "%u"' /dev/urandom`; echo $random; done
for i in `seq 50000`;do echo $RANDOM | md5sum | cut -c 1-9 ;done >> test.txt
sort ./test.txt
sort --reverse ./test.txt
sort ./test.txt | uniq -u
sort --numeric-sort ./test.txt
sort ./test.txt | uniq -u > test1.txt
sort ./test.txt | uniq -u >> test1.txt
grep "ab" ./test.txt
grep -o "ab" ./test.txt | wc -l
