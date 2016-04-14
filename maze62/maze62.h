#define CODIFICACAO 0
#define DECODIFICACAO 1

#ifndef INCLUDES_MAZE_H
#define INCLUDES_MAZE_H

#include<stdio.h>
#include<stdint.h>
#include"m62_codificador.h"
#include"m62_decodificador.h"

#endif

uint8_t alfanumerico_hexa(uint8_t input);
uint8_t msb(uint8_t numero, uint8_t b);
uint8_t alfanumerico(uint8_t resto);
uint8_t lsb(uint8_t numero, uint8_t b);