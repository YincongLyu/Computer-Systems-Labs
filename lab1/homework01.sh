for i in seq`1000`;do echo %RANDOM|md5sum|cut -c 1-9; done >> test.txt
sort -u test.txt
sort -n test.txt|uniq
touch f1.txt
touch f2.txt
sort -u test.txt > f1.txt
sort -u test.txt >> f2.txt


touch computer.txt
grep computer computer.txt
grep -o computer computer.txt|uniq -c