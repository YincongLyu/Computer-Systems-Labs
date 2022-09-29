#1
sort --unique test.txt

sort --numeric-sort --unique test.txt

sort --output=1.sh --unique test.txt
sort --output=1.sh --numeric-sort --unique test.txt

sort --unique test.txt>>1.sh
sort --numeric-sort --unique test.txt>>1.sh

#2
grep -c "010" test.txt
