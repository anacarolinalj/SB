#define CODIFICA_DECODIFICA 1
#define CODIFICA 2
#define DECODIFICA 3

#define MEDIUM_SIZE 50
#define MAX_SIZE 100
#define BYTE_POR_VEZ 3
#define BYTES_DECODIFICA 4

/*
 * funcao para codificacao de um arquivo binario, usando base 64
 * a funcao recebe o nome do arquivo a ser codificado
 * a funcao escreve em um arquivo txt a codificao usando base 64 
 * a funcao nao retorna nada
 */
void codifica64(char nome_arq[]);

/*
 * funcao para codificar em base 64 os 24 bits
 */
void codifica24(uint8_t bits24[], FILE* esc_arq, int qtd_car); 

/* 
 * funcao para decodificacao de um arquivo binario, usando base 64
 * a funcao recebe o nome do arquivo a ser decodificado
 * 
 * a funcao escreve em um arquivo txt o original assumindo que na 
 * codificacao foi usada base 64 com padding
 *
 * a funcao nao retorna nada
 */
void decodifica64(char nome_arq[]);

/*
 * funcao para decodificar uma string de comprimento 4
 * que foi codificada na base 64
 */
void decodifica64_24(uint8_t bytes[], FILE* esc_arq);
