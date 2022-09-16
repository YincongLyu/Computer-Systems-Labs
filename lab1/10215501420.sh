 for i in `seq 10000`;do echo $RANDOM | md5sum | cut -c 1-9 ;done >> test.txt
 sort test.txt | uniq -u >> test1.txt
 sort --numeric-sort test.txt |uniq -u >> test2.txt
 sort test.txt | uniq -u > test3.txt
 sort --numeric-sort test.txt |uniq -u > test3.txt
 sort test.txt | uniq -u >> test4.txt
 sort --numeric-sort test.txt |uniq -u >> test4.txt
 grep "cd" test.txt
 grep "cd" test.txt | wc -l