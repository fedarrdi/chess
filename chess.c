#include "a.h"
int mod(int a){return (a < 0) ? -a : a;}
int global_evaluation = 0;

///----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

weight rook_weight(struct position pos) { return 300; }

enum bool rook_valid_move(struct move move)
{
    return (move.to.x >= 0 && move.to.x < SIZE && move.to.y >= 0 && move.to.y < SIZE &&
            (board[move.to.y][move.to.x].type == empty || board[move.to.y][move.to.x].color != board[move.from.y][move.from.x].color) &&
            (move.to.x == move.from.x || move.to.y == move.from.y));
}

enum bool rook_enum_move(struct position *pos, struct move *move)
{
    struct position d = {pos->x - move->to.x, pos->y - move->to.y};
    static int a = 1;
    static enum bool b = false;

    move->to.x = pos->x;
    move->to.y = pos->y - a;
    if((!d.y && !d.x) || (d.y > 0 && !d.x))
    {
        a++;
        if(!b)
        {
            b = ((board[move->to.y][move->to.x].type != empty) && (board[move->to.y][move->to.x].color != board[move->from.y][move->from.x].color));
            if (rook_valid_move(*move)) return true;
        }

        b = false;
        a = 1;
        d.y = pos->y - move->to.y, d.x = pos->x - move->to.x;
    }

    move->to.x = pos->x + a;
    move->to.y = pos->y;
    if((d.y > 0 && !d.x) || (d.x < 0 && !d.y))
    {
        a++;
        if(!b)
        {
            b = ((board[move->to.y][move->to.x].type != empty) && (board[move->to.y][move->to.x].color != board[move->from.y][move->from.x].color));
            if (rook_valid_move(*move)) return true;
        }

        b = false;
        a = 1;
        d.y = pos->y - move->to.y, d.x = pos->x - move->to.x;
    }

    move->to.x = pos->x;
    move->to.y = pos->y + a;
    if((d.x < 0 && !d.y) || (!d.x && d.y < 0))
    {
        a++;
        if(!b)
        {
            b = ((board[move->to.y][move->to.x].type != empty) && (board[move->to.y][move->to.x].color != board[move->from.y][move->from.x].color));
            if (rook_valid_move(*move)) return true;
        }

        b = false;
        a = 1;
        d.y = pos->y - move->to.y, d.x = pos->x - move->to.x;
    }

    move->to.x = pos->x - a;
    move->to.y = pos->y;
    if( (!d.x && d.y < 0) || (!d.y && d.x > 0))
    {
        a++;
        if(!b)
        {
            b = ((board[move->to.y][move->to.x].type != empty) && (board[move->to.y][move->to.x].color != board[move->from.y][move->from.x].color));
            if (rook_valid_move(*move)) return true;
        }

        b = false;
        a = 1;
        d.y = pos->y - move->to.y, d.x = pos->x - move->to.x;
    }

    return false;
}

///----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
weight bishop_wight(struct position pos) { return 200; }

enum bool bishop_valid_move(struct move move)
{
    return (move.to.x >= 0 && move.to.x < SIZE && move.to.y >= 0 && move.to.y < SIZE &&
    (board[move.to.y][move.to.x].type == empty || board[move.to.y][move.to.x].color != board[move.from.y][move.from.x].color)
    && move.to.x != move.from.x && move.to.y != move.from.y && mod(move.to.x - move.from.x) == mod(move.to.y - move.from.y));
}

enum bool bishop_enum_move(struct position *pos, struct move *move)
{
    struct position d = {pos->x - move->to.x, pos->y - move->to.y};
    static int a = 1;
    static enum bool b = false;

    move->to.x = pos->x + a;
    move->to.y = pos->y - a;
    if((!d.y && !d.x) || (d.x < 0 && d.y > 0))
    {
        a++;
        if(!b)
        {
            b = ((board[move->to.y][move->to.x].type != empty) && (board[move->to.y][move->to.x].color != board[move->from.y][move->from.x].color));
            if (bishop_valid_move(*move)) return true;
        }
        b = false;
        a = 1;
        d.y = pos->y - move->to.y, d.x = pos->x - move->to.x;
    }

