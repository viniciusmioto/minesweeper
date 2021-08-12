/* Vinícius Mioto, GRR20203931, gcc mines.c 
 * Para compilar tudo e jogar: gcc -o mines mines.c pilha.c pilha.h && ./mines*/
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include "pilha.h"

/* Nesse jogo as bombas são *'s e remetem ao jogo KMines. Posições não abertas são #'s */
#define BOMB -1
#define SAFE 0
#define WIN 1
#define LOSE 2
#define PLAYING 3
#define EDGE -2
#define MAX 50
#define SYMB_BOMB '*'
#define SYMB_SAFE ' '
#define SYMB_HIDDEN '#'

/* Estrutura "quadrado" possui a informação do conteúdo (bomb, safe e edge) e também se está aberta ou fechada (revealed) */
struct square
{
    int info;
    bool revealed;
};

/* Estrutura "campo" possui uma matrix que é um vetor de vetores do tipo "quadrado" */
struct field
{
    int x, y, total_bombs, num_closed, status;
    struct square matrix[MAX][MAX];
};

/* Sorteia número aleatório para as jogadas, obedecendo o critério dos parâmetros MIN e MAX do "campo" */
int random_coord(int min, int max)
{
  return min + (rand() % (max - min + 1));
}

/* Recebe as coordenas X e Y, retorna verdadeiro se uma posição for uma borda, e falso caso contrário.
Uma borda é uma posição da "matrix" na qual não é possível executar jogadas ou plantar bombas */
bool is_edge(int x, int y, struct field *my_field)
{
    return ((x == 0) || (y == 0) || (x == my_field->x + 1) || (y == my_field->y + 1));
}

/* Utilizado na inicialização do jogo, remove possível lixo da memória, preenche as informações dos quadrados */
void clear_field(struct field *my_field)
{
    int i, j;
    for (i = 0; i <= my_field->x + 1; i++)
        for (j = 0; j <= my_field->y + 1; j++) 
        {
            if (is_edge(i, j, my_field))
                my_field->matrix[i][j].info = EDGE;
            else
                my_field->matrix[i][j].info = SAFE;
            my_field->matrix[i][j].revealed = false;
        }
}

/* Abre todas as posições do campo */
void reveal_all(struct field *my_field)
{
    int i, j;
    printf("REVELOU TUDO\n");
    for (i = 1; i <= my_field->x; i++)
        for (j = 1; j <= my_field->y; j++)
            my_field->matrix[i][j].revealed = true;
}

/* Preenche a quantidade de bombas solicitadas pelo jogador em posições aleatórias */
void plant_bombs(struct field *my_field)
{
    int i, x, y;
    for (i = 0; i < my_field->total_bombs; i++) {
        do 
        {
            x = random_coord(1, my_field->x);
            y = random_coord(1, my_field->y);
        } while (my_field->matrix[x][y].info != SAFE);
        my_field->matrix[x][y].info = BOMB;
    }
}

/* Recebe as coordenadas X e Y, e retorna o número de vizinhos que contém bomba. Essa função é acionada apenas se a posição em questão NÃO tiver bomba.
Por vizinhos entende-se os "quadrados" que estão em contato com (x, y) */
int num_neighbor_bombs(struct field *my_field, int x, int y)
{
    int bombs, i, j;
    bombs = 0;
    for (i = x - 1; i <= x + 1; i++)
        for (j = y - 1; j <= y + 1; j++)
            if (my_field->matrix[i][j].info == BOMB)
                bombs++;
    return bombs;
}

/* Preenche os quadrados que NÃO possuem bombas com a quantidade de bombas dos vizinhos */
void update_neighbor_info(struct field *my_field)
{
    int i, j;
    for (i = 1; i <= my_field->x; i++)
        for (j = 1; j <= my_field->y; j++)
            if (my_field->matrix[i][j].info == SAFE)
                my_field->matrix[i][j].info = num_neighbor_bombs(my_field, i, j); 
}

/* Limpa a tela a cada vez que é acinado e atualiza os valores para cada posição*/
void show_field(struct field *my_field)
{
    int i, j;
    printf("\e[1;1H\e[2J");
    /* printf("\nFaltam: %d posições\n", my_field->num_closed); */
    printf("     ");
    for (i = 1; i <= my_field->x; i++)
        printf("%3d ", i);
    printf("\n   +");
    for (i = 1; i <= 2 * my_field->y; i++)
        printf("--");
    printf("\n");
    for (i = 1; i <= my_field->x; i++) 
    {
        printf("%3d| ", i);
        for (j = 1; j <= my_field->y; j++) 
        {
            if (!my_field->matrix[i][j].revealed)
                printf("%3c ", SYMB_HIDDEN);
            else 
            {
                switch (my_field->matrix[i][j].info) 
                {
                    case BOMB:
                        printf("%3c ", SYMB_BOMB);
                        break;
                    case SAFE:
                        printf("%3c ", SYMB_SAFE);
                        break;
                    default:
                        printf("%3d ", my_field->matrix[i][j].info);
                        break;
                }
            }
        }
        printf("\n");
    }
    printf("\n");
}

