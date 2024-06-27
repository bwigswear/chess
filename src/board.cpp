#include "board.h"
Board::Board(sf::Texture& texture) 
    : piecesTexture(texture)
{

}

void Board::RenderBoard(sf::RenderWindow &window, float squareSize)
{

    sf::RectangleShape boardSquares[8][8];
    sf::Color Tan (224, 209, 175);
    sf::Color Brown (92, 65, 6);

    for (int row = 0; row < 8; ++row) 
    {
        for (int col = 0; col < 8; ++col) 
        {
            boardSquares[row][col].setSize(sf::Vector2f(squareSize, squareSize));
            boardSquares[row][col].setPosition(col * squareSize, row * squareSize);
            if(board[row][col] & WHITESQUARE) boardSquares[row][col].setFillColor(Tan);
            if(board[row][col] & BLACKSQUARE) boardSquares[row][col].setFillColor(Brown);
            window.draw(boardSquares[row][col]);
        }
    }
}

void Board::ResetBoard()
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            board[i][j] = ((i % 2 == 0 && j % 2 == 1) || (i % 2 == 1 && j % 2 == 0)) ? BLACKSQUARE : WHITESQUARE;
        }
    }

    board[0][0] |= (ROOK | BLACKPIECE); board[1][0] |= (KNIGHT | BLACKPIECE); board[2][0] |= (BISHOP | BLACKPIECE); board[3][0] |= (QUEEN | BLACKPIECE);
    board[4][0] |= (KING | BLACKPIECE); board[5][0] |= (BISHOP | BLACKPIECE); board[6][0] |= (KNIGHT | BLACKPIECE); board[7][0] |= (ROOK | BLACKPIECE);

    board[0][7] |= (ROOK | WHITEPIECE); board[1][7] |= (KNIGHT | WHITEPIECE); board[2][7] |= (BISHOP | WHITEPIECE); board[3][7] |= (QUEEN | WHITEPIECE);
    board[4][7] |= (KING | WHITEPIECE); board[5][7] |= (BISHOP | WHITEPIECE); board[6][7] |= (KNIGHT | WHITEPIECE); board[7][7] |= (ROOK | WHITEPIECE);
    
    
    for (int i = 0; i < 8; i++) 
    {
        board[i][1] |= (PAWN | BLACKPIECE); // Set the second row with pawns
        board[i][6] |= (PAWN | WHITEPIECE); // Set the seventh row with pawns
    }
}

void Board::RenderPieces(sf::RenderWindow &window, sf::Sprite* pieceSprites, float squareSize)
{
    sf::IntRect textureRect(0, 0, 200, 200);
    int x = 0;
    
    for(int j = 0; j < 8; j++)
    {
        for(int i = 0; i < 8; i++)
        {
            if(board[i][j] & 7)
            {
                pieceSprites[x].setTexture(piecesTexture);
                textureRect.top = (board[i][j] & WHITEPIECE) ? 0 : 200;
                switch (board[i][j] & 7){
                    case PAWN:
                        textureRect.left = 1000;
                        pieceSprites[x].setTextureRect(textureRect);
                        pieceSprites[x].setPosition(i * squareSize, j * squareSize);
                        break;
                    case ROOK:
                        textureRect.left = 800;
                        pieceSprites[x].setTextureRect(textureRect);
                        pieceSprites[x].setPosition(i * squareSize, j * squareSize);
                        break;
                    case KNIGHT:
                        textureRect.left = 600;
                        pieceSprites[x].setTextureRect(textureRect);
                        pieceSprites[x].setPosition(i * squareSize, j * squareSize);
                        break;
                    case BISHOP:
                        textureRect.left = 400;
                        pieceSprites[x].setTextureRect(textureRect);
                        pieceSprites[x].setPosition(i * squareSize, j * squareSize);
                        break;
                    case QUEEN:
                        textureRect.left = 200;
                        pieceSprites[x].setTextureRect(textureRect);
                        pieceSprites[x].setPosition(i * squareSize, j * squareSize);
                        break;
                    case KING:
                        textureRect.left = 0;
                        pieceSprites[x].setTextureRect(textureRect);
                        pieceSprites[x].setPosition(i * squareSize, j * squareSize);
                }
                pieceSprites[x].setScale(squareSize / 200, squareSize / 200);
                window.draw(pieceSprites[x++]);
            }

        }
    }

}

void printBinary(char c) {
    // Use std::bitset to convert char to binary string
    std::bitset<8> binary(c);
    std::cout << binary << std::endl;
}

