for i in `seq 57`
do
        echo "job-3_$i:"
        echo "  runs-on: ubuntu-latest"
        echo "  steps:"
        echo "  - uses: actions/checkout@v2"
        echo "  - name: Execute Codes"
        echo "    run: |"
        echo "      set -x"
        echo "      cd ./lab3/bomb-handout"
        echo "      cd ./$i"
        echo "      ./bomb answer.txt"
done