/* Recebe o número de linhas, colunas e bombas. Verifica se estão de acordo com os critérios de bombas e tamanho do compo. */
bool verify_field_info(int *lines, int *columns, int *bombs)
{
    printf("Informe o número de linhas (MAX: %d): ", MAX);
    scanf("%d", lines);
    
    if (*lines > MAX || *lines <= 0)
        return false;

    printf("Informe o número de colunas (MAX: %d): ", MAX);
    scanf("%d", columns);
    
    if (*columns > MAX || *columns <= 0)
        return false;

    printf("Informe o número de bombas (entre 1 e %d): ", *lines * *columns);
    scanf("%d", bombs);

    if (*bombs > *lines * *columns || *bombs <= 0)
        return false;

    return true;
}

/* Recebe as informações do campo, se essas estiverem OK, aciona o "limpa_campo" e "planta_bombas" */
void init_field(struct field *my_field, int lines, int columns, int bombs)
{
    my_field->x = lines;
    my_field->y = columns;
    my_field->total_bombs = bombs;
    my_field->num_closed = (my_field->x * my_field->y) - my_field->total_bombs;
    my_field->status = PLAYING;
    clear_field(my_field);
    plant_bombs(my_field);
    update_neighbor_info(my_field);
}

/* Abre as posições que NÃO possuem bombas, com o uso de uma pilha de coordenadas. Retorna o número de quadrados abertos */
int show_neighbors(struct field *my_field, struct stack *stk)
{
    struct coordinate coord, neighbor;
    int opened, i, j;
    opened = 1;

    while(!is_empty(stk))
    {
        coord = unstacking(stk);
        for (i = coord.x - 1; i <= coord.x + 1; i++)
            for (j = coord.y - 1; j <= coord.y + 1; j++)
            {
                if (!my_field->matrix[i][j].revealed && my_field->matrix[i][j].info == SAFE && !is_edge(i, j, my_field))
                {
                    neighbor.x = i;
                    neighbor.y = j;
                    stacking(neighbor, stk);
                    my_field->matrix[i][j].revealed = true;
                    opened++;
                } 
                else 
                    if (!is_edge(i, j, my_field) && !my_field->matrix[i][j].revealed)
                    {
                        my_field->matrix[i][j].revealed = true;
                        opened++;
                    }
            }
    }
    return opened;
}

/* Recebe as coordenadas X e Y. Permite que as jogadas sejam executas apenas em quadrados "novos" */
void read_move(int *x, int *y, struct field *my_field)
{
    do
    {
        printf("Informe a linha: ");
        scanf("%d", x);
        printf("Informe a coluna: ");
        scanf("%d", y);
    } while (my_field->matrix[*x][*y].revealed); /* verifica se o quadrado é "novo" */
}

/* Recebe as coordenada X e Y, e realiza o a atualização do campo*/
void make_move(int x, int y, struct field *my_field) 
{
    struct coordinate coord;
    struct stack stk;
    int opened;

    /* Se a posição escolhida contém uma bomba o jogo é parado e usuário irá visualizar o campo com todas as bombas plantadas */
    if (my_field->matrix[x][y].info == BOMB)
    {
        reveal_all(my_field);
        my_field->status = LOSE;
    }
    else
    {
        /* Se não for bomba atualiza a info do quadrado e abre os vizinhos (quando aplicável) */
        opened = 1;
        my_field->matrix[x][y].revealed = true;
        if (my_field->matrix[x][y].info == SAFE)
        {
            coord.x = x;
            coord.y = y;
            init_stack(&stk, MAX*MAX);
            stacking(coord, &stk);
            opened = show_neighbors(my_field, &stk); /* Número de vizinhos abertos */
        }
        /* Quando o jogador atinge o número de posições definidas, ganha o jogo */
        my_field->num_closed -= opened;
        if (my_field->num_closed == 0) {
            my_field->status = WIN;
            reveal_all(my_field);
        }
    }
}

/* Programa principal */
int main()
{
    struct field mine_field;
    int x, y, lines, columns, bombs;

    srand(time(NULL));

    /* Verifica as infos do campo. Se não estiverem de acordo o jogo será encerrado! */
    if (!verify_field_info(&lines, &columns, &bombs))
        return 1;

    init_field(&mine_field, lines, columns, bombs);
    show_field(&mine_field);

    do
    {
      read_move(&x, &y, &mine_field);
      make_move(x, y, &mine_field);
      show_field(&mine_field);
    } while (mine_field.status == PLAYING);

    if (mine_field.status == WIN)
      printf("PARABÉNS \nVocê venceu!!!\n");
    else
      printf("**BOOM** \nVocê perdeu :(\n");
    return 0;
}