    move->to.x = pos->x + a;
    move->to.y = pos->y + a;
    if((d.x < 0 && d.y > 0) || (d.x < 0 && d.y < 0))
    {
        a++;
        if(!b)
        {
            b = ((board[move->to.y][move->to.x].type != empty) && (board[move->to.y][move->to.x].color != board[move->from.y][move->from.x].color));
            if (bishop_valid_move(*move)) return true;
        }

        b = false;
        a = 1;
        d.y = pos->y - move->to.y, d.x = pos->x - move->to.x;
    }

    move->to.x = pos->x - a;
    move->to.y = pos->y + a;
    if((d.x < 0 && d.y < 0) || (d.y < 0 && d.x > 0))
    {
        a++;
        if(!b)
        {
            b = ((board[move->to.y][move->to.x].type != empty) && (board[move->to.y][move->to.x].color != board[move->from.y][move->from.x].color));
            if (bishop_valid_move(*move)) return true;
        }

        b = false;
        a = 1;
        d.y = pos->y - move->to.y, d.x = pos->x - move->to.x;
    }
    move->to.x = pos->x - a;
    move->to.y = pos->y - a;
    if((d.y < 0 && d.x > 0) || (d.x > 0 && d.y > 0) && !b)
    {
        a++;
        if(!b)
        {
            b = ((board[move->to.y][move->to.x].type != empty) && (board[move->to.y][move->to.x].color != board[move->from.y][move->from.x].color));
            if (bishop_valid_move(*move)) return true;
        }

        b = false;
        a = 1;
        d.y = pos->y - move->to.y, d.x = pos->x - move->to.x;
    }
    b = false;
    a = 1;

    return false;

}

///----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

weight queen_weight(struct position pos){ return 100 + board[pos.y][pos.x].color ? (8 - pos.y)*40 :  pos.y*40; }

enum bool queen_valid_move(struct move move)
{
    return (move.to.x >= 0 && move.to.x < SIZE && move.to.y >= 0 && move.to.y < SIZE && (board[move.to.y][move.to.x].type == empty || board[move.to.y][move.to.x].color != board[move.from.y][move.from.x].color)
    && ((move.to.x == move.from.x || move.to.y == move.from.y) || (move.to.x != move.from.x && move.to.y != move.from.y && mod(move.to.x - move.from.x) == mod(move.to.y - move.from.y))));
}

enum bool queen_enum_move(struct position *pos, struct move *move)
{
    struct position d = {pos->x - move->to.x, pos->y - move->to.y};
    static int a = 1;
    static enum bool b = false;

    move->to.x = pos->x;
    move->to.y = pos->y - a;
    if((!d.y && !d.x) || (d.y > 0 && !d.x))
    {
        a++;
        if(!b)
        {
            b = ((board[move->to.y][move->to.x].type != empty) && (board[move->to.y][move->to.x].color != board[move->from.y][move->from.x].color));
            if (queen_valid_move(*move)) return true;
        }

        b = false;
        a = 1;
        d.y = pos->y - move->to.y, d.x = pos->x - move->to.x;
    }

    move->to.x = pos->x + a;
    move->to.y = pos->y - a;
    if((d.y > 0 && !d.x) || (d.x < 0 && d.y > 0))
    {
        a++;
        if(!b)
        {
            b = ((board[move->to.y][move->to.x].type != empty) && (board[move->to.y][move->to.x].color != board[move->from.y][move->from.x].color));
            if (queen_valid_move(*move)) return true;
        }

        b = false;
        a = 1;
        d.y = pos->y - move->to.y, d.x = pos->x - move->to.x;
    }

    move->to.x = pos->x + a;
    move->to.y = pos->y;
    if((d.x < 0 && d.y > 0) || (d.x < 0 && !d.y))
    {
        a++;
        if(!b)
        {
            b = ((board[move->to.y][move->to.x].type != empty) && (board[move->to.y][move->to.x].color != board[move->from.y][move->from.x].color));
            if (queen_valid_move(*move)) return true;
        }

        b = false;
        a = 1;
        d.y = pos->y - move->to.y, d.x = pos->x - move->to.x;
    }

