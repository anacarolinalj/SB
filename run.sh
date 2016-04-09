#!/bin/bash

gcc -Wall -std=c99 -g -o exe base64.c

./exe a.pdf 1
diff a.pdf decodifica_64

./exe b.mp3 1
diff b.mp3 decodifica_64

./exe c.jpg 1 
diff c.jpg decodifica_64

./exe d.mp4 1
diff d.mp4 decodifica_64

# simple tests codificacao
#echo "real tests"
#./exe real1.txt 1  
#./exe real2.txt 1

# test on given files
#echo "test on korf"
#./exe korf_frontier_search.pdf 1
#echo "test on duran"
#./exe duran.mp3 1 # o nome Duran Duran foi substituido
#diff 

#simple tests decodificacao
#./exe real3.txt 2

#./exe aa 1
#diff aa decodifica_64
#echo "done"

#./exe real3.txt 1
#diff real3.txt decodifica_64
#echo "done"

# teste integrando decodificacao e codificacao 
#./exe ff 1
#diff ff decodifica_64
#echo "done"

# teste integrando decodificacao e codificacao em um arquivo grande passado
#./exe a.pdf 1
#diff a.pdf decodifica_64
#echo "done"


# teste integrando decodificacao e codificacao no mp3
#./exe b.mp3 1
#diff b.mp3 decodifica_64
#echo "done"

# teste integrando decodificacao e codificacao no .jpg
#./exe c.jpg 1
#./exe codificado_64 2
#diff c.jpg decodifica-64
#echo "done"

