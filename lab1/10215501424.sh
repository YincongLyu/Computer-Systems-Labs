#!/bin/bash
for i in `seq 5`;do echo $RANDOM | md5sum | cut -c 1-9 ;done > test.txt
sort -u test.txt
sort -u -n test.txt
sort -n -u test.txt > test_a.txt
cat test_a.txt >> test_b.txt
echo "OK"




#!/bin/bash
for i in `seq 5`;do echo  "apple" && echo "app";done > test2.txt
cat test2.txt
grep -rn "apple" test2.txt |wc -l
echo "OK"