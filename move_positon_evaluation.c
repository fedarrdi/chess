#include "a.h"
#include<math.h>
extern long long global_evaluation;
extern struct square board[8][8];
extern struct piece piece[7];
extern int move_cnt;

int mod(int a);
int max(int a, int b){ return (a > b) ? a : b;}
int min(int a, int b){ return (a < b) ? a : b;}

float game_weight()
{
    float weight = 0;
    for(struct position pos = {0, 0};pos.y < SIZE;pos.y++)
        for(;pos.x < SIZE;pos.x++)
            weight += piece[board[pos.y][pos.x].type].weight(pos);
    /**
     first we round 10003300 to 1000000
     total sum of piece weight = 10000000
     squash it to number between 1 and 0 1 = 1000000
     if we divide by 10 8 times we will get 1
     if we divide smaller number by 10 8 times it will become less then 1
     and always bigger than 0 because board.value is always > 0
    */

    weight -= 3300;
    for(int i = 1;i < 8;i++)
        weight /= 10;

    return  weight;
}

struct position find_piece(enum type piece, enum color color)
{
    for(struct position pos = {0, 0};pos.y < SIZE;pos.y++)
        for(;pos.x < SIZE;pos.x++)
            if(board[pos.y][pos.x].type == piece && board[pos.y][pos.x].color == color)
                return pos;
}

int king_move_position(const struct position *pos)
{
    int evaluation = 0;
    float endGameWeight = mod(1 - game_weight());/// reverse
    struct position enemy_king = find_piece(king, !board[pos->y][pos->x].color);
    struct position my_king = find_piece(king, board[pos->y][pos->x].color);
    int dstFromEnemyKing = mod(my_king.x - enemy_king.x) + mod(my_king.y - enemy_king.y);
    int dstEnemyKingFromCenter = max(3 - enemy_king.x, enemy_king.x - 4) + max(3 - enemy_king.y, enemy_king.y - 4);
    evaluation += dstEnemyKingFromCenter + 14 - dstFromEnemyKing;
    return evaluation * endGameWeight;
}

int evaluate_piece_move(struct position *pos)
{
    struct move move = {*pos, *pos};
    int danger_squares = 0, defends = 0, king_attack = 0;
    struct square *to;
    struct position enemyKingPos = find_piece(king, !board[pos->y][pos->x].color), move_pos;

    while(piece[board[pos->y][pos->x].type].enum_move(pos, &move))
    {
        to = &board[move.to.y][move.to.x];

        if(to->color == board[move.to.y][move.to.x].color)
            defends += 4 / (board[move.to.y][move.to.x].type + 1);///if a pawn defends a queen that is useless most of the times but if queen defends a pawn that is always useful
        else
            danger_squares += to->type + 1;

        int dstBtwEnemyKingLineOfFire = mod(move.to.x - enemyKingPos.x) + mod(move.to.y - enemyKingPos.y) + 1;
        if(dstBtwEnemyKingLineOfFire < 3) king_attack += 2;
    }

    if(board[pos->y][pos->x].type == knight) danger_squares *= 5;
    else if(board[pos->y][pos->x].type == pawn) danger_squares *= 2;

    return 3*(danger_squares + defends + king_attack);
}

int evaluate_taking(struct position *pos, const struct undo *undo)
{
    struct square *from = &board[pos->y][pos->x];

    if(undo->taken == empty)
        return 0;

    int activity_of_taken_piece = 500/(evaluate_piece_move(pos) + 1);

    if((from->color == black && global_evaluation <= piece[knight].weight(*pos)) || (from->color == white && global_evaluation >= piece[knight].weight(*pos)))
        return 20 + activity_of_taken_piece;

    if((from->color == black && global_evaluation >= piece[knight].weight(*pos)) || (from->color == white && global_evaluation <= piece[knight].weight(*pos)))
        return -100 + activity_of_taken_piece;

    if(game_weight() > 0.95)
        return -30 + activity_of_taken_piece;
}

///best in the start of thee game when strong pieces need to stay protected and pawn need to be in the front
int center_taking(const struct position *pos)
{
    int eval = 300/pow(board[pos->y][pos->x].type, 2);///the weaker the piece the better to be in the middle of the board
    eval /= min(mod(3 - pos->x), mod(4 - pos->x)) + 1;/// the greater the dist from the center the bigger the divider
    eval /= min(mod(4 - pos->y), mod(5 - pos->y)) + 1;/// if the piece is in the middle of the board give more points
    return eval * game_weight();
}

void calc_space(int *white_space, int *black_space)
{
    for(int y = 0;y < SIZE;y++)
        for(int x = 0;x < SIZE;x++)
            if(board[y][x].type == pawn)
                board[y][x].color == white ? ((*white_space) += SIZE - y - 1) : ((*black_space) += y + 1);
}

int space_taking(enum color player)
{
    int white_space = 0, black_space = 0;
    calc_space(&white_space, &black_space);
    white_space*=2, black_space*=2;
    return (player == black ? black_space - white_space : white_space - black_space) * mod(0.5 - game_weight());
}

///develop piece in the start of the game
int piece_early_development(const struct position *pos)
{
    if (move_cnt <= 22 || board[pos->y][pos->x].type == pawn) return 0;

    if (move_cnt < 12 && board[pos->y][pos->x].type > pawn && board[pos->y][pos->x].type < queen) return 100;

    if (move_cnt > 12 && move_cnt < 22 && board[pos->y][pos->x].type == queen) return 15;

    if (move_cnt < 12 && board[pos->y][pos->x].type == queen) return -200;
}

int evaluate_king_position_mid_game(struct position *pos)
{
    if(board[pos->y][pos->x].type != king) return 0;
    enum color king_color = board[pos->y][pos->x].color;
    int king_safety = 0;

    if(pos->x + 1 < SIZE && board[pos->y][pos->x + 1].color == king_color) king_safety ++;
    if(pos->x - 1 >= 0 && board[pos->y][pos->x - 1].color == king_color) king_safety ++;
    if(pos->y + 1 < SIZE && board[pos->y + 1][pos->x].color == king_color) king_safety ++;
    if(pos->y - 1 >= 0 && board[pos->y - 1][pos->x].color == king_color) king_safety ++;
    if(pos->x + 1 < SIZE &&  pos->y + 1 < SIZE && board[pos->y + 1][pos->x + 1].color == king_color) king_safety ++;
    if(pos->x - 1 >= 0 && pos->y + 1 < SIZE && board[pos->y + 1][pos->x - 1].color == king_color) king_safety ++;
    if(pos->y - 1 >= 0 && pos->x + 1 < SIZE && board[pos->y - 1][pos->x + 1].color == king_color) king_safety ++;
    if(pos->y - 1 >= 0 && pos->x - 1 >= 0 && board[pos->y - 1][pos->x - 1].color == king_color) king_safety ++;

    int king_dst_from_center = max(mod(pos->x - 3), mod(pos->x - 4)) * max(mod(pos->y - 3), mod(pos->y - 4));

    return (king_safety + king_dst_from_center)*game_weight();
}