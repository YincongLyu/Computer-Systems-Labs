# Question1
i=0
 
# Generate 50000 random strings into text.txt 
while (($i<50000))
do
	echo $random | md5sum | cut -c 1-9 >> test.txt
	((i++))
done

# >> for w and > for a
sort -n test.txt | uniq > 1.txt
sort -n test.txt | uniq >> 2.txt
sort -f test.txt | uniq > 3.txt
sort -f test.txt | uniq >> 4.txt


# Question2
# We use the same file test.txt For testing
# Find the string 'computer' 
# Sum up and print, use pipe as an interface
grep -rn "computer" test.txt
grep -o "computer" test.txt | wc -l
