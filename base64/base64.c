#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "base64.h" 

// array que de um indice permitira obter a letra correspondente em base 64
char conversor[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

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
        printf("Escolha o que deseja fazer com o arquivo.\n");
        printf("Digite 1 caso deseje codificar e decodificar o arquivo.\n");
        printf("Digite 2 caso deseje codificar o arquivo.\n");
        printf("Digite 3 caso deseje decodificar o arquivo.\n");
        printf("Digite sua opcao:  ");    
        scanf ("%d", &opcao);
    }

    // caso um argumento tenha sido passado, sera o nome do arquivo. Pega entao a opcao
    else if (argc == 2)
    {
        strcpy(nome_arq, argv[1]);
        printf("Escolha o que deseja fazer com o arquivo.\n");
        printf("Digite 1 caso deseje codificar e decodificar o arquivo.\n");
        printf("Digite 2 caso deseje codificar o arquivo.\n");
        printf("Digite 3 caso deseje decodificar o arquivo.\n");
        printf("Digite sua opcao:  ");    
        scanf ("%d", &opcao);
    }

    // caso ambos os argumentos tenham sido passados por linha de comando  
    else
    {
        strcpy(nome_arq, argv[1]);
        opcao = atoi(argv[2]);
    }
 
    // chama metodo conveniente, dependendo dos argumentos passados
    if (opcao == CODIFICA_DECODIFICA)
    {
        codifica64(nome_arq);
        decodifica64("codifica64");
        
    }
    else if (opcao == CODIFICA)
    {
        codifica64(nome_arq);
    }
    else if (opcao == DECODIFICA)
    {
        decodifica64(nome_arq); 
    }
    else
    {
        exit(EXIT_FAILURE);
    }
     
    return 0;
}

