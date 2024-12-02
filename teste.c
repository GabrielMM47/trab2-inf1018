#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cria_func.h"

typedef int (*func_ptr_recebe_nada)();
typedef int (*func_ptr_recebe_int)(int x);
typedef int (*func_ptr_recebe_void_e_size_t)(void *candidata, size_t n);
typedef int (*func_ptr_recebe_size_t)(size_t n);

char fixa[] = "quero saber se a outra string é um prefixo dessa";

int mult(int x, int y)
{
    return x * y;
}

int retorna_igual(int x)
{ // tem que ter pelo menos um parâmetro
    return x;
}

int retorna_igual_ponteiro(void *x)
{
    return (int)x;
}

void teste_retorna_igual_original(void)
{
    DescParam params[1];
    func_ptr_recebe_int f_retorna_igual;
    int i;
    unsigned char codigo[500];

    params[0].tipo_val = INT_PAR; /* a nova função passa para retorna_igual um valor inteiro */
    params[0].orig_val = PARAM;   /* que é o mesmo valor que foi passado para a função */

    cria_func(retorna_igual, params, 1, codigo);
    f_retorna_igual = (func_ptr_recebe_int)codigo;

    for (i = 1; i <= 10; i++)
    {
        fprintf(stderr, "chamando...\n");
        printf("%d\n", f_retorna_igual(i)); /* a nova função recebe 1 argumento */
    }
}

void teste_retorna_igual_constante_inteiro(void)
{
    DescParam params[1];
    func_ptr_recebe_nada f_retorna_igual;
    int i;
    unsigned char codigo[500];

    params[0].tipo_val = INT_PAR; /* a nova função passa para retorna_igual um valor inteiro */
    params[0].orig_val = FIX;     /* que é uma constante */
    params[0].valor.v_int = 42;

    cria_func(retorna_igual, params, 1, codigo);
    f_retorna_igual = (func_ptr_recebe_nada)codigo;

    for (i = 1; i <= 10; i++)
    {
        fprintf(stderr, "chamando...\n");
        printf("%d\n", f_retorna_igual()); /* a nova função não recebe argumento */
    }
}

void teste_retorna_igual_constante_ponteiro(void)
{
    DescParam params[1];
    func_ptr_recebe_nada f_retorna_igual_ponteiro;
    int i;
    unsigned char codigo[500];

    params[0].tipo_val = PTR_PAR; /* a nova função passa para retorna_igual um valor de ponteiro */
    params[0].orig_val = FIX;     /* que é uma constante */
    params[0].valor.v_ptr = (void*)0x7;

    cria_func(retorna_igual_ponteiro, params, 1, codigo);
    f_retorna_igual_ponteiro = (func_ptr_recebe_nada)codigo;

    for (i = 1; i <= 10; i++)
    {
        fprintf(stderr, "chamando...\n");
        printf("%d\n", f_retorna_igual_ponteiro()); /* a nova função não recebe argumento */
    }
}

void teste_retorna_igual_ind_inteiro(void)
{
    DescParam params[1];
    func_ptr_recebe_nada f_retorna_igual;
    int i;
    unsigned char codigo[500];

    params[0].tipo_val = INT_PAR; /* a nova função passa para retorna_igual um valor inteiro */
    params[0].orig_val = IND;     /* que vem de uma variável */
    params[0].valor.v_ptr = &i;

    cria_func(retorna_igual, params, 1, codigo);
    f_retorna_igual = (func_ptr_recebe_nada)codigo;

    for (i = 1; i <= 10; i++)
    {
        fprintf(stderr, "chamando...\n");
        printf("%d\n", f_retorna_igual()); /* a nova função não recebe argumento */
    }
}

void teste_retorna_igual_ind_ponteiro(void)
{
    DescParam params[1];
    func_ptr_recebe_nada f_retorna_igual_ponteiro;
    void* i;
    unsigned char codigo[500];

    params[0].tipo_val = PTR_PAR; /* a nova função passa para retorna_igual um valor de ponteiro */
    params[0].orig_val = IND;     /* que vem de uma variável */
    params[0].valor.v_ptr = &i;

    cria_func(retorna_igual_ponteiro, params, 1, codigo);
    f_retorna_igual_ponteiro = (func_ptr_recebe_nada)codigo;

    for (i = (void*)1; i <= (void*)10; i++)
    {
        fprintf(stderr, "chamando...\n");
        printf("%d\n", f_retorna_igual_ponteiro()); /* a nova função não recebe argumento */
    }
}

