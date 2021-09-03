#include<stdio.h>
#include<math.h>

enum type { empty, pawn, knight, bishop, rook, queen, king };
enum color { black, white };

struct square
{
	enum type type;
	enum color color;
};


struct chess_board_less_memory //typicly for a chess board are need 64 ints but these way only 14 are needed
{	
	unsigned long long piece[6];//, 0 - pawn, 1 - knight, 2 - bishop, 3 - rook, 4 - queen, 5 - king
	unsigned long long color;// at first every piece is black by default
};


void code_board(struct chess_board_less_memory* empty_board, struct square board[8][8])
{
	for (int i = 0; i < 6; empty_board->piece[i++] = 0);
	empty_board->color = 0;
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			unsigned long long n = pow(2, y * 8 + x);
            if (board[y][x].type)
            {
                empty_board->piece[board[y][x].type - 1] += n;// board[y][x].type - 1 because we don't need arr for empty
                empty_board->color += board[y][x].color * n;
            }
		}
	}
}


void decode_board(struct square(*empty_board)[8], struct chess_board_less_memory board)
{
    for (int i = 0; i < 6; i++)
    {
        for (int y = 7; y >= 0; y--)
            for (int x = 7; x >= 0; x--)
            {
                unsigned long long n = pow(2, y * 8 + x);
                if (n <= board.piece[i])
                {
                    empty_board[y][x].type = i + 1;
                    board.piece[i] -= n;
                }

            }

    }

    for (int y = 7; y >= 0; y--)
        for (int x = 7; x >= 0; x--)
        {
            unsigned long long n = pow(2, y * 8 + x);
            if (n <= board.color)
            {
                empty_board[y][x].color = white;
                board.color -= n;
            }
        }
    
}


void fill_board(struct square (*board)[8])
{
    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            board[y][x].type = empty;

    for (int x = 0; x < 8; x++)
        board[1][x].type = board[6][x].type = pawn, board[1][x].color = black, board[6][x].color = white;
    board[0][3].type = board[7][3].type = queen;
    board[0][3].color = black;
    board[7][3].color = white;
    board[0][4].type = board[7][4].type = king;
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

void print_board(struct square board[8][8])
{
    printf("  X");
    for (int x = 0; x < 8; x++)
        printf("   %d", x);

    printf("\n");
    for (int y = 0; y < 8; y++)
    {
        printf("    |");
        for (int x = 0; x < 8; x++)
            printf("---+");

        printf("\n");
        printf("%d ->|", y);
        for (int x = 0; x < 8; x++)
        {
            char a = getPiece(board[y][x]);
            printf(" %c |", a);
        }
        if (y == 8 - 1)
        {
            printf("\n"); printf("    |");
            for (int x = 0; x < 8; x++)
                printf("---+");
        }
        printf("\n");
    }
    printf("  Y  ");
    printf("\n"); printf("\n");
}

int main()
{
	struct square board[8][8];
	fill_board(board);
   // print_board(board);
	struct chess_board_less_memory new_board;
	code_board(&new_board, board);
	struct square old_new_board[8][8];
	decode_board(old_new_board, new_board);
    print_board(old_new_board);
	return 0;
}