    move->to.x = pos->x + a;
    move->to.y = pos->y + a;
    if((d.x < 0 && !d.y) || (d.x < 0 && d.y < 0))
    {
        a++;
        if(!b)
        {
            b = ((board[move->to.y][move->to.x].type != empty) && (board[move->to.y][move->to.x].color != board[move->from.y][move->from.x].color));
            if (queen_valid_move(*move)) return true;
        }

        b = false;
        a = 1;
        d.y = pos->y - move->to.y, d.x = pos->x - move->to.x;
    }

    move->to.x = pos->x;
    move->to.y = pos->y + a;
    if((d.x < 0 && d.y < 0) || (!d.x && d.y < 0))
    {
        a++;
        if(!b)
        {
            b = ((board[move->to.y][move->to.x].type != empty) && (board[move->to.y][move->to.x].color != board[move->from.y][move->from.x].color));
            if (queen_valid_move(*move)) return true;
        }

        b = false;
        a = 1;
        d.y = pos->y - move->to.y, d.x = pos->x - move->to.x;
    }

    move->to.x = pos->x - a;
    move->to.y = pos->y + a;
    if((!d.x && d.y < 0) || (d.y < 0 && d.x > 0) )
    {
        a++;
        if(!b)
        {
            b = ((board[move->to.y][move->to.x].type != empty) && (board[move->to.y][move->to.x].color != board[move->from.y][move->from.x].color));
            if (queen_valid_move(*move)) return true;
        }

        b = false;
        a = 1;
        d.y = pos->y - move->to.y, d.x = pos->x - move->to.x;
    }

    move->to.x = pos->x - a;
    move->to.y = pos->y;
    if((d.y < 0 && d.x > 0) || (!d.y && d.x > 0))
    {
        a++;
        if(!b)
        {
            b = ((board[move->to.y][move->to.x].type != empty) && (board[move->to.y][move->to.x].color != board[move->from.y][move->from.x].color));
            if (queen_valid_move(*move)) return true;
        }

        b = false;
        a = 1;
        d.y = pos->y - move->to.y, d.x = pos->x - move->to.x;
    }

    move->to.x = pos->x - a;
    move->to.y = pos->y - a;
    if((!d.y && d.x > 0) || (d.x > 0 && d.y > 0) && !b)
    {
        a++;
        if(!b)
        {
            b = ((board[move->to.y][move->to.x].type != empty) && (board[move->to.y][move->to.x].color != board[move->from.y][move->from.x].color));
            if (queen_valid_move(*move)) return true;
        }

        b = false;
        a = 1;
        d.y = pos->y - move->to.y, d.x = pos->x - move->to.x;
    }
    b = false;
    a = 1;

    return false;
}

///----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
weight pawn_weight(struct position pos)
{
    int boost = 0;
    if(pos.x == 1 || pos.x == 6) boost = 10;
    if(pos.x == 2 || pos.x == 5) boost = 20;
    if(pos.x == 3 || pos.x == 4) boost = 30;
    return 100 + (board[pos.y][pos.x].color ? 8 - pos.y : pos.y) + boost;
}

enum bool pawn_valid_move(struct move move)
{
    short t = (board[move.from.y][move.from.x].color == white) ? -1 : 1;
    short s = (board[move.from.y][move.from.x].color == black) ? 1 : 6;

    return (move.to.x >= 0 && move.to.x < SIZE && move.to.y >= 0 && move.to.y < SIZE && (((move.to.y - move.from.y)*t == 1 && move.from.x == move.to.x && board[move.to.y][move.to.x].type == empty)  ||
            ((move.to.y - move.from.y)*t == 1 && move.from.x == move.to.x + 1 && board[move.to.y][move.to.x].type != empty && board[move.to.y][move.to.x].color != board[move.from.y][move.from.x].color) ||
            ((move.to.y - move.from.y)*t == 1 && move.from.x == move.to.x - 1 && board[move.to.y][move.to.x].type != empty && board[move.to.y][move.to.x].color != board[move.from.y][move.from.x].color) ||
            ((move.to.y - move.from.y)*t == 2 && move.from.x == move.to.x && board[move.to.y][move.to.x].type == empty && move.from.y == s))) ;
}

