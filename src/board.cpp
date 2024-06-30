#include "board.h"
Board::Board(sf::Texture& texture) 
    : piecesTexture(texture), turn(true), blackKing(4, 0), whiteKing(4, 7)
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

int Board::CheckMove(int startX, int startY, int endX, int endY, bool checkTurn)
{
    if(checkTurn) if((turn && (board[startX][startY] & BLACKPIECE)) || (!turn && (board[startX][startY] & WHITEPIECE))) return 0;
    if(endX < 0 || endX > 7 || endY < 0 || endY > 7) return 0;
    if(startX == endX && startY == endY) return 0;
    if((board[startX][startY] & board[endX][endY]) & 48) return 0;
    int xStep, yStep;
    switch (board[startX][startY] & 7){
        case PAWN:
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
                if(endY - startY == 2 && startY == 1 && (board[startX][startY] & BLACKPIECE))
                {
                    if(!(board[endX][endY] & 7) && !(board[endX][endY - 1] & 7))
                    {
                        board[endX][endY] |= 8;
                        return 1;
                    }
                    return 0;
                }
                if(startY - endY == 2 && startY == 6 && (board[startX][startY] & WHITEPIECE))
                {
                    if(!(board[endX][endY] & 7) && !(board[endX][endY + 1] & 7))
                    {
                        board[endX][endY] |= 8;
                        return 1;
                    }
                    return 0;
                }
            }
            xStep = endX - startX;
            yStep = endY - startY;
            if(abs(xStep) != 1 || abs(yStep) != 1 || (yStep > 0 && !(board[startX][startY] & BLACKPIECE)) || (yStep < 0 && !(board[startX][startY] & WHITEPIECE))) return 0;
            if(board[endX][endY] & 7) return 2;
            //Piece direction is valid, movement is 1 square diagonal, but no direct capture: check for en passant
            if((board[endX][endY - yStep] & MOVED) && (board[endX][endY - yStep] & 7) == PAWN) return 4;
            return 0;
            break;
        case ROOK:
            if(startX != endX && startY != endY) return 0;
            if(startX == endX)
            {
                yStep = startY > endY ? -1 : 1;
                for(int i = startY + yStep; i != endY; i+=yStep) if(board[endX][i] & 7) return 0;
                return board[endX][endY] & 7 ? 2 : 1;
            }
            if(startY == endY)
            {
                xStep = startX > endX ? -1 : 1;
                for(int i = startX + xStep; i != endX; i+=xStep) if(board[i][endY] & 7) return 0;
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
            if(abs(startX - endX) == abs(startY - endY))
            {
                xStep = startX > endX ? -1 : 1;
                yStep = startY > endY ? -1 : 1;
                for(int i = startX + xStep, j = startY + yStep; i != endX && j != endY; i+=xStep, j+=yStep) if(board[i][j] & 7) return 0;
                return board[endX][endY] & 7 ? 2 : 1;
            }
            if(startX == endX)
            {
                yStep = startY > endY ? -1 : 1;
                for(int i = startY + yStep; i != endY; i+=yStep) if(board[endX][i] & 7) return 0;
                return board[endX][endY] & 7 ? 2 : 1;
            }
            if(startY == endY)
            {
                xStep = startX > endX ? -1 : 1;
                for(int i = startX + xStep; i != endX; i+=xStep) if(board[i][endY] & 7) return 0;
                return board[endX][endY] & 7 ? 2 : 1;
            }
            return 0;
            break;
        case KING:
            if(startX == 4 && startY == 7 && endX == 6 && endY == 7 && !(board[startX][startY] & MOVED) && (board[7][7] & ROOK) && !(board[7][7] & MOVED) 
            && !(board[5][7] & 7) && !(board[6][7] & 7)) return 3;
            if(startX == 4 && startY == 0 && endX == 6 && endY == 0 && !(board[startX][startY] & MOVED) && (board[7][0] & ROOK) && !(board[7][0] & MOVED)
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
    /* After a move is made, any opportunity to en passant is lost. Maybe there is a way of doing this with less overhead,
    especially considering how rarely it will matter.*/
    turn = !(board[startX][startY] & WHITEPIECE);
    if((board[startX][startY] & 7) == KING || (board[startX][startY] & 7) == ROOK) board[startX][startY] |= MOVED;  
    for(int i = 0; i < 8; i++)
    {
        if((board[i][3] & 7) == PAWN) board[i][3] &= 247;
        if((board[i][4] & 7) == PAWN) board[i][4] &= 247;
    }
    board[endX][endY] &= 192; board[endX][endY] |= (board[startX][startY] & 63);
    board[startX][startY] &= 192;
    captureSprite.setPosition(-1 * squareSize, -1 * squareSize);
    pieceSprite.setPosition(endX * squareSize, endY * squareSize);
    if((board[endX][endY] & (KING | WHITEPIECE)) == (KING | WHITEPIECE)) whiteKing.x = endX, whiteKing.y = endY;
    if((board[endX][endY] & (KING | BLACKPIECE)) == (KING | BLACKPIECE)) blackKing.x = endX, blackKing.y = endY;
    return;
}

void Board::MakeMove(int startX, int startY, int endX, int endY, float squareSize, sf::Sprite &pieceSprite)
{
    turn = !(board[startX][startY] & WHITEPIECE);
    if((board[startX][startY] & 7) == KING || (board[startX][startY] & 7) == ROOK) board[startX][startY] |= MOVED;  
    for(int i = 0; i < 8; i++)
    {
        if((board[i][3] & 7) == PAWN) board[i][3] &= 247;
        if((board[i][4] & 7) == PAWN) board[i][4] &= 247;
    }
    board[endX][endY] |= (board[startX][startY] & 63);
    board[startX][startY] &= 192;
    pieceSprite.setPosition(endX * squareSize, endY * squareSize);
    if((board[endX][endY] & PAWN) && abs(startY - endY) == 2) board[endX][endY] |= 8;
    return;
}

bool Board::MateCheck(char color)
{
    /* This function ONLY checks if the King has a position to move to in which it will not be in check.
    It will consider the King to be in mate even if every surrounding square is occupied.
    It will only be called by StaleMateCheck and CheckMateCheck. */
    std::vector<sf::Vector2i> positions;
    sf::Vector2i kingPos = color == WHITEPIECE ? whiteKing : blackKing;

    //If a square is not occupied by the same color piece and a viable board position, add it to the list of positions to be checked
    if(kingPos.x - 1 > -1 && kingPos.y - 1 > -1 && !(board[kingPos.x - 1][kingPos.y - 1] & color)) positions.push_back(kingPos + sf::Vector2i(-1, -1));
    if(kingPos.y - 1 > -1 && !(board[kingPos.x][kingPos.y - 1] & 7)) positions.push_back(kingPos + sf::Vector2i(0, -1));
    if(kingPos.x + 1 < 8 && kingPos.y - 1 > -1 && !(board[kingPos.x + 1][kingPos.y - 1] & color)) positions.push_back(kingPos + sf::Vector2i(1, -1));
    if(kingPos.x + 1 < 8 && !(board[kingPos.x + 1][kingPos.y] & 7)) positions.push_back(kingPos + sf::Vector2i(1, 0));
    if(kingPos.x + 1 < 8 && kingPos.y + 1 < 8 && !(board[kingPos.x + 1][kingPos.y + 1] & color)) positions.push_back(kingPos + sf::Vector2i(1, 1));
    if(kingPos.y + 1 < 8 && !(board[kingPos.x][kingPos.y + 1] & 7)) positions.push_back(kingPos + sf::Vector2i(0, 1));
    if(kingPos.x - 1 > -1 && kingPos.y + 1 < 8 && !(board[kingPos.x - 1][kingPos.y + 1] & color)) positions.push_back(kingPos + sf::Vector2i(-1, 1));
    if(kingPos.x - 1 > -1 && !(board[kingPos.x - 1][kingPos.y] & 7)) positions.push_back(kingPos + sf::Vector2i(-1, 0));
    
    //If all squares surrounding the king are occupied return true.
    if(positions.size() == 0) return true;

    //Check each open position around the king to see if the king will be in check after moving there
    for(sf::Vector2i position : positions)
    {
        bool capable = true;
        for(int i = 0; i < 64; i++)
        {
            if((board[i % 8][i / 8] & 7) && !(board[i % 8][i / 8] & color) && CheckMove(i % 8, i / 8, position.x, position.y, false)) {capable = false; break;}
        }
        if(capable) return false;
    }
    return true;
}


/* Check all checks must check all opposing color pieces to see if a piece has the king in check. This must be called after every move
since a move could place a king in check even if the moved piece is not the one directly responsible. The function will return a vector
(-1, -1) if there is no check, (-2, -2) if there are two pieces checking, or (x, y) representing the position of the piece responsible
for check if there is only one. */
sf::Vector2i Board::CheckAllChecks(char color)
{
    sf::Vector2i checker(-1, -1);
    for(int i = 0; i < 64; i++)
    {
        if((board[i % 8][i / 8] & 7) && !(board[i % 8][i / 8] & color) && CheckCheck(i % 8, i / 8, color))
        {
            if(checker.x == -1)
            {
                checker = sf::Vector2i(i % 8, i / 8);
            }
            else
            {
                return sf::Vector2i(-2, -2);
            }
        }
    }
    return checker;
}

/* CheckCheck function checks to see if a piece at a certain position and of a certain color is responsible for the opposing king being in check.
This function should be called after almost every move to determine if a checkmate should be checked for and should also be called during the checking of
a viable king move. */
int Board::CheckCheck(int x, int y, char color)
{
    return CheckMove(x, y, color == BLACKPIECE ? blackKing.x : whiteKing.x, color == BLACKPIECE ? blackKing.y : whiteKing.y, false);
}

//CheckMateCheck function checks to see if a specific color is in checkmate and should only be called after that color's king has been placed in check.
bool Board::CheckMateCheck(char color, sf::Vector2i& checker)
{

    if(checker.x == -1) return false;
    if(checker.x == -2) return MateCheck(color);

    //If there is one piece responsible for checking, check to see if that piece can be captured or blocked



    return true;
}