void teste_mult_ind_fix(void)
{
    DescParam params[2];
    func_ptr_recebe_nada f_mult;
    int i;
    unsigned char codigo[500];

    params[0].tipo_val = INT_PAR; /* a nova função passa para mult um valor inteiro */
    params[0].orig_val = IND;     /* que é o valor corrente da variavel i */
    params[0].valor.v_ptr = &i;

    params[1].tipo_val = INT_PAR; /* o segundo argumento passado para mult é a constante 10 */
    params[1].orig_val = FIX;
    params[1].valor.v_int = 10;

    cria_func(mult, params, 2, codigo);
    f_mult = (func_ptr_recebe_nada)codigo;

    for (i = 1; i <= 10; i++)
    {
        fprintf(stderr, "chamando...\n");
        printf("%d\n", f_mult()); /* a nova função não recebe argumentos */
    }
}

void teste_mult_fix_param(void)
{
    DescParam params[2];
    func_ptr_recebe_int f_mult;
    int i;
    unsigned char codigo[500];

    params[0].tipo_val = INT_PAR; /* a nova função passa para mult um valor inteiro */
    params[0].orig_val = FIX;     /* que é uma constante */
    params[0].valor.v_int = 10;

    params[1].tipo_val = INT_PAR; /* o segundo argumento passado é passado sem alteração */
    params[1].orig_val = PARAM;

    cria_func(mult, params, 2, codigo);
    f_mult = (func_ptr_recebe_int)codigo;

    for (i = 1; i <= 10; i++)
    {
        fprintf(stderr, "chamando...\n");
        printf("%d\n", f_mult(i)); /* a nova função recebe 1 argumento */
    }
}

void teste_memcmp_fix_param_param(void)
{
    DescParam params[3];
    func_ptr_recebe_void_e_size_t mesmo_prefixo;
    char s[] = "quero saber tudo";
    int tam;
    unsigned char codigo[500];

    params[0].tipo_val = PTR_PAR; /* o primeiro parâmetro de memcmp é um ponteiro para char */
    params[0].orig_val = FIX;     /* a nova função passa para memcmp o endereço da string "fixa" */
    params[0].valor.v_ptr = fixa;

    params[1].tipo_val = PTR_PAR; /* o segundo parâmetro de memcmp é também um ponteiro para char */
    params[1].orig_val = PARAM;   /* a nova função recebe esse ponteiro e repassa para memcmp */

    params[2].tipo_val = INT_PAR; /* o terceiro parâmetro de memcmp é um inteiro */
    params[2].orig_val = PARAM;   /* a nova função recebe esse inteiro e repassa para memcmp */

    cria_func(memcmp, params, 3, codigo);
    mesmo_prefixo = (func_ptr_recebe_void_e_size_t)codigo;

    tam = 12;
    printf("'%s' tem mesmo prefixo-%d de '%s'? %s\n", s, tam, fixa, mesmo_prefixo(s, tam) ? "NAO" : "SIM");
    tam = strlen(s);
    printf("'%s' tem mesmo prefixo-%d de '%s'? %s\n", s, tam, fixa, mesmo_prefixo(s, tam) ? "NAO" : "SIM");
}

void teste_memcmp_ind_fix_param(void)
{
    DescParam params[3];
    func_ptr_recebe_size_t mesmo_prefixo;
    char s[] = "quero saber tudo";
    int tam;
    unsigned char codigo[500];
    char* stringdin = (char*)malloc(15*sizeof(char));

    params[0].tipo_val = PTR_PAR; /* o primeiro parâmetro de memcmp é um ponteiro para char */
    params[0].orig_val = IND;     /* a nova função passa para memcmp o endereço que estiver na stringdin */
    params[0].valor.v_ptr = &stringdin;

    params[1].tipo_val = PTR_PAR; /* o segundo parâmetro de memcmp é também um ponteiro para char */
    params[1].orig_val = FIX;   /* a nova função pega o valor fixo do "s" e passa*/
    params[1].valor.v_ptr = s;

    params[2].tipo_val = INT_PAR; /* o terceiro parâmetro de memcmp é um inteiro */
    params[2].orig_val = PARAM;   /* a nova função recebe esse inteiro e repassa para memcmp */

    cria_func(memcmp, params, 3, codigo);
    mesmo_prefixo = (func_ptr_recebe_size_t)codigo;

    tam = 5;
    strcpy(stringdin, "queello");
    printf("'%s' tem mesmo prefixo-%d de '%s'? %s\n", s, tam, stringdin, mesmo_prefixo(tam) ? "NAO" : "SIM");
    strcpy(stringdin, "quero");
    printf("'%s' tem mesmo prefixo-%d de '%s'? %s\n", s, tam, stringdin, mesmo_prefixo(tam) ? "NAO" : "SIM");
}

int main(void)
{
    teste_retorna_igual_original();
    teste_retorna_igual_constante_inteiro();
    teste_retorna_igual_constante_ponteiro();
    teste_retorna_igual_ind_inteiro();
    teste_retorna_igual_ind_ponteiro();
    teste_mult_ind_fix();
    teste_mult_fix_param();
    teste_memcmp_fix_param_param();
    teste_memcmp_ind_fix_param();
}