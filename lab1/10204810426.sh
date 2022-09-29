for i in `seq 50000`;do echo $RANDOM | md5sum | cut -c 1-9 ;done >> test.txt
sort -n|uniq//按数字排序
sort -d |uniq//按字符排序
sort -n |uniq>test1.txt//覆盖写
sort -d |uniq>test1.txt//覆盖写
sort -n |uniq>>test2.txt//追加写
sort -d |uniq >>test.txt//追加写

for((i=0;i<200;i++)) do head -c 100/dev/urandom|tr-dc A-Za-z>>Testfile;echo-e \>>Testfile;done
grep -r com Testfile
grep -c com Testfile
