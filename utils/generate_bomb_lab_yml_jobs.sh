echo "name: Bomb Lab CI"

echo "on:"
echo "  push:"
echo "  pull_request:"
echo "env:"
echo '  GITHUB_TOKEN: ${{ secrets.GITHUB_TOKEN }}'

echo "jobs:"

for i in `seq 70`
do
        echo "  job-3_$i:"
        echo "    runs-on: ubuntu-latest"
        echo "    steps:"
        echo "    - uses: actions/checkout@v2"
        echo "    - name: Execute Codes"
        echo "      run: |"
        echo '        ddl=`date -d "2022-11-06 15:59" +%s --utc`'
        echo '        current_time=`date +%s`'
        echo '        [ $current_time -le $ddl ]'
        echo "        set -ex"
        echo "        cd ./lab3/bomb-handout"
        echo "        cd ./$i"
        echo "        ./bomb answer.txt"
done
