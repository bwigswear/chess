#pragma once
#define BLACKSQUARE  0b10000000
#define BLACKPIECE   0b00100000
#define WHITESQUARE  0b01000000
#define WHITEPIECE   0b00010000
#define PAWN   0b00000001
#define ROOK   0b00000010
#define KNIGHT 0b00000011
#define BISHOP 0b00000100
#define QUEEN  0b00000101
#define KING   0b00000111

class Board{
private:
    char board[8][8];
public:
    Board();
    void ResetBoard();
};