#!/bin/bash

gcc -Wall -std=c99 -g -lm -o b85 base85.c

./b85 a.pdf 1
diff a.pdf decodifica85

./b85 b.mp3 1
diff b.mp3 decodifica85

./b85 c.jpg 1 
diff c.jpg decodifica85

./b85 d.mp4 1
diff d.mp4 decodifica85