int Board::CheckMove(int startX, int startY, int endX, int endY)
{
    if(endX < 0 || endX > 7 || endY < 0 || endY > 7) return 0;
    if(startX == endX && startY == endY) return 0;
    if((board[startX][startY] & board[endX][endY]) & 48) return 0;
    int xStep, yStep;
    switch (board[startX][startY] & 7){
        case PAWN: //WILL NEED TO ADD EN PASSANT
            if(startX == endX)
            {
                if(endY - startY == 1)
                {
                    return (board[startX][startY] & BLACKPIECE) && !(board[endX][endY] & 7) ? 1 : 0;
                }
                if(startY - endY == 1)
                {
                    return (board[startX][startY] & WHITEPIECE) && !(board[endX][endY] & 7) ? 1 : 0;
                }
                if(endY - startY == 2)
                {
                    return (board[startX][startY] & BLACKPIECE) && !(board[endX][endY] & 7) && !(board[endX][endY - 1] & 7) ? 1 : 0;
                }
                if(startY - endY == 2)
                {
                    return (board[startX][startY] & WHITEPIECE) && !(board[endX][endY] & 7) && !(board[endX][endY + 1] & 7) ? 1 : 0;
                }
            }
            if(startX - endX == 1 || startX - endX == -1)
            {
                if(endY - startY == 1)
                {
                    return (board[startX][startY] & BLACKPIECE) && (board[endX][endY] & WHITEPIECE) ? 2 : 0;
                }
                if(startY - endY == 1)
                {
                    return (board[startX][startY] & WHITEPIECE) && (board[endX][endY] & BLACKPIECE) ? 2 : 0;
                }
            }
            return 0;
            break;
        case ROOK:
            if(startX != endX && startY != endY) return 0;
            
            if(startX == endX)
            {
                if(startY > endY)
                {
                    for(int i = startY - 1; i > endY; i--)
                    {
                        if(board[startX][i] & 7) return 0;
                    }
                }
                else
                {
                    for(int i = startY + 1; i < endY; i++)
                    {
                        if(board[startX][i] & 7) return 0;
                    }
                }
                return board[endX][endY] & 7 ? 2 : 1;
            }
            if(startY == endY)
            {
                if(startX > endX)
                {
                    for(int i = startX - 1; i > endX; i--)
                    {
                        if(board[i][startY] & 7) return 0;
                    }
                }
                else
                {
                    for(int i = startX + 1; i < endX; i++)
                    {
                        if(board[i][startY] & 7) return 0;
                    }
                }
                return board[endX][endY] & 7 ? 2 : 1;
            }
            return 0;
            break;
        case KNIGHT:
            if(startX == endX || startY == endY) return 0;
            if((abs(startY - endY) == 2 && abs(startX - endX) == 1) || (abs(startY - endY) == 1 && abs(startX - endX) == 2)) return board[endX][endY] & 7 ? 2 : 1;
            return 0;
            break;
        case BISHOP:
            if(abs(startX - endX) != abs(startY - endY)) return 0;
            xStep = startX > endX ? -1 : 1;
            yStep = startY > endY ? -1 : 1;
            for(int i = startX + xStep, j = startY + yStep; i != endX && j != endY; i+=xStep, j+=yStep) if(board[i][j] & 7) return 0;
            return board[endX][endY] & 7 ? 2 : 1;
            break;
        case QUEEN:

            return 0;
            break;
        case KING:
            if(endX == 6 && endY == 7 && !(board[startX][startY] & MOVED) && (board[7][7] & ROOK) && !(board[7][7] & MOVED) 
            && !(board[5][7] & 7) && !(board[6][7] & 7)) return 3;
            if(endX == 6 && endY == 0 && !(board[startX][startY] & MOVED) && (board[7][0] & ROOK) && !(board[7][0] & MOVED)
            && !(board[5][0] & 7) && !(board[6][0] & 7)) return 3;
            //Implement check checking here
            if(abs(startX - endX) > 1 || abs(startY - endY) > 1) return 0;
            return board[endX][endY] & 7 ? 2 : 1;
            return 0;
            break;

    }
    return 0;
}

void Board::MakeMove(int startX, int startY, int endX, int endY, float squareSize, sf::Sprite &pieceSprite, sf::Sprite &captureSprite)
{
    board[endX][endY] &= 192; board[endX][endY] |= (board[startX][startY] & 63);
    board[startX][startY] &= 192;
    captureSprite.setPosition(-1 * squareSize,-1 * squareSize);
    pieceSprite.setPosition(endX * squareSize, endY * squareSize);
    return;
}

void Board::MakeMove(int startX, int startY, int endX, int endY, float squareSize, sf::Sprite &pieceSprite)
{
    board[endX][endY] |= (board[startX][startY] & 63);
    board[startX][startY] &= 192;
    pieceSprite.setPosition(endX * squareSize, endY * squareSize);
    return;
}