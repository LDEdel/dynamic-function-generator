#include <stdio.h>
#include "../include/cria_func.h"

typedef int (*func_ptr)(void);

int id(int x)
{
    return x;
}

int main(void)
{
    DescParam params[1];
    unsigned char codigo[500];

    int x = 50;

    params[0].tipo_val = INT_PAR;
    params[0].orig_val = IND;
    params[0].valor.v_ptr = &x;

    cria_func(id, params, 1, codigo);

    func_ptr f = (func_ptr)codigo;

    printf("%d\n", f());

    x = 123;

    printf("%d\n", f());

    return 0;
}