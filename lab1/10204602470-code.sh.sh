#生成 50000 行的文件，每一行可以包含字母、数字。长度不限制。
for((i = 0; i < 50000; i++))
do
head -c 50 /dev/urandom | tr -dc A-Za-z0-9 >> RandChara;
echo -e \ >> RandChara;
done

#按照字母顺序排序这个文件，输出所有的 unique 行。小写在前，大写在后。
sort RandChara | uniq;

#按照数字顺序排序这个文件，输出所有的 unique 行。
sort -n RandChara | uniq;

# 以覆盖写的方式将上述的结果重定向到一个文件中
sort RandChara | uniq >> CoverFile;

#以追加写的方式将上述的结果重定向到一个文件中
sort -n RandChara | uniq >> AddFile;