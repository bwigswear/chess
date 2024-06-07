#include "board.h"
#include <iostream>

Board::Board() 
{
    Board::ResetBoard();
}

void Board::ResetBoard()
{
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            board[i][j] = ((i % 2 == 0 && j % 2 == 1) || (i % 2 == 1 && j % 2 == 0)) ? BLACKSQUARE : WHITESQUARE;
        }
    }

    board[0][0] |= (ROOK | BLACKPIECE); board[0][1] |= (KNIGHT | BLACKPIECE); board[0][2] |= (BISHOP | BLACKPIECE); board[0][3] |= (QUEEN | BLACKPIECE);
    board[0][4] |= (KING | BLACKPIECE); board[0][5] |= (BISHOP | BLACKPIECE); board[0][6] |= (KNIGHT | BLACKPIECE); board[0][7] |= (ROOK | BLACKPIECE);

    board[7][0] |= (ROOK | WHITEPIECE); board[7][1] |= (KNIGHT | WHITEPIECE); board[7][2] |= (BISHOP | WHITEPIECE); board[7][3] |= (QUEEN | WHITEPIECE);
    board[7][4] |= (KING | WHITEPIECE); board[7][5] |= (BISHOP | WHITEPIECE); board[7][6] |= (KNIGHT | WHITEPIECE); board[7][7] |= (ROOK | WHITEPIECE);
    
    
    for (int i = 0; i < 8; i++) {
        board[1][i] |= (PAWN | BLACKPIECE); // Set the second row with pawns
        board[6][i] |= (PAWN | WHITEPIECE); // Set the seventh row with pawns
    }
}