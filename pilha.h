/* Vinícius Mioto, GRR20203931, gcc pilha.h */
#include <stdbool.h>
struct coordinate 
{
    int x, y;
} coordinate;

struct stack 
{
    int top, size;
    struct coordinate *coords;
} stack;

void init_stack(struct stack *stk, int size);
bool is_empty(struct stack *stk);
bool one_element(struct stack *stk);
void stacking (struct coordinate coord, struct stack* stk);
struct coordinate unstacking(struct stack *stk);
struct coordinate top (struct stack* stk);

/*
Para esse trabalho, em específico, fiz questão de não criar os aliases para coordinate e stack, para fins de aprendizado e coompreensão dessas estruturas. Apesar da utilização dos aliases poupar digitação, a sintaxe pode confundir um pouco com a P.O.O. do Java, por exemplo. Então optei por deixar explícito os structs.
*/
