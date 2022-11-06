echo "name: Attack Lab CI"

echo "on:"
echo "  push:"
echo "  pull_request:"
echo "env:"
echo '  GITHUB_TOKEN: ${{ secrets.GITHUB_TOKEN }}'

echo "jobs:"

for i in `seq 70`
do
        echo "  job-4_$i:"
        echo "    runs-on: ubuntu-latest"
        echo "    steps:"
        echo "    - uses: actions/checkout@v2"
        echo "    - name: Install dependencies"
        echo "      run: |"
        echo "        sudo apt-get install gdb"
        echo "    - name: Execute Codes"
        echo "      run: |"
        echo '        ddl=`date -d "2022-11-20 15:59" +%s --utc`'
        echo '        current_time=`date +%s`'
        echo '        [ $current_time -le $ddl ]'
        echo "        set -ex"
        echo "        cd ./lab4/attack-handout"
        echo "        cd ./$i"
        echo "        gdb -ex 'set logging on' -ex quit"
        echo "        cat answer1.txt | ./hex2raw > answer1_raw1.txt"
        echo "        gdb ctarget -ex 'run -q < answer1_raw1.txt' |& tee gdb.txt"
        echo '        result=`cat gdb.txt | grep "PASS" | wc -l`'
        echo '        [ $result -eq 2 ]'
        echo "        rm -f answer1_raw1.txt"
        echo "        cat answer2.txt | ./hex2raw > answer2_raw2.txt"
        echo "        gdb ctarget -ex 'run -q < answer2_raw2.txt' |& tee gdb.txt"
        echo '        result=`cat gdb.txt | grep "PASS" | wc -l`'
        echo '        [ $result -eq 2 ]'
        echo "        rm -f answer2_raw2.txt"
        echo "        cat answer3.txt | ./hex2raw > answer3_raw3.txt"
        echo "        gdb ctarget -ex 'run -q < answer3_raw3.txt' |& tee gdb.txt"
        echo '        result=`cat gdb.txt | grep "PASS" | wc -l`'
        echo '        [ $result -eq 2 ]'
        echo "        rm -f answer3_raw3.txt"
        echo "        cat answer4.txt | ./hex2raw > answer4_raw4.txt"
        echo "        gdb rtarget -ex 'run -q < answer4_raw4.txt' |& tee gdb.txt"
        echo '        result=`cat gdb.txt | grep "PASS" | wc -l`'
        echo '        [ $result -eq 2 ]'
        echo "        rm -f answer4_raw4.txt"
        echo "        rm -f gdb.txt"
done
