/* Leonardo Dana Edelsberg 2510515 3WC */

#include "../include/cria_func.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

static void emit8(unsigned char *c, int *p, unsigned char v)
{
    c[(*p)++] = v;
}

static void emit64(unsigned char *c, int *p, uint64_t v)
{
    memcpy(c + *p, &v, 8);
    *p += 8;
}

static void mov_imm64_rax(unsigned char *c, int *p, uint64_t val)
{
    emit8(c,p,0x48);
    emit8(c,p,0xB8);
    emit64(c,p,val);
}

static void mov_imm64_rdi(unsigned char *c, int *p, uint64_t val)
{
    emit8(c,p,0x48);
    emit8(c,p,0xBF);
    emit64(c,p,val);
}

static void mov_imm64_rsi(unsigned char *c, int *p, uint64_t val)
{
    emit8(c,p,0x48);
    emit8(c,p,0xBE);
    emit64(c,p,val);
}

static void mov_imm64_rdx(unsigned char *c, int *p, uint64_t val)
{
    emit8(c,p,0x48);
    emit8(c,p,0xBA);
    emit64(c,p,val);
}

static void save_received(unsigned char *c, int *p)
{
    emit8(c,p,0x49);
    emit8(c,p,0x89);
    emit8(c,p,0xFA);

    emit8(c,p,0x49);
    emit8(c,p,0x89);
    emit8(c,p,0xF3);

    emit8(c,p,0x49);
    emit8(c,p,0x89);
    emit8(c,p,0xD0);
}

static void r10_to_rdi(unsigned char *c, int *p)
{
    emit8(c,p,0x4C);
    emit8(c,p,0x89);
    emit8(c,p,0xD7);
}

static void r10_to_rsi(unsigned char *c, int *p)
{
    emit8(c,p,0x4C);
    emit8(c,p,0x89);
    emit8(c,p,0xD6);
}

static void r10_to_rdx(unsigned char *c, int *p)
{
    emit8(c,p,0x4C);
    emit8(c,p,0x89);
    emit8(c,p,0xD2);
}

static void r11_to_rsi(unsigned char *c, int *p)
{
    emit8(c,p,0x4C);
    emit8(c,p,0x89);
    emit8(c,p,0xDE);
}

static void r11_to_rdi(unsigned char *c, int *p)
{
    emit8(c,p,0x4C);
    emit8(c,p,0x89);
    emit8(c,p,0xDF);
}

static void r11_to_rdx(unsigned char *c, int *p)
{
    emit8(c,p,0x4C);
    emit8(c,p,0x89);
    emit8(c,p,0xDA);
}

static void r8_to_rdi(unsigned char *c,int *p)
{
    emit8(c,p,0x4C);
    emit8(c,p,0x89);
    emit8(c,p,0xC7);
}

static void r8_to_rsi(unsigned char *c,int *p)
{
    emit8(c,p,0x4C);
    emit8(c,p,0x89);
    emit8(c,p,0xC6);
}

static void r8_to_rdx(unsigned char *c,int *p)
{
    emit8(c,p,0x4C);
    emit8(c,p,0x89);
    emit8(c,p,0xC2);
}

static void load_ind_int_rdi(unsigned char *c,int *p,void *addr)
{
    mov_imm64_rax(c,p,(uint64_t)addr);

    emit8(c,p,0x8B);
    emit8(c,p,0x38);
}

static void load_ind_int_rsi(unsigned char *c,int *p,void *addr)
{
    mov_imm64_rax(c,p,(uint64_t)addr);

    emit8(c,p,0x8B);
    emit8(c,p,0x30);
}

static void load_ind_int_rdx(unsigned char *c,int *p,void *addr)
{
    mov_imm64_rax(c,p,(uint64_t)addr);

    emit8(c,p,0x8B);
    emit8(c,p,0x10);
}

static void load_ind_ptr_rdi(unsigned char *c,int *p,void *addr)
{
    mov_imm64_rax(c,p,(uint64_t)addr);

    emit8(c,p,0x48);
    emit8(c,p,0x8B);
    emit8(c,p,0x38);
}

