#include "a.h"

extern long long global_evaluation;
extern int move_cnt;

int mod(int a);
int max(int a, int b){ return (a > b) ? a : b;}
int min(int a, int b){ return (a < b) ? a : b;}
int pow(int times, int v)
{
    int value = 1;
    for(int i = 1; i <= times; i++, value *= v);
    return value;
}

float game_weight()
{
    float weight = 0;
    for(struct position pos = {0, 0};pos.y < SIZE;pos.y++)
        for(;pos.x < SIZE;pos.x++)
            weight += board[pos.y][pos.x].type;
    /**
     total sum of piece weight = 2 007 992
     squash it to number between 1 and 0 1 = 2 007 992
     1 thousand different numbers
     2 007 992 / 1000 ~= 2007
     => 2 007 992 / 2007 while > 1 = a number between 1 and 0 scalable for chess weight
    */
    while(weight >= 1)
        weight /= 2007;
    return  weight;
}

struct position find_piece(enum type piece, enum color color)
{
    for(struct position pos = {0, 0};pos.y < SIZE;pos.y++)
        for(;pos.x < SIZE;pos.x++)
            if(board[pos.y][pos.x].type == piece && board[pos.y][pos.x].color == color)
                return pos;
}

int king_move_position(struct position pos)
{
    int evaluation = 0;
    float endGameWeight = game_weight();
    struct position enemy_king = find_piece(king, !board[pos.y][pos.x].color);
    struct position my_king = find_piece(king, board[pos.y][pos.x].color);
    int dstFromEnemyKing = mod(my_king.x - enemy_king.x) + mod(my_king.y - enemy_king.y);
    int dstEnemyKingFromCenter = max(3 - enemy_king.x, enemy_king.x - 4) + max(3 - enemy_king.y, enemy_king.y - 4);
    evaluation += dstEnemyKingFromCenter*2 + 14 - dstFromEnemyKing;
    return evaluation * 10 * endGameWeight;
}

int evaluate_piece_move(struct position pos)
{
    struct move move = {pos, pos};
    int danger_squares = 0, defends = 0, king_attack = 0;
    struct square *to;
    struct position enemyKingPos = find_piece(king, !board[pos.y][pos.x].color), move_pos;

    while(piece[board[pos.y][pos.x].type].enum_move(&pos, &move))
    {
        to = &board[move.to.y][move.to.x];

        if(to->color == board[move.to.y][move.to.x].color)
            defends += 4 / (board[move.to.y][move.to.x].type + 1);///if a pawn defends a queen that is useless most of the times but if queen defends a pawn that is always useful
        else
            danger_squares += to->type + 1;

        int dstBtwEnemyKingLineOfFire = mod(move.to.x - enemyKingPos.x) + mod(move.to.y - enemyKingPos.y) + 1;
        if(dstBtwEnemyKingLineOfFire < 3) king_attack += 2;
    }

    if(board[pos.y][pos.x].type == knight) danger_squares *= 3;
    else if(board[pos.y][pos.x].type == pawn) danger_squares *= 2;

    return danger_squares + defends + king_attack;
}

int evaluate_taking(struct position pos, struct undo undo)
{
    struct square *from = &board[pos.y][pos.x];

    if(undo.taken == empty)
        return 0;

    int activity_of_taken_piece = 500/(evaluate_piece_move(pos) + 1);

    if((from->color == black && global_evaluation <= piece[knight].weight(pos)) || (from->color == white && global_evaluation >= piece[knight].weight(pos)))
        return 20 + activity_of_taken_piece;

    if((from->color == black && global_evaluation >= piece[knight].weight(pos)) || (from->color == white && global_evaluation <= piece[knight].weight(pos)))
        return -100 + activity_of_taken_piece;

    if(game_weight() > 0.95)
        return -30 + activity_of_taken_piece;
}

///best in the start of thee game when strong pieces need to stay protected and pawn need to be in the front
int center_taking(struct position pos)
{
    int eval = 200/pow(board[pos.y][pos.x].type, 2);///the weaker the piece the better to be in the middle of the board
    eval /= min(mod(3 - pos.x), mod(4 - pos.x)) + 1;/// the greater the dist from the center the bigger the divider
    eval /= min(mod(4 - pos.y), mod(5 - pos.y)) + 1;/// if the piece is in the middle of the board give more points
    return eval;
}

void calc_space(int *white_space, int *black_space)
{
    for(int y = 0;y < SIZE;y++)
        for(int x = 0;x < SIZE;x++)
            if(board[y][x].type == pawn)
                if(board[y][x].color == white) (*white_space) += SIZE - y - 1;
                else                           (*black_space) += y + 1;
}

int space_taking(enum color player)
{
    int white_space = 0, black_space = 0;
    calc_space(&white_space, &black_space);
    white_space*=5, black_space*=5;
    if(player == black) return (black_space - white_space);
    return  (white_space - black_space);
}

///develop piece in the start of the game
int piece_early_development(struct position pos)
{
    if(move_cnt <= 22 || board[pos.y][pos.x].type == pawn)
        return 0;

    if(move_cnt < 12 && board[pos.y][pos.x].type > pawn && board[pos.y][pos.x].type < queen)
        return 100;

    if(move_cnt > 12 && move_cnt < 22 && board[pos.y][pos.x].type == queen)
        return 15;
}
