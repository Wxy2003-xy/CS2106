#!/bin/bash

# Check if we have enough arguments
num_of_args=$#
if [[ $num_of_args -ne 3 ]]; then
    echo "Usage: ./grade.sh <filename> <number_of_tests> <query_point>"
    exit 1
fi

filename=$1
num_of_tests=$2
query_point=$3

if [[ $num_of_tests -le 0 ]]; then
    echo "number_of_tests must be larger than 0"
    exit 1
fi

if [[ $query_point -lt 0 ]]; then
    echo "query_point must be larger or equal to 0"
    exit 1
fi

if [[ $query_point -gt $num_of_tests ]]; then
    echo "query_point must be less than or equal to the number_of_tests"
    exit 1
fi

avail_tests=0
for i in ref/*; do
    avail_tests=$((avail_tests + 1))
done

if [[ $num_of_tests -gt $avail_tests ]]; then
    num_of_tests=$avail_tests
fi

# Delete temporary files
rm -f ref/*.out

# Compile the reference program
gcc ref/sum.c ref/utils.c -o ref/sum

# Generate reference output files
for i in ref/*.in; do
    ref/sum < "$i" > "${i%.*}.out"
done

# Now mark submissions
n=$(whoami)
d=$(date "+%A, %d %b %Y %H:%M:%S")
echo "Test date and time: $d"

full_score=0
for i in subs/*; do
    gcc "$i/sum.c" "$i/utils.c" -o "$i/sum" 2>/dev/null
    if [[ $? -ne 0 ]]; then
        echo "Student ${i:5} has a compilation error."
        continue
    fi

    for j in ref/*.in; do
        "$i/sum" < "$j" > "${j%.*}.sub"
    done

    score=0
    for k in ref/*.sub; do
        reference="${k:0:6}.out"
        if diff -q "$k" "$reference" >/dev/null; then
            score=$((score + 1))
        fi
    done

    if [[ $score -eq $query_point ]]; then
        full_score=$((full_score + 1))
    fi

    echo "Student ${i:5} score $score / $num_of_tests"
done

echo "There are $full_score students with $query_point scores."

# Note: See Lab020n.pdf for format of output file. Marks will be deducted for missing elements.
