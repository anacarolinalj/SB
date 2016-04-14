#define CODIFICACAO_H
#include "m62_codificador.h"
#undef CODIFICACAO_H


static FILE *saida_codificacao;


void escrever_saida_codificacao(uint8_t r) {
    uint8_t output;
    
    output = alfanumerico(r);
    fprintf(saida_codificacao,"%c",output);
}

void caso_especial_codificacao(uint8_t *lidos, uint8_t *desp, uint8_t *input_resto, uint8_t *output, uint8_t input, uint8_t resto, uint8_t cv, FILE *entrada) {
    uint8_t quociente;
    uint8_t desp_esp = *desp;
    uint8_t lidos_esp = *lidos;
    uint8_t input_resto_esp = *input_resto;
    uint8_t out=*output;
    
    escrever_saida_codificacao(resto);
    quociente = cv/62;

    if(desp_esp==5) {
        cv = lsb(input,desp_esp)<<1|quociente;
        resto = cv%62;
           
        if((resto==0x01)||(resto==0x00)) {
            fread(&input,sizeof(uint8_t),1,entrada); 
            
            desp_esp = 8;
            lidos_esp = 0;
            caso_especial_codificacao(&lidos_esp,&desp_esp,&input_resto_esp,&out,input,resto,cv,entrada);
        
        } else {
            lidos_esp = 6;
            desp_esp = 2;
            input_resto_esp=0x00;
            escrever_saida_codificacao(resto);
        }
 
    } else if(desp_esp<=4) {
            input_resto_esp = ((lsb(input,desp_esp))<<(6-desp_esp))|quociente;
            
        
            lidos_esp = 5-(desp_esp);
            desp_esp = 8-(lidos_esp);
        
            
            fread(&input,sizeof(uint8_t),1,entrada); 

            if(desp_esp!=3) {
                cv = ((msb(input,lidos_esp)>>(8-lidos_esp))&(~(0x01)))|(input_resto_esp);
            } else {
                cv = (msb(input,5)>>2)|quociente;
            }
            resto = cv%62;
            input_resto_esp = (lsb(input,desp_esp))<<(6-desp_esp);
            
            if((resto==0x01)||(resto==0x00)) {
                desp_esp = 8;
                lidos_esp = 0;

                caso_especial_codificacao(&lidos_esp,&desp_esp,&input_resto_esp,&out,input,resto,cv,entrada);
            
        
            } else if(desp_esp>=6) {
                escrever_saida_codificacao(resto);
                cv = lsb(input,6);
                resto = cv%62;
                
                
                fread(&input,sizeof(uint8_t),1,entrada);
                if((resto==0x01)||(resto==0x00)) { 
                    desp_esp = 8;
                    lidos_esp = 0;
                    caso_especial_codificacao(&lidos_esp,&desp_esp,&input_resto_esp,&out,input,resto,cv,entrada);
                    
                } else {
                    escrever_saida_codificacao(resto);

                    cv = msb(input,6)>>2;
                    resto = cv%62;
                    input_resto_esp = lsb(input,4)<<2;
                    lidos_esp = 4;
                    desp_esp = 4;
                    if((resto==0x01)||(resto==0x00)) { 
                        caso_especial_codificacao(&lidos_esp,&desp_esp,&input_resto_esp,&out,input,resto,cv,entrada);
                    } else {
                        escrever_saida_codificacao(resto);
                    }

                }
            } else {
                lidos_esp = 8-lidos_esp;
                desp_esp = 8-lidos_esp;
                escrever_saida_codificacao(resto);
            }
    }
    else {
        if(desp_esp==6) {
            input_resto_esp = lsb(input,6)&(~(0x01));
            cv = ((input_resto_esp)|quociente);

            input_resto_esp = lsb(input,1)<<5;
                
            lidos_esp = 5;
            desp_esp = 3;
        } else if(desp_esp==7) {
            input_resto_esp = ((lsb(input,desp_esp))&(~(0x02)))>>1;
            cv = input_resto_esp|quociente;
            input_resto_esp = lsb(input,2)<<4;
                
            lidos_esp = 4;
            desp_esp = 4;
        } else {
            input_resto_esp = ((lsb(input,desp_esp))&(~(0x07)))>>2;
            cv = input_resto_esp|quociente;
            input_resto_esp = (lsb(input,3))<<3;

            lidos_esp = 3;
            desp_esp = 5;
        }
    
        resto = cv%62;
        
        if((resto==0x01)||(resto==0x00)) {
            caso_especial_codificacao(&lidos_esp,&desp_esp,&input_resto_esp,&out,input_resto_esp,resto,cv,entrada);
        
        }
        else {
            escrever_saida_codificacao(resto);
        }
    }
    
    *lidos = lidos_esp;
    *desp = desp_esp;
    *input_resto = input_resto_esp;
    *output = out;

}


void codificacao(char *nome_entrada, char *nome_saida) {
    uint8_t input;
    uint8_t output;
    uint8_t input_resto=0x00;
    uint8_t cv;
    uint8_t resto;
    uint8_t lidos=6,desp=2;
    uint32_t cont=0;
    
    
    FILE *entrada;
    
    entrada = fopen(nome_entrada,"rb");
    saida_codificacao = fopen(nome_saida,"w+");
    
    if(entrada) {
        while(fread(&input,sizeof(uint8_t),1,entrada)!=0) {
            cv = (input>>desp)|input_resto;
            resto = cv%62;

            if((resto!=0)&&(resto!=1)) {
                if(desp==7) {
                    escrever_saida_codificacao(resto);
        
                    lidos = 1;
                    desp = 7;
    
                    cv = (input<<1);
                    cv = cv>>2;
                    input_resto = (lsb(input,1))<<5;
                    lidos=6;
                    desp=1;
                    
                    
                    resto = cv%62;
                    if((resto!=0)&&(resto!=1)) {
                        escrever_saida_codificacao(resto);
    
                        lidos=5;
                        desp=3;
                    } else caso_especial_codificacao(&lidos,&desp,&input_resto,&output,input,resto,cv,entrada);
            
                } else if(desp==6) {
                    escrever_saida_codificacao(resto);
                
                    cv = (lsb(input,6));
                    resto = cv%62;
                    
                    lidos = 6;
                    desp = 0;
                    
                    input_resto = 0x00;
                    if((resto!=0)&&(resto!=1)) {
                        escrever_saida_codificacao(resto);
                        cont++;
            
                        lidos=6;
                        desp=2;
                    } else
                        caso_especial_codificacao(&lidos,&desp,&input_resto,&output,input,resto,cv,entrada);

                } else {
                    input_resto = (lsb(input,desp))<<(6-desp);
                    escrever_saida_codificacao(resto);
    
        
                    lidos = 6-desp;
                    desp = 8-lidos;
                }
            } else caso_especial_codificacao(&lidos,&desp,&input_resto,&output,input,resto,cv,entrada);
            cont++;
        }
        fclose(entrada);
        fclose(saida_codificacao);
    }

}