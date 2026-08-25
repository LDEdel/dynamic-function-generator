RELATÓRIO DE TESTES – GERADOR DINÂMICO DE FUNÇÕES

Aluno: Leonardo Dana Edelsberg
Matrícula: 2510515

1. Objetivo

O objetivo dos testes foi validar a implementação da função cria_func para Linux x86-64, verificando a geração dinâmica de código para funções com até três parâmetros, incluindo parâmetros repassados, fixos e indiretos, tanto para inteiros quanto para ponteiros.

2. Teste Inicial

Função utilizada:

int id(int x)
{
return x;
}

Configuração:

params[0].tipo_val = INT_PAR;
params[0].orig_val = PARAM;

Chamada:

f_id(i);

Resultado obtido:

1
2
3
4
5
6
7
8
9
10

Conclusão:

O código gerado executa corretamente uma chamada indireta para a função original, preservando o valor de retorno.

3. Teste de Parâmetro Fixo (FIX)

Função utilizada:

int mult(int x, int y)
{
return x * y;
}

Configuração:

params[0] = PARAM
params[1] = FIX(10)

Resultado obtido:

10
20
30
40
50
60
70
80
90
100

Conclusão:

Parâmetros inteiros fixos estão sendo carregados corretamente.

4. Teste de Remapeamento de Registradores

Função utilizada:

int soma3(int a, int b, int c)
{
return a + b + c;
}

Configuração:

params[0] = PARAM
params[1] = FIX(100)
params[2] = PARAM

Chamada:

f_soma(1,2,3);

A função gerada deveria chamar:

soma3(1,100,2)

Resultado obtido:

103

Conclusão:

O remapeamento dos registradores de parâmetros está correto.

5. Teste de Ponteiro Fixo (PTR_PAR + FIX)

Função utilizada:

int tamanho(char *s)
{
return strlen(s);
}

Configuração:

params[0].tipo_val = PTR_PAR;
params[0].orig_val = FIX;
params[0].valor.v_ptr = "abcdef";

Resultado obtido:

6

Conclusão:

Ponteiros fixos são carregados corretamente.

6. Teste de Ponteiro Indireto (PTR_PAR + IND)

Função utilizada:

int tamanho(char *s)
{
return strlen(s);
}

Configuração:

char *str = "hello";

params[0].tipo_val = PTR_PAR;
params[0].orig_val = IND;
params[0].valor.v_ptr = &str;

Resultado obtido:

5

Após alterar:

str = "abcdefghij";

Resultado obtido:

10

Conclusão:

O valor corrente armazenado no endereço informado é carregado corretamente a cada chamada.

7. Teste com strcmp

Função utilizada:

int compara(char *a, char *b)
{
return strcmp(a,b);
}

Configuração:

params[0] = FIX("abcdef")
params[1] = PARAM

Resultados obtidos:

0
-23

Interpretação:

strcmp retorna:

0 quando as strings são iguais;
valor negativo quando a primeira string é lexicograficamente menor.

Conclusão:

Parâmetros ponteiro fixos e parâmetros ponteiro recebidos pela função gerada estão funcionando corretamente.

8. Teste de Inteiro Indireto (INT_PAR + IND)

Função utilizada:

int id(int x)
{
return x;
}

Configuração:

int x = 50;

params[0].tipo_val = INT_PAR;
params[0].orig_val = IND;
params[0].valor.v_ptr = &x;

Primeira chamada:

50

Após:

x = 123;

Segunda chamada:

123

Conclusão:

A implementação carrega corretamente o valor inteiro armazenado no endereço fornecido.

9. Conclusão

A implementação gera corretamente código de máquina para Linux x86-64, suportando até três parâmetros e as três formas de obtenção de argumentos especificadas pelo enunciado (PARAM, FIX e IND).