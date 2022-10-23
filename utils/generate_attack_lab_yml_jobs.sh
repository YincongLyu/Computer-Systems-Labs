for i in `seq 2`
do
        echo "job-4_$i:"
        echo "  runs-on: ubuntu-latest"
        echo "  steps:"
        echo "  - uses: actions/checkout@v2"
        echo "  - name: Execute Codes"
        echo "    run: |"
        echo '      ddl=`date -d "2022-11-06 15:59" +%s --utc`'
        echo '      current_time=`date +%s`'
        echo '      [ $current_time -le $ddl ]'
        echo "      set -x"
        echo "      cd ./lab3/bomb-handout"
        echo "      cd ./$i"
        echo "      cat answer1.txt | ./hex2raw | ./ctarget -q | grep "PASS" | wc -l"
        echo "      [ $? -eq 2 ]"
        echo "      cat answer2.txt | ./hex2raw | ./ctarget -q | grep "PASS" | wc -l"
        echo "      [ $? -eq 2 ]"
        echo "      cat answer3.txt | ./hex2raw | ./ctarget -q | grep "PASS" | wc -l"
        echo "      [ $? -eq 2 ]"
        echo "      cat answer4.txt | ./hex2raw | ./rtarget -q | grep "PASS" | wc -l"
        echo "      [ $? -eq 2 ]"
done