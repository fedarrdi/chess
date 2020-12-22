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

void get_best_move(struct move *best_move, int *best_eval, struct move curr_move, int curr_eval, enum color player)
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
enum bool checkmate(enum color player)
{
    for(int y = 0;y < SIZE;y++)
    {
        for(int x = 0;x < SIZE;x++)
        {
            if(board[y][x].type == king && board[y][x].color == !player)
            {
                struct position pos = {y, x};
                struct move move = {pos, pos};
                if(!piece[board[y][x].type].enum_move(&pos, &move)) return true;
            }
        }
    }
    return false;
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

            if(!checkmate(!player))
            {
                global_evaluation = ~0;
                break;
            }

            if(depth) find_best_move(move, out_eval, !player, depth - 1);

            get_best_move(&best_move, &best_eval, curr_move, curr_eval, player);

            undo_move(curr_move, taken);
        }
    }
    if(global_evaluation == ~0) return 0;

    *move = best_move;
    *out_eval = best_eval;
    return 1;
}