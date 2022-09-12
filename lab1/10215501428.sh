#! /bin/bash
for i in `seq 1000`;do echo $RANDOM | md5sum | cut -c 1-9 ;done >> hw1.txt

sort hw1.txt | uniq -u
sort -n hw1.txt | uniq -u
sort hw1.txt > 2forhw1
sort -n hw1.txt > 3forhw1
sort hw1.txt >> 2forhw1
sort -n hw1.txt >> 3forhw1

grep 'sc' hw1.txt
grep -o 'sc' hw1.txt
grep 'sc' hw1.txt | wc -l
grep -o 'sc' hw1.txt | wc