#!/bin/bash

gcc -Wall -std=c99 -g -lm -o b85 base85.c

#./b85 aa 1
#diff aa decodifica85
#echo "finished"

./b85 c.jpg 1
diff c.jpg decodifica85
echo "done"

#./b85 c.jpg 2
