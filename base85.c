#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "base85.h" 

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
        printf("Escolha sua opcao.\n");
        printf("Digite 1 para codificar e decodificar\n");
        printf("Digite 2 para codificar\n");
        printf("Digite 3 para decodificar\n");
        printf("Digite sua opcao:  ");    
        scanf ("%d", &opcao);
    }

    // caso um argumento tenha sido passado, sera o nome do arquivo. Pega entao a opcao
    else if (argc == 2)
    {
        strcpy(nome_arq, argv[1]);
        printf("Escolha sua opcao\n");
        printf("1 para codificar e decodificar o arquivo passado\n");
        printf("3 para decodificar\n");
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
        codifica85(nome_arq);
        decodifica85("codifica85");
        
    }
    else if (opcao == CODIFICA)
    {
        codifica85(nome_arq);
    }
    else if (opcao == DECODIFICA)
    {
        decodifica85(nome_arq); 
    }
    else
    {
        exit(1);
    }
     
    return 0;
}

void codifica85(char nome_arq[])
{   
    FILE *le_arq, *esc_arq;

    uint8_t bytes[4];
    char nome_escrita[MEDIUM_SIZE] = "codifica85";
    uint8_t* p_aux;
    int comprimento, qtd_car, result;


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
        // le quatro bytes, ou o que der do arquivo e poe na string 
        p_aux = malloc(sizeof(uint8_t));
        p_aux = bytes;
        for (comprimento = 0; comprimento < 4; comprimento++)
        {
            result = fread(p_aux, 1, 1, le_arq);
            if (result == 0)
            {
                break;
            }
            p_aux++;
        }
        
        if (comprimento == 0)
        {
            //printf("erro");
            break;
        }

        // enquanto o comprimento nao for igual a 4, acrescenta bytes 0
        for (int i = comprimento; i < 4; i++)
        {
            bytes[i] = 0;
        }   

        // pega quantos bytes devem ser escritos 
        qtd_car = comprimento + 1;

        // codifica bytes 
        codifica85bytes(bytes, esc_arq, qtd_car);
    }
    
    // escreve string final 
    putc('~', esc_arq);
    putc('>', esc_arq);

    // fecha arquivos abertos
    fclose(le_arq);
    fclose(esc_arq);
}

void codifica85bytes(uint8_t bytes[], FILE* esc_arq, int qtd)
{   
    uint32_t valor;
    uint8_t parte[5];
    uint32_t dividendo;
    bool zero_string;

    // obtem valor contido nos 4 bytes
    valor = (bytes[0] << 24) & 0xFF000000;
    valor += (bytes[1] << 16) & 0x00FF0000;
    valor += (bytes[2] << 8) & 0x0000FF00;
    valor += (bytes[3] << 0) & 0x000000FF;

    // obtem valor de cada uma das 5 partes
    dividendo = valor; 
    for(int i = 1; i <= 5; i++)
    {
         parte[5 - i] = dividendo % 85; 
         dividendo = dividendo / 85;
    }
    
    // testa se a string contem apenas 0's
    zero_string = true;
    for (int i = 0; i < 5; i++)
    {
        if (parte[i] != 0)
        {
            zero_string = false;
            break;
        }
    }
    
    // se a string nao contem soh zeros
    if (!zero_string)
    {
        // escreve quantidade correta de caracteres no arquivo 
        // por causa do padding, podemos escrever de 2 a 5 caracteres 
        for (int i = 0; i < qtd; i++)
        {
            putc(parte[i] + 33, esc_arq);
        }
    }
    
    // se a string contem soh zeros
    else
    {
        putc('z', esc_arq);
    }
}

void decodifica85(char nome_arq[])
{   
    FILE *le_arq, *esc_arq;

    uint8_t bytes[5];
    char nome_escrita[MEDIUM_SIZE] = "decodifica85";
    uint8_t* p_aux;
    int comprimento, qtd_car, result;
    bool leu_z;
    bool fim = false, finaliza_prox = false;


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
        // a cada iteracao, ainda nao lemos um z
        leu_z = false;

        // le 5 bytes, ou o que der do arquivo e poe no array
        p_aux = malloc(sizeof(uint8_t));
        p_aux = bytes;
        for (comprimento = 0; comprimento < 5; comprimento++)
        {
            result = fread(p_aux, 1, 1, le_arq);
            if (result == 0)
            {
                break;
            }
            
            // se lemos um '~' chegamos ao fim do arquivo 
            if (*p_aux == '~')
            {
                // caso seja o primeiro caracter, nao precisa escrever nada
                if (comprimento == 0)
                {
                    fim = true;    
                }

                // caso nao seja o primeiro caracter
                else
                {
                    for(int i = comprimento; i < 5; i++)
                    {  
                        //escreve u's ate que o comprimento fique igual a 5
                        bytes[i] = 'u';
                    }
                        
                    // como depois vem um '~', chegamos ao fim do arquivo e pode-se sair na proxima iteracao
                    finaliza_prox = true; 
                }

                break;
            }

            // se lemos um z entao podemos imprimir uma sequencia de quatro zeros e sair 
            if (*p_aux == 'z')
            {
                leu_z = true;
                break;
            }
            p_aux++;
        }
        
        // se chegamos ao fim do arquivo 
        if (fim)
        {
            // sai do loop
            break;
        }

        // se lemos um z
        if (leu_z)
        {
            // imprime quatro zeros e vai para proxima iteracao do loop 
            for(int i = 0; i < 4; i++)
            {
                putc(0, esc_arq);
            }
            continue;
        }
        
        // pega quantidade de caracteres que devemos escrever
        qtd_car = comprimento - 1;

        // decodifica bytes 
        decodifica85bytes(bytes, esc_arq, qtd_car);

        // se os bytes eram os ultimos
        if (finaliza_prox)
        {
            // sai do loop
            break;
        }
    }
     
    // fecha arquivos abertos
    fclose(le_arq);
    fclose(esc_arq);
}

void decodifica85bytes(uint8_t bytes[], FILE* esc_arq, int qtd)
{
    uint32_t valor = 0; 
    uint32_t partes[4];

    // obtem valor que o array de bytes representa
    for (int i = 0; i < 5; i++) 
    {
        valor = (valor * 85 + (bytes[i] - 33));
    }

    // pega valor de cada uma das 4 partes e imprime
    partes[0] = valor & 0xFF000000;
    partes[0] = partes[0] >> 24;
    partes[1] = valor & 0x00FF0000;
    partes[1] = partes[1] >> 16;
    partes[2] = valor & 0x0000FF00;
    partes[2] = partes[2] >> 8;
    partes[3] = valor & 0x000000FF;
    partes[3] = partes[3] >> 0;
    
    // imprime a quantidade de caracteres passadas
    for (int i = 0; i < qtd; i++)
    {
        putc(partes[i], esc_arq);
    }
}