void codifica64(char nome_arq[])
{   
    FILE *le_arq, *esc_arq;

    uint8_t bits24[3];
    char nome_escrita[MEDIUM_SIZE] = "codifica64";
    uint8_t* p_aux;
    int comprimento, result;

    // abre arquivo binario para leitura do seu binario 
    le_arq = fopen(nome_arq, "rb");
    if (le_arq == NULL)
    {
        printf("problema na abertura do arquivo, programa termina com erro! \n");
        exit(EXIT_FAILURE);
    }

    // abre arquivo binario para escrita
    esc_arq = fopen(nome_escrita, "wb");
    if (esc_arq == NULL)
    {
        printf("problema para gerar arquivo que sera escrito, programa termina com erro! \n");
        exit(EXIT_FAILURE);
    }

    // enquanto nao chegamos ao fim do arquivo 
    while (!feof(le_arq))
    {
        // le tres bytes, ou o que der do arquivo e poe na string 
        p_aux = malloc(sizeof(uint8_t));
        p_aux = bits24;
        for (comprimento = 0; comprimento < 3; comprimento++)
        {
            result = fread(p_aux, 1, 1, le_arq);
            if (result == 0)
            {
                break;
            }
            p_aux++;
        }

        // se ha tres caracteres a considerar
        if (comprimento == 3)
        {
            codifica64_comp3(bits24, esc_arq); 
        }

        // se soh ha dois caracteres a considerar
        else if (comprimento == 2)
        {
            codifica64_comp2(bits24, esc_arq);
        }

        // se soh ha um caracter a considerar
        else if (comprimento == 1)
        {
            codifica64_comp1(bits24, esc_arq);
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

void decodifica64(char nome_arq[])
{
    FILE *le_arq, *esc_arq;
    char nome_escrita[MEDIUM_SIZE] = "decodifica64";
    uint8_t bytes[4];
    uint8_t* p_aux;
    int result;
    int comprimento;
    
    // abre arquivo para leitura
    le_arq = fopen(nome_arq, "rb");
    if (le_arq == NULL)
    {
        printf("problema na abertura do arquivo, programa termina com erro! \n");
        exit(EXIT_FAILURE);
    }

    // abre arquivo para escrita
    esc_arq = fopen(nome_escrita, "wb");
    if (esc_arq == NULL)
    {
        printf("problema na abertura do arquivo, programa termina com erro! \n");
        exit(EXIT_FAILURE);
    }

    // enquanto nao chegamos ao fim do arquivo 
    while (!feof(le_arq))
    {
        // le tres bytes, ou o que der do arquivo e poe na string 
        p_aux = malloc(sizeof(uint8_t));
        p_aux = bytes;
        for (comprimento = 0; comprimento < 4; comprimento++)
        {
            result = fread(p_aux, 1, 1, le_arq);
            if (result == 0)
            {
                p_aux++;
                break;
            }
            p_aux++;
        }

        // ocasionalmente no fim de arquivo o comprimento pode ser menor do que 4
        // a solucao encontrada foi sair do loop imediatamente (funciona)
        if (comprimento != 4)
        {
            break;
        }

        // decodifica sequencia de 4 caracteres
        decodifica64_24(bytes, esc_arq);
    }

    // fecha arquivos abertos
    fclose(le_arq);
    fclose(esc_arq);
}

void codifica64_comp3(uint8_t bits24[], FILE* esc_arq)
{
    int valor, parte1, parte2, parte3, parte4;

    // obtem valor em bytes dos 3 primeiros caracteres
    valor = (bits24[0] << 16) & 0x00FFFFFF;
    valor += (bits24[1] << 8) & 0x0000FFFF;
    valor += (bits24[2]) & 0x000000FF;

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

void codifica64_comp2(uint8_t bits24[], FILE* esc_arq)
{
    int valor, parte1, parte2, parte3;

    // obtem valor dos 2 caracteres concatenados
    valor = (bits24[0] << 16) & 0x00FFFFFF;
    valor += (bits24[1] << 8) & 0X0000FFFF;

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

void codifica64_comp1(uint8_t bits24[], FILE* esc_arq)
{
    int valor, parte1, parte2;

    // obtem valor do 1 caractere
    valor = (bits24[0] << 16) & 0x00FFFFFF;

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

void decodifica64_24(uint8_t bytes[], FILE* esc_arq)
{
    int qtd_caracter, valor; 
    int parte1, parte2, parte3;
    int i = 0; 

    // pega a quantidade de caracteres que a string contem
    while (i < 4)
    {
        if (bytes[i] == '=')
        {
            break;
        }   

        i++;
    }
    qtd_caracter = i - 1; 

    // se a string tera 3 caracteres
    if (qtd_caracter == 3)
    {   
        // pega o valor em bits da string
        valor = (strchr(conversor, bytes[0]) - conversor) << 18;
        valor += (strchr(conversor, bytes[1]) - conversor) << 12;
        valor += (strchr(conversor, bytes[2]) - conversor) << 6;
        valor += (strchr(conversor, bytes[3]) - conversor);
        
        // pega e imprime o valor de cada um dos 3 caracteres individualmente
        parte1 = valor & 0xFF0000;
        parte1 = parte1 >> 16;
        putc(parte1, esc_arq);

        parte2 = valor & 0x00FF00;
        parte2 = parte2 >> 8;
        putc(parte2, esc_arq);

        parte3 = valor & 0x0000FF;
        putc(parte3, esc_arq);
    }
    
    // se a string tera 2 caracteres
    else if (qtd_caracter == 2)
    {
        // pega o valor em bits da string
        valor = (strchr(conversor, bytes[0]) - conversor) << 18;
        valor += (strchr(conversor, bytes[1]) - conversor) << 12;
        valor += (strchr(conversor, bytes[2]) - conversor) << 6;

        // pega e imprime o valor de cada um dos 2 caracteres individualmente
        parte1 = valor & 0xFF0000;
        parte1 = parte1 >> 16;
        putc(parte1, esc_arq);

        parte2 = valor & 0x00FF00;
        parte2 = parte2 >> 8;
        putc(parte2, esc_arq);
    }

    // se a string tem apenas 1 caractere
    else
    {
        // pega o valor em bits da string
        valor = (strchr(conversor, bytes[0]) - conversor) << 18;
        valor += (strchr(conversor, bytes[1]) - conversor) << 12;

        // pega e imprime o valor do caracter
        parte1 = valor & 0xFF0000;
        parte1 = parte1 >> 16;
        putc(parte1, esc_arq);
    }
}
