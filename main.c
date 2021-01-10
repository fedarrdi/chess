#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include "a.h"
#include <time.h>
#include <conio.h>
#include <stdlib.h>

extern int global_evaluation;

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
    if (p.type == knight && p.color == white) return 'h';
    if (p.type == knight) return 'H';
    if (p.type == queen && p.color == white) return 'q';
    if (p.type == queen) return 'Q';
    if (p.type == bishop && p.color == white) return 'b';
    if (p.type == bishop) return 'B';
    if (p.type == rook && p.color == white) return 'r';
    if (p.type == rook) return 'R';
    return ' ';
}

void fill_board()
{
    for(int y = 0;y < SIZE;y++)
        for(int x = 0;x < SIZE;x++)
            board[y][x].type = empty;

    for(int x = 0;x < SIZE;x++)
        board[1][x].type = board[6][x].type = pawn, board[1][x].color = black, board[6][x].color = white;


    board[0][3].type =  board[7][3].type =  queen;
    board[0][3].color = black;
    board[7][3].color = white;

    board[0][4].type =  board[7][4].type =  king;
    board[0][4].color = black;
    board[7][4].color = white;

    board[0][2].type = board[0][5].type = board[7][2].type = board[7][5].type = bishop;
    board[0][2].color = board[0][5].color = black;
    board[7][2].color = board[7][5].color = white;

    board[0][1].type = board[0][6].type = board[7][1].type = board[7][6].type = knight;
    board[0][1].color = board[0][6].color = black;
    board[7][1].color = board[7][6].color = white;

    board[0][0].type = board[0][7].type = board[7][0].type = board[7][7].type = rook;
    board[0][0].color = board[0][7].color = black;
    board[7][0].color = board[7][7].color = white;

}

void print_board()
{
    printf("Coordinates are being entered with first X, and then Y!\n\n");
    printf("  X");
    for (int x = 0; x < 8; x++)
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
    printf("  Y  ");
    printf("\n"); printf("\n");
}

void move_piece(int turn)
{
    enum color color = !turn;
    struct position from, to;

    back:;
    printf("Enter cords of the piece you want to move:\n");
    scanf("%d", &from.x);
    scanf("%d", &from.y);

    if (board[from.y][from.x].type == empty || board[from.y][from.x].color != color)
    {
        printf("Wrong cords!\n");
        goto back;
    }

    struct move move = {from, from};
    if(!piece[board[from.y][from.x].type].enum_move(&from, &move))
    {
        printf("This piece has no possible moves!\n");
        goto back;
    }

    back1:;
    printf("Enter where you want to move the piece:\n");
    scanf("%d", &to.x);
    scanf("%d", &to.y);

    move.from = from;
    move.to = to;
    if(!piece[board[from.y][from.x].type].valid_move(move))
    {
        printf("This move is not valid!\n");
        goto back1;
    }

    struct undo taken;
    piece[board[from.y][from.x].type].play_move(move, &taken);
    board[from.y][from.x].type = empty;
}

int extend_depth()
{
    int figures = 0;
    for(int y = 0;y < SIZE;y++)
        for(int x = 0;x < SIZE;x++)
            if(board[y][x].type != empty)
                figures++;

    if(figures < 10) return 4;
    if(figures < 15) return 3;
    if(figures < 20) return 2;
    if(figures < 25) return 1;
    return 0;
}


enum bool find_best_move(struct move *move, int *out_eval, enum color player, int depth, int alpha, int beta);

int main()
{
    struct move move;
    struct undo undo;
    int eval = 0, depth, step = 0, alpha = (int) -1e8, beta = (int) 1e8, turn;

    system("cls");
    printf("                                                      CHESS               \n\n");
    printf("Information:\n");
    printf("Whites are playing with UPPERCASE letters, and Blacks are playing with lowercase letters.\n\n");
    printf("List of figures are:\n");
    printf("K - King\n");
    printf("Q - Queen\n");
    printf("B - Bishop\n");
    printf("H - Knight\n");
    printf("R - Rook\n");
    printf("P - Pawn\n\n\n");
    printf("Press Any Key To Continue...");
    getch();
    system("cls");

    printf("Please choose your level:\n");
    printf("0 - New to Chess\n");
    printf("1 - Beginner\n");
    printf("3 - Intermediate\n");
    printf("4 - Grandmaster / Advanced\n\n\n");
    Enter:
    printf("Enter: ");

    scanf("%d", &depth);

    if (depth < 0 || depth > 5)
    {
        printf("\nInvalid choice, please choose again!\n\n\n");
        goto Enter;
    }

    system("cls");

    printf("Please choose piece color\n");
    printf("1 - Black\n");
    printf("0 - White\n");

    scanf("%d", &turn);

    fill_board();
    while(1)
    {
        depth += extend_depth();

        if(step % 2 == 1)
        {
            if(global_evaluation >= 1e6)
            {
                printf("Black wins!\n");
                break;
            }
            printf("Black\n");
        }
        else
        {
            if(global_evaluation <= -1e6)
            {
                printf("Black wins!\n");
                break;
            }
            printf("White\n");
        }

        print_board();
        if (step % 2 == turn)
            move_piece(turn);

        else
        {
            find_best_move(&move, &eval, turn, depth, alpha, beta);
            piece[board[move.from.y][move.from.x].type].play_move(move, &undo);
        }

        timeout(2);
        system("cls");
        step++;
    }

    return 0;
}