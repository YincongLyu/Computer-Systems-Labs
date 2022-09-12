#!/bin/bash
i=0
touch test.txt 
touch cover.txt
touch add.txt
cat /dev/null>test.txt 
cat /dev/null>cover.txt
cat /dev/null>add.txt

while (($i<1000))
do 
    echo $(date +%s%N | md5sum | head -c 15) >> test.txt
    let i++

sort test.txt | uniq > cover.txt 
sort test.txt -n | uniq > cover.txt
sort test.txt | uniq >> add.txt 
sort test.txt -n | uniq > add.txt
grep -o computer test.txt | wc -l






