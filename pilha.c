/* Vinícius Mioto, GRR20203931, gcc pilha.c */

#include <limits.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "pilha.h"

/* Inicializa a pilha, removendo o lixo */
void init_stack (struct stack *stk, int size)
{
    stk->top = -1;
    stk->size = size;
    stk->coords = (struct coordinate *)malloc(stk->size * sizeof(coordinate));
}

/* Retorna TRUE se uma pilha não possui elementos e FALSE caso contrário */
bool is_empty (struct stack *stk)
{
    return stk->top == -1;
}

/* Retorna TRUE se uma pilha possui apenas 1 elemento e FALSE caso contrário */
bool one_element (struct stack *stk)
{
    return stk->top == 0;
}

/* Recebe a coordenada (struct) e insere no topo da pilha (stk) */
void stacking (struct coordinate coord, struct stack* stk)
{
    stk->top++;
    stk->coords[stk->top] = coord;
}

/* Remove o elemento do topo e retorna as coordenadas da posição */
struct coordinate unstacking (struct stack *stk)
{
    if (!is_empty(stk))
    {
        stk->top--;
        return stk->coords[stk->top + 1];
    }
    else 
    {
        struct coordinate coords;
        coords.x = 0;
        coords.y = 0;
        return coords;
    }
}

/* Retorna a coordenada do Topo */
struct coordinate top (struct stack* stk)
{
    return stk->coords[stk->top];
}