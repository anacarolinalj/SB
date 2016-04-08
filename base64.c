#include <stdio.h> 
#include <string.h>
#include <stdlib.h>

#include "base64.h" 

// array que de um indice permitira obter a letra correspondente em base 64
const char conversor[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int main(int argc, char* argv[])
{   
    char nome_arq[MEDIUM_SIZE];
    int opcao;
    
    // caso nenhum argumento tenha sido passado via linha de comando, pega ambos
    if (argc == 1)
    {
        printf("Na hora de executar o programa, nao foi digitado o nome do arquivo.");
        printf("\nDigite o nome do arquivo: ");        
        scanf ("%s", nome_arq);
        printf("Escolha se deseja codificar o arquivo passado (digite 1) ou decodificar (digite 2): ");
        printf("Digite sua opcao:  ");    
        scanf ("%d", &opcao);

    }

    // caso um argumento tenha sido passado, sera o nome do arquivo. Pega entao a opcao
    else if (argc == 2)
    {
        strcpy(nome_arq, argv[1]);
        printf("Escolha se deseja codificar o arquivo passado (digite 1) ou decodificar (digite 2): ");
        printf("Digite sua opcao:  ");    
        scanf ("%d", &opcao);
    }

    // caso ambos os argumentos tenham sido passados por linha de comando  
    else
    {
        strcpy(nome_arq, argv[1]);
        opcao = atoi(argv[2]);
    }
 
    // teste
    printf("%s\n", nome_arq);
    printf("%d\n", opcao);

    // chama metodo conveniente, dependendo dos argumentos passados
    if (opcao == CODIFICA)
    {
        codifica_64(nome_arq);
        
    }
    else
    {
        decodifica_64(nome_arq);
    }
     
    return 0;
}

void codifica_64(char nome_arq[])
{   
    FILE *le_arq, *esc_arq;

    char linha[BYTE_POR_VEZ];
    char nome_escrita[MEDIUM_SIZE] = "arquivo_codificado_64";
    int comprimento;

    // abre arquivo binario para leitura do seu binario 
    le_arq = fopen(nome_arq, "rb");
    if (le_arq == NULL)
    {
        printf("problema na abertura do arquivo, programa termina com erro! \n");
        exit(EXIT_FAILURE);
    }

    // abre arquivo binario para escrita
    esc_arq = fopen(nome_escrita, "w");
    if (esc_arq == NULL)
    {
        printf("problema para gerar arquivo que sera escrito, programa termina com erro! \n");
        exit(EXIT_FAILURE);
    }

    // vai lendo linhas do arquivo, uma a uma 
    while (fgets(linha, BYTE_POR_VEZ + 1, le_arq) != NULL)
    {
        comprimento = strlen(linha); 

        // se ha tres caracteres a considerar
        if (comprimento == 3)
        {
            codifica_64_comp3(linha, esc_arq); 

        }

        // se soh ha dois caracteres a considerar
        else if (comprimento == 2)
        {
            codifica_64_comp2(linha, esc_arq);
        }

        // se soh ha um caracter a considerar
        else if (comprimento == 1)
        {
            codifica_64_comp1(linha, esc_arq);
        }

        // se nao ha nenhum caracter a considerar, passa para proxima iteracao
        else
        {
            continue;
        }

    }
    
    // fecha arquivos abertos
    fclose(le_arq);
    fclose(esc_arq);
}

void decodifica_64(char nome_arq[])
{
    // abre arquivo para leitura
    
    // vai lendo 4 caracteres por vez
         
    // decodifica sequencia de 4 caracteres 
    
    // fecha arquivos abertos
}

void codifica_64_comp3(char linha[], FILE* esc_arq) 
{
    int valor, parte1, parte2, parte3, parte4;

    // obtem valor em bytes dos 3 primeiros caracteres
    valor = linha[0] << 16;
    valor += linha[1] << 8;
    valor += linha[2];

    // obtem valor de cada uma das 4 partes da string e imprime no arquivo de saida
    parte1 = valor & 0xFC0000;
    parte1 = parte1 >> 18;
    putc(conversor[parte1], esc_arq);

    parte2 = valor & 0x03F000;
    parte2 = parte2 >> 12;
    putc(conversor[parte2], esc_arq);

    parte3 = valor & 0x000FC0;
    parte3 = parte3 >> 6;
    putc(conversor[parte3], esc_arq);

    parte4 = valor & 0x00003F;
    putc(conversor[parte4], esc_arq);
}

void codifica_64_comp2(char linha[], FILE* esc_arq) 
{
    int valor, parte1, parte2, parte3;

    // obtem valor dos 2 caracteres concatenados
    valor = linha[0] << 16;
    valor += linha[1] << 8;

    // obtem valor das tres partes da string
    parte1 = valor & 0xFC0000;
    parte1 = parte1 >> 18;
    putc(conversor[parte1], esc_arq);

    parte2 = valor & 0x03F000;
    parte2 = parte2 >> 12;
    putc(conversor[parte2], esc_arq);

    parte3 = valor & 0x000FC0;
    parte3 = parte3 >> 6;
    putc(conversor[parte3], esc_arq);

    // o ultimo simbolo eh um sinal de igual
    putc('=', esc_arq);
}

void codifica_64_comp1(char linha[], FILE* esc_arq) 
{
    int valor, parte1, parte2;

    // obtem valor do 1 caractere
    valor = linha[0] << 16;
    valor += linha[1] << 8;

    // obtem valor das duas partes da string
    parte1 = valor & 0xFC0000;
    parte1 = parte1 >> 18;
    putc(conversor[parte1], esc_arq);

    parte2 = valor & 0x03F000;
    parte2 = parte2 >> 12;
    putc(conversor[parte2], esc_arq);

    // os dois ultimos simbolos sao sinais de igual
    putc('=', esc_arq);
    putc('=', esc_arq);
}
