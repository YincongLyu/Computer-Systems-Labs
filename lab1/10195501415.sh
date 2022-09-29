#!/bin/bash
for i in `seq 1000`
do echo $RANDOM | md5sum | cut -c 1-9 
done >> origin.20220914 

sort test.txt --unique
sort test.txt --numeric -u
sort test.txt -u origin.20220914 > output_u.20220914
sort test.txt -n -u origin.20220914 >> output_n_u.20220914
echo "worked"


#!/bin/bash
grep 66 origin.20220914

grep -c 66 origin.20220914
grep -c "66" origin.20220914 | wc -l
