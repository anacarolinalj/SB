#define CODIFICA 1
#define MEDIUM_SIZE 50
#define MAX_SIZE 100
#define BYTE_POR_VEZ 3

/*
 * funcao para codificacao de um arquivo binario, usando base 64
 * a funcao recebe o nome do arquivo a ser codificado
 * a funcao escreve em um arquivo txt a codificao usando base 64 
 * a funcao nao retorna nada
 */
void codifica_64(char nome_arq[]);

/* funcao para codificar uma string de comprimento 3 para base64
 */
void codifica_64_comp3(char linha[], FILE* esc_arq);

/* funcao para codificar uma string de comprimento 2 para base64
 */
void codifica_64_comp2(char linha[], FILE* esc_arq);

/* funcao para codificar uma string de comprimento 1 para base64
 */
void codifica_64_comp1(char linha[], FILE* esc_arq);

/* 
 * funcao para decodificacao de um arquivo binario, usando base 64
 * a funcao recebe o nome do arquivo a ser decodificado
 * 
 * a funcao escreve em um arquivo txt o original assumindo que na 
 * codificacao foi usada base 64 com padding
 *
 * a funcao nao retorna nada
 */
void decodifica_64(char nome_arq[]);

/*
 * funcao para decodificar uma string de comprimento 4
 * que foi codificada na base 64
 */
void decodifica_64_24(char linha[], FILE* esc_arq);
