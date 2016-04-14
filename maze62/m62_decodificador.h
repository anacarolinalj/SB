#ifndef INCLUDES_H
#define INCLUDES_H

#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include"maze62.h"

#endif

#define CASO_6 6
#define CASO_5 5


void escrever_saida_decodificacao(uint8_t output);
void concatena_bits(uint8_t v, uint8_t falta, uint8_t lido, uint8_t *input_resto, uint8_t caso);
void forma_byte(uint8_t v, uint8_t lido, uint8_t falta, uint8_t *input_resto, uint8_t caso, uint8_t input);
void seta_falta(uint8_t *falta, uint8_t lido, uint8_t caso);
uint8_t acha_codigo_caso_especial(uint8_t input, uint8_t v1, uint8_t v2);
void caso_especial_decodificacao(uint8_t v, FILE *entrada, uint8_t input, uint8_t lido, uint8_t *falta, uint8_t *input_resto, uint8_t caso);
void decodificacao(char *nome_entrada, char *nome_saida);

