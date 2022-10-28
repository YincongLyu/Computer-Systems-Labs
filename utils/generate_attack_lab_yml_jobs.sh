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
        echo "    - name: Execute Codes"
        echo "      run: |"
        echo '        ddl=`date -d "2022-11-20 15:59" +%s --utc`'
        echo '        current_time=`date +%s`'
        echo '        [ $current_time -le $ddl ]'
        echo "        set -x"
        echo "        cd ./lab4/attack-handout"
        echo "        cd ./$i"
        echo "        cat answer1.txt | ./hex2raw | ./ctarget -q | grep "PASS" | wc -l"
        echo '        [ $? -eq 2 ]'
        echo "        cat answer2.txt | ./hex2raw | ./ctarget -q | grep "PASS" | wc -l"
        echo '        [ $? -eq 2 ]'
        echo "        cat answer3.txt | ./hex2raw | ./ctarget -q | grep "PASS" | wc -l"
        echo '        [ $? -eq 2 ]'
        echo "        cat answer4.txt | ./hex2raw | ./rtarget -q | grep "PASS" | wc -l"
        echo '        [ $? -eq 2 ]'
done
