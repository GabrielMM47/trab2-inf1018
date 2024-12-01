#include "cria_func.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Gabriel Martins Mendes - 2311271 - Turma 3WC*/
/* Leo Klinger Svartman - 2310862 - Turma 3WC*/

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

void cria_func(void *f, DescParam params[], int n, unsigned char codigo[]){

    printf("Endereço recebido: %p\n", f);
    // TA COM BASTANTE COMENTARIO PARA A GENTE NÃO SE PERDER... (DEPOIS TIRAMOS ALGUNS
    int ind = 0;

    //ALINHANDO A PILHA
    codigo[ind++] = 0x55;
    codigo[ind++] = 0x48;
    codigo[ind++] = 0x89;
    codigo[ind++] = 0xe5;
    codigo[ind++] = 0x48; 
    codigo[ind++] = 0x83;
    codigo[ind++] = 0xec;
    codigo[ind++] = 0x10;

    codigo[ind++] = 0x48;
    codigo[ind++] = 0xb8;                     // movabs constante 64bits, r12
    memcpy(&codigo[ind], &f, sizeof(void *)); // Passa o endereço da função para o código
    ind += sizeof(void *);


    // tem que ver se está tudo correto, pois quem chama pode não passar todos os parâmetros, o que desalinha 
    int contaPARAM = 0;
    for (int i = 0; i < n; i++){
        if (params[i].orig_val == PARAM)
            contaPARAM++;
    }

    if (contaPARAM != n){
        if (n == 2){
            if (params[0].orig_val != PARAM){
                // tenho que mover de di para si
                moverdi2rsi(codigo, &ind);
            }
        }

        if (n == 3){
            if(contaPARAM == 1){
                if (params[0].orig_val != PARAM){
                    if (params[1].orig_val == PARAM){
                        // tenho que mover de di para si
                        moverdi2rsi(codigo, &ind);
                    }
                    else if (params[2].orig_val == PARAM){
                        // Movo de rdi para rdx
                        moverdi2rdx(codigo, &ind);
                    }
                }
                // nao tem else, pois está no lugar certo.
            }
            else // 2 parametros, 3 não pode pq não é != n
            {
                if (params[0].orig_val != PARAM){
                    // tenho que shiftar todos para a direita
                    // fazendo si --> dx primeiro
                    moversi2rdx(codigo, &ind);

                    // Movo de rdi para rsi
                    moverdi2rsi(codigo, &ind);
                }
                if (params[1].orig_val != PARAM){
                    // so tenho que mover um, pois significa que o outro ta no lugar certo (di)
                    moversi2rdx(codigo, &ind);
                }
            }
        }
    }

    /*
    n == 2:

    Caso (1º FIX, 2º PARAM): Move rdi → rsi. Resolvido.
    Caso (1º PARAM, 2º FIX): Nenhuma ação necessária. Resolvido.

    n == 3:

    1º FIX, 2º PARAM, 3º FIX: Move rdi → rsi. Resolvido.
    1º FIX, 2º FIX, 3º PARAM: Move rdi → rdx. Resolvido.
    1º PARAM, 2º FIX, 3º FIX: Nenhuma ação necessária. Resolvido.
    1º FIX, 2º FIX, 3º FIX (não há PARAM): Nenhuma ação necessária. Resolvido.
    1º FIX, 2º PARAM, 3º PARAM:
        Move rsi → rdx.
        Move rdi → rsi. Resolvido.
    1º PARAM, 2º FIX, 3º PARAM:
        Move rsi → rdx. Resolvido.
    
    */

    // copiar todos os parâmetros para os registradores corretos
    for (int i = 0; i < n; i++){
        switch (params[i].tipo_val){
        case INT_PAR:
            switch (params[i].orig_val){
            case PARAM:
                // ja estão no lugar certo :)
                continue;
                break;
            case FIX:
                if (i == 0)
                    codigo[ind++] = 0xbf; // edi
                else if (i == 1)
                    codigo[ind++] = 0xbe; // esi
                else if (i == 2)
                    codigo[ind++] = 0xba; // edx

                memcpy(&codigo[ind], &params[i].valor.v_int, sizeof(int));
                ind += sizeof(int);
                // copia bem sucedida

                break;
            case IND:
                // o que eu movo aqui é o valor do ponteiro para r9 e depois (r9) para o respectivo reg
                int *endereco = (int *)params[i].valor.v_ptr;
                if (i == 0){
                    // vou mover esse endereço para r9 se for o primeiro param
                    codigo[ind++] = 0x49;
                    codigo[ind++] = 0xb9;
                    memcpy(&codigo[ind], &endereco, sizeof(int *));
                    ind += sizeof(int *);
                    codigo[ind++] = 0x41;
                    codigo[ind++] = 0x8b;
                    codigo[ind++] = 0x39;
                }
                else if (i == 1){
                    // vou mover esse endereço para r10 se for o segundo param
                    codigo[ind++] = 0x49;
                    codigo[ind++] = 0xba;
                    memcpy(&codigo[ind], &endereco, sizeof(int *));
                    ind += sizeof(int *);
                    codigo[ind++] = 0x41;
                    codigo[ind++] = 0x8b;
                    codigo[ind++] = 0x32;
                }
                else if (i == 2){
                    // vou mover esse endereco para r11 se for o terceiro param
                    codigo[ind++] = 0x49;
                    codigo[ind++] = 0xbb;
                    memcpy(&codigo[ind], &endereco, sizeof(int *));
                    ind += sizeof(int *);
                    codigo[ind++] = 0x41;
                    codigo[ind++] = 0x8b;
                    codigo[ind++] = 0x13;
                }
                break;
            }
            break;

        case PTR_PAR:
            switch (params[i].orig_val){
            case PARAM:
                // TODO
                // já era aqui :)
                continue;
                break;

            case FIX:
                if (i == 0){
                    codigo[ind++] = 0x48;
                    codigo[ind++] = 0xbf; // rdi
                }
                else if (i == 1){
                    codigo[ind++] = 0x48;
                    codigo[ind++] = 0xbe; // rsi
                }
                else if (i == 2){
                    codigo[ind++] = 0x48;
                    codigo[ind++] = 0xba; // rdx
                }
                memcpy(&codigo[ind], &params[i].valor.v_ptr, sizeof(void *));
                printf("Endereço recebido em cria_func: %p\n", params[i].valor.v_ptr);
                ind += sizeof(void *);
                break;

            case IND:
                void* endereco = params[i].valor.v_ptr;
                if (i == 0){
                    // move para r9 e desreferencia em rdi
                    codigo[ind++] = 0x49;
                    codigo [ind++] = 0xb9;
                    memcpy(&codigo[ind], &endereco, sizeof(void*));
                    ind += sizeof(void*);
                    codigo[ind++] = 0x49;
                    codigo[ind++] = 0x8b;
                    codigo[ind++] = 0x39;
                }
                else if (i == 2){
                    // move para r10 e desreferencia em rsi
                    codigo[ind++] = 0x49;
                    codigo [ind++] = 0xba;
                    memcpy(&codigo[ind], &endereco, sizeof(void*));
                    ind += sizeof(void*);
                    codigo[ind++] = 0x49;
                    codigo[ind++] = 0x8b;
                    codigo[ind++] = 0x32;
                }
                else{
                    // move para r11 e desreferencia em rdx
                    codigo[ind++] = 0x49;
                    codigo [ind++] = 0xbb;
                    memcpy(&codigo[ind], &endereco, sizeof(void*));
                    ind += sizeof(void*);
                    codigo[ind++] = 0x49;
                    codigo[ind++] = 0x8b;
                    codigo[ind++] = 0x13;
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