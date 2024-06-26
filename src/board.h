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
#define MOVED  0b00001000

#include <SFML/Graphics.hpp>
#include <bitset> //For debug
#include <iostream>

class Board{
private:
    char board[8][8];
    sf::Texture& piecesTexture;
public:
    Board(sf::Texture&);
    void ResetBoard();
    void RenderBoard(sf::RenderWindow&, float squareSize);
    void RenderPieces(sf::RenderWindow &window, sf::Sprite* pieceSprites, float squareSize);
    int CheckMove(int startX, int startY, int endX, int endY);
    void MakeMove(int startX, int startY, int endX, int endY, float squareSize, sf::Sprite &pieceSprite, sf::Sprite &captureSprite);
    void MakeMove(int startX, int startY, int endX, int endY, float squareSize, sf::Sprite &pieceSprite);
};