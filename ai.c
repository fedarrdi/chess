#include "a.h"
#include<stdio.h>
extern int global_evaluation;

void print_board();

int timeout ( int seconds );

void undo_move(struct move move, struct undo undo);

enum bool enum_board(enum color player, struct move *move)
{
    for(;move->from.y < SIZE;move->from.y++)
    {
        for(;move->from.x < SIZE;move->from.x++)
        {
            if(move->to.x == - 1)
                move->to = move->from;

            if (board[move->from.y][move->from.x].color == player && piece[board[move->from.y][move->from.x].type].enum_move(&move->from, move))
                return true;

            move->to.x = -1;
        }
    move->from.x = 0;
    }
    move->to.x = 0;
    return false;
}

enum bool find_best_move(struct move *move, int *out_eval, enum color player, int depth, int alpha, int beta)
{
    struct move curr_move = {{0, 0}, {-1, 0}}, best_move = {{0, 0}, {-1, 0}};
    struct undo taken;
    int best_eval = global_evaluation;

    while(enum_board(player, &curr_move))
    {
        piece[board[curr_move.from.y][curr_move.from.x].type].play_move(curr_move, &taken);

        int curr_eval = global_evaluation;

        if(depth) find_best_move(move, &curr_eval, !player, depth - 1, alpha, beta);

        if(curr_eval < -1e6 || curr_eval > 1e6) return false;

        if((!player && best_eval >= curr_eval) || (player && best_eval <= curr_eval))
        {
            (player) ? (alpha = curr_eval) : (beta = curr_eval);
            best_eval = curr_eval;
            best_move = curr_move;
        }
        if(beta <= alpha) break;

        undo_move(curr_move, taken);

    }
    if(curr_move.to.x == -1) return false;

    if(best_move.to.x == -1) best_move = curr_move;

    *move = best_move;
    *out_eval = best_eval;
    return true;
}