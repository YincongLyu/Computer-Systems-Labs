for i in `seq 1000`;do echo $RANDOM | md5sum | cut -c 1-9 ;done >> zuoye.txt
	sort -d zuoye.txt | uniq -u > zimu.txt
	sort -n zuoye.txt  | uniq -u > shuzi.txt
	cat zimu.txt >> zhuijia.txt
       	cat shuzi.txt >> zhuijia.txt
	vim chazhao.txt
	grep "computer" chazhao.txt
	grep -o "computer" chazhao.txt |wc -l
