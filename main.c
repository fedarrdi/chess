#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include "a.h"
#include <time.h>

int timeout ( int seconds )
{
    clock_t endwait;
    endwait = clock () + seconds * CLOCKS_PER_SEC ;
    while (clock() < endwait) {}

    return  1;
}


char getPiece(struct square p)
{
    if (p.type == pawn && p.color == white) return 'p';
    if (p.type == pawn) return 'P';
    if (p.type == king && p.color == white) return 'k';
    if (p.type == king) return 'K';
    return ' ';
}

void fill_board()
{
    for(int y = 0;y < SIZE;y++)
        for(int x = 0;x < SIZE;x++)
            board[y][x].type = empty;

    for(int x = 0;x < SIZE;x++)
    {
        board[0][x].type = board[7][x].type = king;
        board[0][x].color = black;
        board[7][x].color = white;

        board[1][x].type = board[6][x].type = pawn;
        board[1][x].color = black;
        board[6][x].color = white;
    }

}

void print_board()
{
    printf("   ");
    for (int x = 0; x < SIZE; x++)
        printf("   %d", x);

    printf("\n");
    for (int y = 0; y < SIZE; y++)
    {
        printf("    |");
        for (int x = 0; x < SIZE; x++)
            printf("---+");

        printf("\n");
        printf("%d ->|", y);
        for (int x = 0; x < SIZE; x++)
        {
            char a = getPiece(board[y][x]);
            printf(" %c |", a);
        }
        if (y == SIZE - 1)
        {
            printf("\n"); printf("    |");
            for (int x = 0; x < SIZE; x++)
                printf("---+");
        }
        printf("\n");
    }
    printf("\n"); printf("\n");
}

void move_piece(int turn)
{
    enum color color = !turn;
    struct position from, to;

    back:;
    printf("Enter cords of the piece you want to move\n");
    scanf("%d", &from.x);
    scanf("%d", &from.y);

    if (board[from.y][from.x].type == empty || board[from.y][from.x].color != color)
    {
        printf("Wrong cords\n");
        goto back;
    }
    struct move move = {from, from};
    if(!piece[board[from.y][from.x].type].enum_move(&from, &move))
    {
        printf("This piece has no possible moves\n");
        goto back;
    }

    back1:;
    printf("Enter were you want to move the piece\n");
    scanf("%d", &to.x);
    scanf("%d", &to.y);

    move.from = from;
    move.to = to;
    if(!piece[board[from.y][from.x].type].valid_move(move))
    {
        printf("This move is not valid\n");
        goto back1;
    }

    struct undo taken;
    piece[board[from.y][from.x].type].play_move(move, &taken);
    board[from.y][from.x].type = empty;
}

int game_over = 0, step = 0;
int find_best_move(struct move *move, int *out_eval, enum color player, int depth);

int main()
{
    struct move move;
    struct undo undo;
    int eval = 0, depth = 2;

    fill_board();
    while(!game_over) {
        print_board();
        if (step % 2)
        {
            printf("Black\n");

            if (!find_best_move(&move, &eval, black, depth))
                printf("Game Over!!!\n");

            piece[board[move.from.y][move.from.x].type].play_move(move, &undo);

            printf("%d, %d, %d, %d\n", move.from.x, move.from.y, move.to.x, move.to.y);
        }
        else
        {
            printf("White\n");

            if(!find_best_move(&move, &eval, white, depth))
                printf("Game Over!!!\n");

            piece[board[move.from.y][move.from.x].type].play_move(move, &undo);

            printf("%d, %d, %d, %d\n", move.from.x, move.from.y, move.to.x, move.to.y);

        }

        if( timeout(2) == 1 );

        step++;
    }
    return 0;
}