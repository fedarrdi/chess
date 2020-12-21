#include "a.h"

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

int find_best_move(struct move *move, int *out_eval, enum color player, int depth)
{
   
}
