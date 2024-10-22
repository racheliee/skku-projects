#!/bin/bash

# get arguments
a=$1
b=$2

#make file
make w3

#execute w3
echo "$a $b" | ./w3 > output.txt

#execute bash calculation
echo $((a + b)) > answer.txt
echo $((a - b)) >> answer.txt

# find difference
diff answer.txt output.txt > result.txt

# clean up files
make clean
