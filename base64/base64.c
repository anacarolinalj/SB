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
    int comprimento, qtd_car; 
    int car_linha = 0;

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
        p_aux = bits24;
        comprimento = fread(p_aux, 1, BYTE_POR_VEZ, le_arq);

        // se chegamos ao fim do arquivo, sai 
        if (comprimento == 0)
        {
            break;
        }

        // acrescenta bytes de 0 para preencher string
        for (int i = comprimento; i < 3; i++)
        {
            bits24[i] = 0;
        }

        // pega quantidade de caracteres a ser escrita
        qtd_car = comprimento + 1; 

        // codifica os 24 bits 
        codifica24(bits24, esc_arq, qtd_car);

        // incrementa os caracteres escritos na linha e imprime \n se chegamos a 76
        car_linha += 4;
        if (car_linha == 76)
        {
            putc('\n', esc_arq);
            car_linha = 0;
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
    uint8_t  *p_aux, *temp;
    int comprimento, car_linha = 0; 
    
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

    // aloca espaco 
    temp = malloc(sizeof(uint8_t));

    // enquanto nao chegamos ao fim do arquivo 
    while (!feof(le_arq))
    {
        // le quatro bytes, ou o que der do arquivo e poe na string 
        p_aux = bytes;
        comprimento = fread(p_aux, 1, BYTES_DECODIFICA, le_arq);

        // incrementa quantidade de caracteres lido na linha e se for igual a 76 le o \n
        car_linha += 4; 
        if (car_linha == 76)
        {
            fread(temp, 1, 1, le_arq);
            car_linha = 0;
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

    // libera espaco alocado
    free(temp);

    // fecha arquivos abertos
    fclose(le_arq);
    fclose(esc_arq);
}

void codifica24(uint8_t bits24[], FILE* esc_arq, int qtd_car)
{
    uint32_t valor = 0;
    uint8_t parte[3];

    // obtem valor em bytes do array 
    valor = (bits24[0] << 16) & 0x00FFFFFF;
    valor += (bits24[1] << 8) & 0x0000FFFF;
    valor += (bits24[2]) & 0x000000FF;


    // obtem valor de cada parte da string
    parte[0] = (valor & 0x00FC0000) >> 18;
    parte[1] = (valor & 0x0003F000) >> 12;
    parte[2] = (valor & 0x00000FC0) >> 6;
    parte[3] = (valor & 0x0000003F) >> 0;

    // escreve caracteres
    for (int i = 0; i < qtd_car; i++)
    {
        putc(conversor[parte[i]], esc_arq);
    }

    // escreve sinal de igual, se necessario
    for (int i = qtd_car; i < 4; i++)
    {
        putc('=', esc_arq);
    }

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
