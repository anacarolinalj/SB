#ifndef MAZE_H
#define MAZE_H
#include "maze62.h"
#endif

uint8_t alfanumerico_hexa(uint8_t input) {
    uint8_t v;
    
    if (input<0x39) v=input-0x30+0x34; 
    else if (input<0x5B) v = input-0x41; 
    else v= input-0x61+0x1A;
    
    return v;
}


uint8_t msb(uint8_t numero, uint8_t b) {
    uint8_t r;

    switch(b) {
        case 0: r = 0x00; break;
        case 1: r = numero&(~(0x7e)); break;
        case 2: r = numero&(~(0x3e)); break;
        case 3: r = numero&(~(0x1e)); break;
        case 4: r = numero&(~(0x0e)); break;
        case 5: r = numero&(~(0x07)); break;
        case 6: r = numero&(~(0x03)); break;
        case 7: r = numero&(~(0x01)); break;
        case 8: r = numero;
    }

    return r;
}

uint8_t alfanumerico(uint8_t resto) {
    uint8_t r;
    
    if (resto<=0x19) r = resto+0x41; 
    else if (resto<0x34) r = resto+0x61-0x1A; 
    else if (resto<0x62) r = resto+0x30-0x34; 
    else if (resto==0x62) r = 0x41; 
    else resto = 0x42;
        
    return r;
}

uint8_t lsb(uint8_t numero, uint8_t b) {
    return numero&((1 << b)-1);
}

