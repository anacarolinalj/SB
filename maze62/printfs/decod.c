#define DECODIFICACAO_H
#include "m62_decodificador.h"
#undef DECODIFICACAO_H

static FILE *teste;

static FILE *saida_decodificacao;

void escrever_saida_decodificacao(uint8_t output) {
    fprintf(saida_decodificacao,"%c",output);
}

void decodificacao(char *nome_entrada, char *nome_saida) {
    uint8_t input;
    uint8_t cont=0;
    uint8_t lido=6, falta=8;
    uint8_t v;
    uint8_t input_resto=0x00;

    FILE *entrada;
    
    entrada = fopen(nome_entrada,"r");
    saida_decodificacao = fopen(nome_saida,"w+b");
    
    teste = fopen("korf_frontier_search.pdf","rb");
    
    
    if(entrada) {
        while((fread(&input,(sizeof(uint8_t)),1,entrada)!=0)) {
            if(falta>=6) lido=6;
            else lido=falta;
            
            printf("\nINPUT: %c",input);
            v = alfanumerico_hexa(input);
            if((input!='A')&&(input!='B')) { 
                
                concatena_bits(v,falta,lido,&input_resto,CASO_6);
                forma_byte(v,lido,falta,&input_resto,CASO_6,input);
                seta_falta(&falta,lido,CASO_6);
                
            } else {
                printf("\n-------------------CASO ESPECIAL-------------------\n");
                caso_especial_decodificacao(v,entrada,input,lido,&falta,&input_resto,CASO_6);
            }
        
                cont++;
        }
        fclose(entrada);
        fclose(saida_decodificacao);
        fclose(teste);
    }
}


void caso_especial_decodificacao(uint8_t v, FILE *entrada, uint8_t input, uint8_t lido, uint8_t *falta, uint8_t *input_resto, uint8_t caso) {
    uint8_t v1=v;
    uint8_t input1=input;
    uint8_t lido1 = lido;
    uint8_t falta_esp=*falta;
    uint8_t input_resto_esp=*input_resto;
    
    uint8_t v2;
    uint8_t input2;
    
    
    fread(&input2,(sizeof(uint8_t)),1,entrada);
    
    v2 = alfanumerico_hexa(input2);
    v1 = acha_codigo_caso_especial(input1,v1,v2);
    v2 = v2>>1;


    concatena_bits(v1,falta_esp,lido1,&input_resto_esp,caso);
    forma_byte(v1,lido1,falta_esp,&input_resto_esp,caso,input1);
    seta_falta(&falta_esp,lido1,caso);
    
    if(falta_esp>=5) lido1=5;
    else lido1=falta_esp;
    v1 = v2;
    
    printf("\nResto do prox: %.2x",input_resto_esp);
    printf("\nInput2: %c", input2);
    printf("\nFalta: %d",falta_esp);
    printf("\nLidos (de %c): %d\n",input2,lido1);


    if((input2!='A')&&(input2!='B')) {
        concatena_bits(v1,falta_esp,lido1,&input_resto_esp,CASO_5);
        forma_byte(v1,lido1,falta_esp,&input_resto_esp,CASO_5,input1);
        seta_falta(&falta_esp,lido1,CASO_5);
    } else {
        printf("\n-------------------CASO ESPECIAL-------------------\n");

        caso_especial_decodificacao(v1,entrada,input2,lido1,&falta_esp,&input_resto_esp,CASO_5);
    }
    *falta = falta_esp;
    *input_resto = input_resto_esp;
}


void concatena_bits(uint8_t v, uint8_t falta, uint8_t lido, uint8_t *input_resto, uint8_t caso) {
    if(falta<=lido) {
        *input_resto = (*input_resto)|(v>>(caso-lido));
    } else {
        *input_resto = (*input_resto)|(v<<(falta-lido));
    }
}


void forma_byte(uint8_t v, uint8_t lido, uint8_t falta, uint8_t *input_resto, uint8_t caso, uint8_t input) {
    uint8_t out,mask;
    
    if(falta==lido) {
        mask = 8-lido;
        out = *input_resto;
            
        if(lido<caso) {
            if(caso==CASO_6) {
                *input_resto = (lsb(v,mask))<<(2+lido);
            } else {
                *input_resto = (lsb(v,mask))<<(3+lido);
            }
        } else *input_resto=0x00;
        
        
        
        uint8_t x;
        static int cont=0;
        fread(&x,(sizeof(uint8_t)),1,teste);
        
        cont++;
        //escrever_saida_decodificacao(out);
        printf("\nOut: %.2x",out);
        printf("\nResto do prox: %.2x",*input_resto);
        printf("\nInput1: %c", input);
        printf("\nFalta: %d",falta);
        printf("\nLidos (de %c): %d",input,lido);
        printf("\nOriginal: %.2x; Obtido: %.2x",x,out);
        printf("\nCont: %d\n",cont);

        if(x!=out) {
            fclose(saida_decodificacao);
            fclose(teste);
            exit(0);
        }
    }
}

void seta_falta(uint8_t *falta, uint8_t lido, uint8_t caso) {
    
    *falta-=lido;
    if(*falta==0) {
        if(lido<caso) {
            *falta=8-(caso-lido);
        } else *falta=8;
    }
}

uint8_t acha_codigo_caso_especial(uint8_t input, uint8_t v1, uint8_t v2) {
    if (input=='A') {
        if((v2&(~(0xfe)))==0x00) {
            v1 = 0x00;
        } else {
            v1 = 0x3e;
        }
            
    } else {
        if((v2&(~(0xfe)))==0x00) {
            v1 = 0x01;
        } else {
            v1 = 0x3f;
        }
    }
    return v1;
}