for i in `seq 1000`;do echo $RANDOM | md5sum | cut -c 1-9 ;done > test.txt
sort --general-numeric-sort test.txt | uniq
sort --numeric-sort test.txt | uniq
sort --general-numeric-sort test.txt | uniq > 1.txt
sort --numeric-sort test.txt | uniq > 1.txt
sort --general-numeric-sort test.txt | uniq >> 2.txt
sort --numeric-sort test.txt | uniq >> 2.txt
grep "56" test.txt
grep "56" test.txt | wc --lines