#!/bin/bash
export LC_ALL=C:
sort -d randstring.txt | uniq
sort -n randstring.txt | uniq
touch n_sorted_a.txt
touch d_sorted_a.txt
touch n_sorted_w.txt
touch d_sorted_w.txt
sort -d randstring.txt | uniq >d_sorted_w.txt
sort -d randstring.txt | uniq>>d_sorted_a.txt
sort -n randstring.txt | uniq>n_sorted_w.txt
sort -n randstring.txt | uniq>>n_sorted_a.txt
grep -w "gas" essay.txt
grep -c "gas" essay.txt

