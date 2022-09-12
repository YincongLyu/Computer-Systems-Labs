   #生成文件
   for i in `seq 5000`;
   do echo $RANDOM | md5sum | cut -c 1-9;
   done>>test.txt;
   
   #1
   sort -u test.txt #按字母排序
   sort -u -n test.txt #按数字排序
   sort -u test.txt>1.txt #将字母排序结果覆盖写到1.txt中
   sort -u test.txt>>2.txt #将字母排序结果追加写到2.txt中
   sort -u -n test.txt>1.txt #将数字排序结果覆盖写到1.txt中
   sort -u -n test.txt>>2.txt #将数字排序结果追加写到2.txt中
   
   #2
   grep "abc" test.txt #找出内容包含"abc"的字符串并输出
   grep -c "abc" test.txt #统计内容包含"abc"的行数并输出