static void load_ind_ptr_rsi(unsigned char *c,int *p,void *addr)
{
    mov_imm64_rax(c,p,(uint64_t)addr);

    emit8(c,p,0x48);
    emit8(c,p,0x8B);
    emit8(c,p,0x30);
}

static void load_ind_ptr_rdx(unsigned char *c,int *p,void *addr)
{
    mov_imm64_rax(c,p,(uint64_t)addr);

    emit8(c,p,0x48);
    emit8(c,p,0x8B);
    emit8(c,p,0x10);
}

void cria_func(void *f, DescParam params[], int n, unsigned char codigo[])
{

    if (n < 1 || n > 3){
        printf("A função posssui mais de 3 parametros. Não pode ser tratada.\n");
        return;
    }

    int p = 0;

    emit8(codigo,&p,0x55);

    emit8(codigo,&p,0x48);
    emit8(codigo,&p,0x89);
    emit8(codigo,&p,0xE5);

    save_received(codigo,&p);

    int param_idx = 0;

    for(int i=0;i<n;i++)
    {
        DescParam *dp = &params[i];

        if(i==0)
        {
            if(dp->orig_val == PARAM)
            {
                if(param_idx==0) r10_to_rdi(codigo,&p);
                else if(param_idx==1) r11_to_rdi(codigo,&p);
                else r8_to_rdi(codigo,&p);

                param_idx++;
            }
            else if(dp->orig_val == FIX)
            {
                if(dp->tipo_val == INT_PAR)
                    mov_imm64_rdi(codigo,&p,(uint64_t)(int64_t)dp->valor.v_int);
                else
                    mov_imm64_rdi(codigo,&p,(uint64_t)dp->valor.v_ptr);
            }
            else
            {
                if(dp->tipo_val == INT_PAR)
                    load_ind_int_rdi(codigo,&p,dp->valor.v_ptr);
                else
                    load_ind_ptr_rdi(codigo,&p,dp->valor.v_ptr);
            }
        }

        else if(i==1)
        {
            if(dp->orig_val == PARAM)
            {
                if(param_idx==0) r10_to_rsi(codigo,&p);
                else if(param_idx==1) r11_to_rsi(codigo,&p);
                else r8_to_rsi(codigo,&p);

                param_idx++;
            }
            else if(dp->orig_val == FIX)
            {
                if(dp->tipo_val == INT_PAR)
                    mov_imm64_rsi(codigo,&p,(uint64_t)(int64_t)dp->valor.v_int);
                else
                    mov_imm64_rsi(codigo,&p,(uint64_t)dp->valor.v_ptr);
            }
            else
            {
                if(dp->tipo_val == INT_PAR)
                    load_ind_int_rsi(codigo,&p,dp->valor.v_ptr);
                else
                    load_ind_ptr_rsi(codigo,&p,dp->valor.v_ptr);
            }
        }

        else if(i==2)
        {
            if(dp->orig_val == PARAM)
            {
                if(param_idx==0) r10_to_rdx(codigo,&p);
                else if(param_idx==1) r11_to_rdx(codigo,&p);
                else r8_to_rdx(codigo,&p);

                param_idx++;
            }
            else if(dp->orig_val == FIX)
            {
                if(dp->tipo_val == INT_PAR)
                    mov_imm64_rdx(codigo,&p,(uint64_t)(int64_t)dp->valor.v_int);
                else
                    mov_imm64_rdx(codigo,&p,(uint64_t)dp->valor.v_ptr);
            }   
            else
            {
                if(dp->tipo_val == INT_PAR)
                    load_ind_int_rdx(codigo,&p,dp->valor.v_ptr);
                else
                    load_ind_ptr_rdx(codigo,&p,dp->valor.v_ptr);
            }
        }
    }

    mov_imm64_rax(codigo,&p,(uint64_t)f);

    emit8(codigo,&p,0xFF);
    emit8(codigo,&p,0xD0);

    emit8(codigo,&p,0xC9);

    emit8(codigo,&p,0xC3);
}