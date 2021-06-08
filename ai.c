#include "a.h"
extern long long global_evaluation;
extern int move_cnt;

void undo_move(struct move *move, struct undo *undo, const int *undo_eval);

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
    long long  best_eval = (player == white) ? -1e10 : 1e10;
    int undo_eval;

    while(enum_board(player, &curr_move))
    {
        move_cnt++;
        piece[board[curr_move.from.y][curr_move.from.x].type].play_move(&curr_move, &taken, &undo_eval);

        int curr_eval = global_evaluation;

        if(depth) find_best_move(move, &curr_eval, !player, depth - 1, alpha, beta);

        if((player == black && best_eval >= curr_eval) || (player == white && best_eval <= curr_eval))
        {
            (player) ? (alpha = curr_eval) : (beta = curr_eval);
            best_eval = curr_eval;
            best_move = curr_move;
        }

        undo_move(&curr_move, &taken, &undo_eval);
        move_cnt--;

        if(beta <= alpha) break;
    }

    if(curr_move.to.x == -1)
        return false;

    *move = best_move;
    *out_eval = best_eval;
    return true;
}