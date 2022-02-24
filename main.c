#include<stdio.h>
#include "a.h"
#include <time.h>
#include <stdlib.h>

long long global_evaluation = 0;
struct square board[8][8];
extern struct piece piece[7];
int move_cnt = 0;

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
        {
            board[y][x].type = empty;
            board[y][x].has_been_moved = 0;
            board[y][x].moves_after_promotion = -1;
        }

    ///two rook endgame
    /*board[0][7].type = king;
    board[0][0].type = rook;
    board[0][1].type = rook;
    board[4][4].type = king;
    board[4][4].color = white;*/

    ///rook and king VS rook and king
    /*board[1][1].type = rook;
    board[2][1].type = king;
    board[5][5].type = king;
    board[5][5].color = white;
    board[6][1].type = rook;
    board[6][1].color = white;*/

    /*for(int x = 0;x < SIZE;x++)
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
    board[7][0].color = board[7][7].color = white;*/

    board[1][6].type = board[1][7].type = pawn;
    board[1][7].color = white;
}

void print_board()
{
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

void move_piece(enum color color)
{
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
    int undo_move;
    piece[board[from.y][from.x].type].play_move(&move, &taken, &undo_move, 1);
    board[from.y][from.x].type = empty;
}

int timeout ( int seconds )
{
    clock_t endwait;
    endwait = clock () + seconds * CLOCKS_PER_SEC ;
    while (clock() < endwait) {}
    return  1;
}

enum bool find_best_move(struct move *move, int *out_eval, enum color player, int depth, int alpha, int beta);

int main()
{
    struct move move;
    struct undo undo;
    int eval = 0, depth, mode, alpha = -1e8, beta = 1e8, turn;

    system("cls");
    printf("                                                      CHESS               \n\n"
           "Information:\n"
           "The figures that you will be playing with will always be small letters.\n\n"
           "List of figures are:\n"
           "k - King\n"
           "q - Queen\n"
           "b - Bishop\n"
           "h - Knight\n"
           "r - Rook\n"
           "p - Pawn\n\n\n"
           "Press Any Key To Continue...");

    printf("Please choose your level:\n"
           "0  - New to Chess\n"
           "2 - Beginner\n"
           "4 - Intermediate\n"
           "6 - Grandmaster / Advanced\n\n\n");
    Enter:;
    printf("Enter: ");

    scanf("%d", &depth);

    turn1:;
    printf("Please choose piece color\n"
           "0 - Black\n"
           "1 - White\n");

    scanf("%d", &turn);

    if (turn != 0 && turn != 1)
    {
        printf("Invalid color, please choose again!\n");
        goto turn1;
    }

    mode_try:;

    printf("Enter mode: \n 1 for bot vs you \n 2 for bot vs bot \n");
    scanf("%d", &mode);

    if(mode != 1 && mode != 2)
    {
        printf("Invalid mode, please choose again!\n");
        goto mode_try;
    }

    fill_board();
    if(mode == 1)
    {
        while (true)
        {
            if (global_evaluation >= 1e6)
            {
                printf("White wins!!!\n");
                break;
            }

            if (global_evaluation <= -1e6)
            {
                printf("Black wins!!!\n");
                break;
            }

            printf(turn ? "White\n" : "Black\n");
            print_board();

            if (turn)
                move_piece(turn);

            else
            {
                int undo_eval;
                find_best_move(&move, &eval, turn, depth, alpha, beta);
                piece[board[move.from.y][move.from.x].type].play_move(&move, &undo, &undo_eval, 0);
                printf("%d, %d , %d, %d", move.from.x, move.from.y, move.to.x, move.to.y);
            }
            printf("depth = %d\n", depth);
            move_cnt++;
            turn = !turn;
        }
    }
    if(mode == 2)
    {
        while (1)
        {
            if (global_evaluation >= 1e6)
            {
                printf("White wins!!!\n");
                break;
            }

            if (global_evaluation <= -1e6)
            {
                printf("Black wins!!!\n");
                break;
            }

            printf(turn ? "White\n" : "Black\n");

            print_board();
            find_best_move(&move, &eval, turn, depth, alpha, beta);
            int undo_eval;
            piece[board[move.from.y][move.from.x].type].play_move(&move, &undo, &undo_eval, 0);
            printf("global evaluation = %d \n", global_evaluation);
            printf("%d, %d, %d, %d\n", move.from.x, move.from.y, move.to.x, move.to.y);
            move_cnt++;

            turn = !turn;
            timeout(2);
        }
    }
    return 0;
}