enum bool pawn_enum_move(struct position* pos, struct move* move)
{
    short f = (board[pos->y][pos->x].color == white) ? -1 : 1;
    struct position d = {pos->x - move->to.x, pos->y - move->to.y};

    move->to.y = pos->y + 2 * f;
    move->to.x = pos->x;
    if (!d.y && !d.x) {
        if (pawn_valid_move(*move)) return true;
        d.y = pos->y - move->to.y, d.x = pos->x - move->to.x;
    }

    move->to.y = pos->y + f;
    move->to.x = pos->x;
    if (d.y == -2*f && !d.x) {
        if (pawn_valid_move(*move)) return true;
        d.y = pos->y - move->to.y, d.x = pos->x - move->to.x;
    }

    move->to.y = pos->y + f;
    move->to.x = pos->x + 1;
    if (d.y == -f && !d.x) {
        if (pawn_valid_move(*move)) return true;
        d.y = pos->y - move->to.y, d.x = pos->x - move->to.x;
    }

    move->to.y = pos->y + f;
    move->to.x = pos->x - 1;
    if (d.y == -f && d.x == -1) {
        if (pawn_valid_move(*move)) return true;
        d.y = pos->y - move->to.y, d.x = pos->x - move->to.x;
    }
    return false;
}

///----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
weight king_weight(struct position pos) { return 1e6; }

enum bool king_valid_move(struct move move)
{
    return (mod(move.to.x - move.from.x) < 2 && mod(move.to.y - move.from.y) < 2 && move.to.x >= 0 &&move.to.x < SIZE && move.to.y >= 0 &&
            move.to.y < SIZE && (board[move.to.y][move.to.x].type == empty || board[move.to.y][move.to.x].color != board[move.from.y][move.from.x].color));
}

enum bool king_enum_move(struct position *pos, struct move* move)
{
    struct position d = {pos->x - move->to.x, pos->y - move->to.y};

    move->to.x = pos->x;
    move->to.y = pos->y - 1;
    if (!d.y && !d.x)
    {
        if (king_valid_move(*move)) return true;
        d.y = pos->y - move->to.y, d.x = pos->x - move->to.x;
    }

    move->to.x = pos->x + 1;
    move->to.y = pos->y - 1;
    if (d.y == 1 && !d.x)
    {
        if (king_valid_move(*move)) return true;
        d.y = pos->y - move->to.y, d.x = pos->x - move->to.x;
    }

    move->to.x = pos->x + 1;
    move->to.y = pos->y;
    if (d.y == 1 && d.x == -1)
    {
        if (king_valid_move(*move)) return true;
        d.y = pos->y - move->to.y, d.x = pos->x - move->to.x;
    }

    move->to.x = pos->x + 1;
    move->to.y = pos->y + 1;
    if (!d.y && d.x == -1)
    {
        if (king_valid_move(*move)) return true;
        d.y = pos->y - move->to.y, d.x = pos->x - move->to.x;
    }

    move->to.x = pos->x ;
    move->to.y = pos->y + 1;
    if (d.y == -1 && d.x == -1)
    {
        if (king_valid_move(*move)) return true;
        d.y = pos->y - move->to.y, d.x = pos->x - move->to.x;
    }

    move->to.x = pos->x - 1;
    move->to.y = pos->y + 1;
    if (d.y == -1 && !d.x)
    {
        if (king_valid_move(*move)) return true;
        d.y = pos->y - move->to.y, d.x = pos->x - move->to.x;
    }

    move->to.x = pos->x - 1;
    move->to.y = pos->y;
    if (d.y == -1 && d.x == 1)
    {
        if (king_valid_move(*move)) return true;
        d.y = pos->y - move->to.y, d.x = pos->x - move->to.x;
    }

    move->to.x = pos->x - 1;
    move->to.y = pos->y - 1;
    if (!d.y && d.x == 1)
    {
        if (king_valid_move(*move)) return true;
        d.y = pos->y - move->to.y, d.x = pos->x - move->to.x;
    }

    return false;
}

///----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
weight knight_weight(struct position pos) { return 200; }

enum bool knight_valid_move(struct move move)
{
    return (move.to.x >= 0 && move.to.x < SIZE && move.to.y >= 0 &&
            move.to.y < SIZE && (board[move.to.y][move.to.x].type == empty || board[move.to.y][move.to.x].color != board[move.from.y][move.from.x].color));
}

