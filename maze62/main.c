#ifndef MAZE_H
#define MAZE_H
#include "maze62.h"
#undef MAZE_H
#endif

void maze62(int opcao, char *arquivo_entrada, char *arquivo_saida) {
    if(opcao==CODIFICACAO) {
        codificacao(arquivo_entrada,arquivo_saida);
    } else decodificacao(arquivo_entrada,arquivo_saida);
}


int main(void) {
    
    maze62(CODIFICACAO,"korf_frontier_search.pdf","korf_frontier_search.b62");
    maze62(DECODIFICACAO,"korf_frontier_search.b62","decod_korf_frontier_search.txt");

    return 0;
}