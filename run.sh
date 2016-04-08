#!/bin/bash

gcc -Wall -std=c99 -g -o exe base64.c
echo "Done with compilation"

echo "Starting tests"

# toy tests
#echo "test1"
#./exe < tests/test1.txt
#echo "test2"
#./exe file1.txt < tests/test2.txt
#echo "test3"
#./exe file1.txt 2
#echo "real tests"
#./exe real1.txt 1  
#./exe real2.txt 1

# test on given files
#echo "test on korf"
#./exe korf_frontier_search.pdf 1
#echo "test on duran"
#./exe duran.mp3 1 # o nome Duran Duran foi substituido
#diff 