enum bool knight_enum_move(struct position* pos, struct move* move)
{
    struct position d = { pos->x - move->to.x, pos->y - move->to.y };

    move->to.x = pos->x + 1;
    move->to.y = pos->y - 2;
    if (!d.y && !d.x)
    {
        if (knight_valid_move(*move)) return true;
        d.y = pos->y - move->to.y, d.x = pos->x - move->to.x;
    }

    move->to.x = pos->x + 2;
    move->to.y = pos->y - 1;
    if (d.y == 2 && d.x == -1)
    {
        if (knight_valid_move(*move)) return true;
        d.y = pos->y - move->to.y, d.x = pos->x - move->to.x;
    }

    move->to.x = pos->x + 2;
    move->to.y = pos->y + 1;
    if (d.y == 1 && d.x == -2)
    {
        if (knight_valid_move(*move)) return true;
        d.y = pos->y - move->to.y, d.x = pos->x - move->to.x;
    }

    move->to.x = pos->x + 1;
    move->to.y = pos->y + 2;
    if (d.y == -1 && d.x == -2)
    {
        if (knight_valid_move(*move)) return true;
        d.y = pos->y - move->to.y, d.x = pos->x - move->to.x;
    }

    move->to.x = pos->x - 1;
    move->to.y = pos->y + 2;
    if (d.y == -2 && d.x == -1)
    {
        if (knight_valid_move(*move)) return true;
        d.y = pos->y - move->to.y, d.x = pos->x - move->to.x;
    }

    move->to.x = pos->x - 2;
    move->to.y = pos->y + 1;
    if (d.y == -2 && d.x == 1)
    {
        if (knight_valid_move(*move)) return true;
        d.y = pos->y - move->to.y, d.x = pos->x - move->to.x;
    }

    move->to.x = pos->x - 2;
    move->to.y = pos->y - 1;
    if (d.y == -1 && d.x == 2)
    {
        if (knight_valid_move(*move)) return true;
        d.y = pos->y - move->to.y, d.x = pos->x - move->to.x;
    }

    move->to.x = pos->x - 1;
    move->to.y = pos->y - 2 ;
    if (d.y == 1 && d.x == 2)
    {
        if (knight_valid_move(*move)) return true;
        d.y = pos->y - move->to.y, d.x = pos->x - move->to.x;
    }

    return false;
}


///----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
weight empty_weight(struct position pos) { return 0; }

enum bool empty_valid_move(struct move move) { return false; }

enum bool empty_enum_move(struct position *pos, struct move *move) {return false;}

void empty_play_move(struct move move, struct undo *taken) {  }


///----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void undo_move(struct move move, struct undo undo)
{
    struct square *from = &board[move.from.y][move.from.x], *to = &board[move.to.y][move.to.x];
    if (to->color == white)
        global_evaluation -= piece[undo.taken].weight(move.to);
    else
        global_evaluation += piece[undo.taken].weight(move.to);
    *from=*to;
    to->type = undo.taken;
    to->color = !from->color;
}

void generic_play_move(struct move move, struct undo *undo)
{
    undo->position = move.to;
    struct square *from = &board[move.from.y][move.from.x], *to = &board[move.to.y][move.to.x];
    undo->taken = to->type;
    *to = *from;
    from->type = empty;
    if(to->color == white)
        global_evaluation += piece[undo->taken].weight(move.to);
    else
        global_evaluation -= piece[undo->taken].weight(move.to);
}

struct piece piece[]=
        {
                { empty_weight, empty_valid_move, empty_play_move, empty_enum_move },
                { pawn_weight, pawn_valid_move, generic_play_move, pawn_enum_move },
                { knight_weight, knight_valid_move, generic_play_move, knight_enum_move },
                { bishop_wight, bishop_valid_move, generic_play_move, bishop_enum_move},
                { rook_weight, rook_valid_move, generic_play_move, rook_enum_move},
                { queen_weight, queen_valid_move, generic_play_move, queen_enum_move },
                { king_weight, king_valid_move, generic_play_move, king_enum_move }
        };