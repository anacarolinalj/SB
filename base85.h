#define CODIFICA_DECODIFICA 1
#define CODIFICA 2
#define DECODIFICA 3

#define MEDIUM_SIZE 50
#define MAX_SIZE 100
#define BYTE_POR_VEZ 4

void codifica85(char nome_arq[]);
void codifica85bytes(uint8_t bytes[], FILE* esc_arq, int qtd);

