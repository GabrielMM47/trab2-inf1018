/* Gabriel Martins Mendes - 2311271 - Turma 3WC */
/* Leo Klinger Svartman - 2310862 - Turma 3WC */

#include "cria_func.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void moverdi2rsi(unsigned char* codigo, int* ind){
    // mov %rdi, %rsi
    codigo[(*ind)++] = 0x48;
    codigo[(*ind)++] = 0x89;
    codigo[(*ind)++] = 0xfe; 
}

void moversi2rdx(unsigned char* codigo, int* ind){
    // mov %rsi, %rdx
    codigo[(*ind)++] = 0x48;
    codigo[(*ind)++] = 0x89;
    codigo[(*ind)++] = 0xf2; 
}

void moverdi2rdx(unsigned char* codigo, int* ind){
    // mov %rdi, %rdx
    codigo[(*ind)++] = 0x48;
    codigo[(*ind)++] = 0x89;
    codigo[(*ind)++] = 0xfa; 
}

void end2reg(unsigned char* codigo, int* ind, unsigned char* reg, void* end){
    codigo[(*ind)++] = reg[0];
    codigo[(*ind)++] = reg[1]; 
    memcpy(&codigo[*ind], end, sizeof(void *));
    *ind += sizeof(void *);
}

void cria_func(void *f, DescParam params[], int n, unsigned char codigo[]){
    int ind = 0;

    // ALINHANDO A PILHA
    codigo[ind++] = 0x55;
    codigo[ind++] = 0x48;
    codigo[ind++] = 0x89;
    codigo[ind++] = 0xe5;
    codigo[ind++] = 0x48; 
    codigo[ind++] = 0x83;
    codigo[ind++] = 0xec;
    codigo[ind++] = 0x10;

    // PASSA O ENDEREÇO DA FUNÇÃO QUE SERÁ CHAMADA COM O CALL INDIRETO PARA O CÓDIGO
    unsigned char mov2rax[] = {0x48,0xb8};
    end2reg(codigo, &ind, mov2rax, &f);

    // ALINHAMENTO DOS PARAMs PARA OS REGISTRADORES CERTOS 
    int numPARAM = 0;
    for (int i = 0; i < n; i++){
        if (params[i].orig_val == PARAM)
            numPARAM++;
    }

    if (numPARAM != n){
        if (n == 2){
            if (params[0].orig_val != PARAM){
                // MOVER DO REGISTRADOR DE 1º ARGUMENTO PARA O DE 2º ARGUMENTO
                moverdi2rsi(codigo, &ind);
            }
        }

        if (n == 3){
            if(numPARAM == 1){
                if (params[0].orig_val != PARAM){
                    if (params[1].orig_val == PARAM){
                        // MOVER DO REGISTRADOR DE 1º ARGUMENTO PARA O DE 2º ARGUMENTO
                        moverdi2rsi(codigo, &ind);
                    }
                    else if (params[2].orig_val == PARAM){
                        // MOVER DO REGISTRADOR DE 1º ARGUMENTO PARA O DE 3º ARGUMENTO
                        moverdi2rdx(codigo, &ind);
                    }
                }
            }

            else {
                if (params[0].orig_val != PARAM){
                    // MOVER DO REGISTRADOR DE 2º ARGUMENTO PARA O DE 3º ARGUMENTO
                    moversi2rdx(codigo, &ind);

                    // MOVER DO REGISTRADOR DE 1º ARGUMENTO PARA O DE 2º ARGUMENTO
                    moverdi2rsi(codigo, &ind);
                }

                if (params[1].orig_val != PARAM){
                    // MOVER DO REGISTRADOR DE 2º ARGUMENTO PARA O DE 3º ARGUMENTO
                    moversi2rdx(codigo, &ind);
                }
            }
        }
    }

    // DEFININDO OS HEXADECIMAIS REFERENTES A MOVIMENTAÇÃO DE UM VALOR DE 8 BYTES PARA OS REGISTRADORES QUE SERÃO UTILIZADOS ABAIXO
    // REGISTRADORES DE ARGUMENTOS
    unsigned char mov2rdi[] = {0x48,0xbf};
    unsigned char mov2rsi[] = {0x48,0xbe};
    unsigned char mov2rdx[] = {0x48,0xba};

    // PASSANDO OS PARÂMETROS DAS FUNÇÕES PARA OS REGISTRADORES REFERENTES AOS ARGUMENTOS CORRETOS
    for (int i = 0; i < n; i++){
        switch (params[i].tipo_val){
        case INT_PAR:

            switch (params[i].orig_val){
            case PARAM:
            // CASO TRATADO NO PRIMEIRO FOR DA FUNÇÃO 
                break;

            case FIX:
            // PASSA A CONSTANTE DE 4 BYTES (INTEIRO) PARA O REGISTRADOR REFERENTE AO ARGUMENTO
                if (i == 0)
                    codigo[ind++] = 0xbf; // %edi
                else if (i == 1)
                    codigo[ind++] = 0xbe; // %esi
                else if (i == 2)
                    codigo[ind++] = 0xba; // %edx

                memcpy(&codigo[ind], &params[i].valor.v_int, sizeof(int));
                ind += sizeof(int);
                break;

            case IND:
                int *end_int = (int *)params[i].valor.v_ptr;
                if (i == 0){
                    // PASSA O ENDEREÇO DO PONTEIRO QUE APONTA PARA A VARIÁVEL PARA O REGISTRADOR de 1º ARGUMENTO (%rdi)
                    end2reg(codigo, &ind, mov2rdi, &end_int);
                    // mov (%rdi), %edi
                    codigo[ind++] = 0x8b;
                    codigo[ind++] = 0x3f;
                }
                else if (i == 1){
                    // PASSA O ENDEREÇO DO PONTEIRO QUE APONTA PARA A VARIÁVEL PARA O REGISTRADOR 2º ARGUMENTO (%rsi)
                    end2reg(codigo, &ind, mov2rsi, &end_int);
                    // mov (%rsi), %esi
                    codigo[ind++] = 0x8b;
                    codigo[ind++] = 0x36;
                }
                else if (i == 2){
                    // PASSA O ENDEREÇO DO PONTEIRO QUE APONTA PARA A VARIÁVEL PARA O REGISTRADOR 3º ARGUMENTO (%rdx)
                    end2reg(codigo, &ind, mov2rdx, &end_int);
                    // mov (%rdx), %edx
                    codigo[ind++] = 0x8b;
                    codigo[ind++] = 0x12;
                }
                break;
            }
            break;

        case PTR_PAR:

            switch (params[i].orig_val){
            case PARAM:
            // CASO TRATADO NO PRIMEIRO FOR DA FUNÇÃO
                break;

            case FIX:
            // PASSA A CONSTANTE DE 8 BYTES (PONTEIRO) PARA O REGISTRADOR REFERENTE AO ARGUMENTO 
                if (i == 0){
                    end2reg(codigo, &ind, mov2rdi, &params[i].valor.v_ptr); // %rdi
                }
                else if (i == 1){
                    end2reg(codigo, &ind, mov2rsi, &params[i].valor.v_ptr); // %rsi
                }
                else if (i == 2){
                    end2reg(codigo, &ind, mov2rdx, &params[i].valor.v_ptr); // %rdx
                }
                break;

            case IND:
                void* end_ptr = params[i].valor.v_ptr;
                if (i == 0){
                    // PASSA O ENDEREÇO DO PONTEIRO QUE APONTA PARA A VARIÁVEL PARA O REGISTRADOR 1º ARGUMENTO (%rdi)
                    end2reg(codigo, &ind, mov2rdi, &end_ptr);
                    // mov (%rdi), %rdi
                    codigo[ind++] = 0x48;
                    codigo[ind++] = 0x8b;
                    codigo[ind++] = 0x3f;
                }
                else if (i == 1){
                    // PASSA O ENDEREÇO DO PONTEIRO QUE APONTA PARA A VARIÁVEL PARA O REGISTRADOR 2º ARGUMENTO (%rsi)
                    end2reg(codigo, &ind, mov2rsi, &end_ptr);
                    // mov (%rsi), %rsi
                    codigo[ind++] = 0x48;
                    codigo[ind++] = 0x8b;
                    codigo[ind++] = 0x36;
                }
                else if (i==2) {
                    // PASSA O ENDEREÇO DO PONTEIRO QUE APONTA PARA A VARIÁVEL PARA O REGISTRADOR 3º ARGUMENTO (%rdx)
                    end2reg(codigo, &ind, mov2rdx, &end_ptr);
                    // mov (%rdx), %rdx
                    codigo[ind++] = 0x48;
                    codigo[ind++] = 0x8b;
                    codigo[ind++] = 0x12;
                }
                break;
            }
            break;
        }
    }

    // CHAMADA DA FUNÇÃO COM O CALL INDIRETO (call *%rax)
    codigo[ind++] = 0xff;
    codigo[ind++] = 0xd0;

    // FINALIZANDO 
    codigo[ind++] = 0xc9; // leave
    codigo[ind++] = 0xc3; // ret  
}