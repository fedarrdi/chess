#include "a.h"

extern int global_evaluation;

void undo_move(struct move move, struct undo undo);


enum bool enum_board(enum bool start, struct move *move) // enumerates all squares and calls wach square enumerate
{
    for(struct position pos = move->from;pos.y < SIZE;pos.y++)
    {
        for (;pos.x < SIZE; pos.x++)
        {
            move->from = pos;
            if (piece[board[move->from.y][move->from.x].type].enum_move(&move->from, move))
                return true;
        }
    }
    return  false;
}

void select_move(struct move *best_move, int *best_eval, struct move curr_move, int curr_eval, enum color player)
{
    if(player == black)
    {
        if(*best_eval > curr_eval)
        {
            *best_eval = curr_eval;
            *best_move = curr_move;
        }
    }
    if(player == white)
    {
        if(*best_eval < curr_eval)
        {
            *best_eval = curr_eval;
            *best_move = curr_move;
        }
    }
}


 int find_best_move(struct move *move, int *out_eval, enum color player, int depth)
{
    struct move curr_move = {0, 0}, best_move;
    struct undo taken;
    int best_eval = global_evaluation;

    while(enum_board(0, &curr_move))
    {
        if(board[move->from.y][move->from.x].color == player)
        {
            piece[board[move->from.y][move->from.x].type].play_move(curr_move, &taken);

            int curr_eval = global_evaluation;

            if(depth) find_best_move(move, out_eval, !player, depth - 1);

            select_move(&best_move, &best_eval, curr_move, curr_eval, player);

            undo_move(curr_move, taken);
        }
    }
    *move = best_move;
    *out_eval = best_eval;
    return 1;
}