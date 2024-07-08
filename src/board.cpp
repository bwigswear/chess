#include "board.h"
Board::Board(sf::Texture& texture) 
    : piecesTexture(texture), turn(true)
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
    turn = true;
    blackKing = {4, 0}; whiteKing = {4, 7};
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

/* This function is the primary logic behind piece movement. Given a starting square and ending square, determine if
the move can be made. The checkTurn parameter is used by calling functions to signify if the turn designation is to be
used when checking a move's validity. Return values are 0 - invalid, 1 - valid + no capture, 2 - valid + capture,
3 - castle, 4 - en passant*/

int Board::CheckMove(int startX, int startY, int endX, int endY, bool checkTurn)
{
    //Check for basic invalid moves
    if(checkTurn) if((turn && (board[startX][startY] & BLACKPIECE)) || (!turn && (board[startX][startY] & WHITEPIECE))) return 0;
    if(endX < 0 || endX > 7 || endY < 0 || endY > 7) return 0;
    if(startX == endX && startY == endY) return 0;
    if((board[startX][startY] & board[endX][endY]) & 48) return 0;
    
    //Check if color will be in check after move is made
    if(checkTurn && TestCheck(startX, startY, endX, endY)) return 0;
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
            // Need some type of board testing to see if the move will put King in check
            if(startX == 4 && startY == 7 && endX == 6 && endY == 7 && !(board[startX][startY] & MOVED) && (board[7][7] & ROOK) && !(board[7][7] & MOVED) 
            && !(board[5][7] & 7) && !(board[6][7] & 7)) return 3;
            if(startX == 4 && startY == 7 && endX == 2 && endY == 7 && !(board[startX][startY] & MOVED) && (board[0][7] & ROOK) && !(board[0][7] & MOVED) 
            && !(board[3][7] & 7) && !(board[2][7] & 7)) return 3;
            if(startX == 4 && startY == 0 && endX == 6 && endY == 0 && !(board[startX][startY] & MOVED) && (board[7][0] & ROOK) && !(board[7][0] & MOVED)
            && !(board[5][0] & 7) && !(board[6][0] & 7)) return 3;
            if(startX == 4 && startY == 0 && endX == 2 && endY == 0 && !(board[startX][startY] & MOVED) && (board[0][0] & ROOK) && !(board[0][0] & MOVED)
            && !(board[3][0] & 7) && !(board[2][0] & 7)) return 3;
            //Implement check checking here
            if(abs(startX - endX) > 1 || abs(startY - endY) > 1) return 0;
            return board[endX][endY] & 7 ? 2 : 1;
            return 0;
            break;
    }
    return 0;
}

//This function is called when a piece is moved to capture another
void Board::MakeMove(int startX, int startY, int endX, int endY, float squareSize, sf::Sprite &pieceSprite, sf::Sprite &captureSprite)
{
    //Change current global board state
    turn = !(board[startX][startY] & WHITEPIECE);

    //Any king or rook movement prevents future castling
    if((board[startX][startY] & 7) == KING || (board[startX][startY] & 7) == ROOK) board[startX][startY] |= MOVED;  
    
    /* After a move is made, opportunity to move on a piece that was available for en passant capture is no longer available.
    This could likely be optimized, especially considering how rarely it comes into play. */
    for(int i = 0; i < 8; i++)
    {
        if((board[i][3] & 7) == PAWN) board[i][3] &= 247;
        if((board[i][4] & 7) == PAWN) board[i][4] &= 247;
    }

    //Transference of board square states
    board[endX][endY] &= 192; board[endX][endY] |= (board[startX][startY] & 63);
    board[startX][startY] &= 192;

    //Repositioning of piece sprites
    captureSprite.setPosition(-1 * squareSize, -1 * squareSize);
    pieceSprite.setPosition(endX * squareSize, endY * squareSize);

    //Updating of king global positions for board state calculations
    if((board[endX][endY] & (KING | WHITEPIECE)) == (KING | WHITEPIECE)) whiteKing.x = endX, whiteKing.y = endY;
    if((board[endX][endY] & (KING | BLACKPIECE)) == (KING | BLACKPIECE)) blackKing.x = endX, blackKing.y = endY;

    return;
}


//This version of make move is called when a piece capture is not necessary.
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
    if((board[endX][endY] & (KING | WHITEPIECE)) == (KING | WHITEPIECE)) whiteKing.x = endX, whiteKing.y = endY;
    if((board[endX][endY] & (KING | BLACKPIECE)) == (KING | BLACKPIECE)) blackKing.x = endX, blackKing.y = endY;

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
        if (!TestCheck(kingPos.x, kingPos.y, position.x, position.y)) return false;
        /*bool capable = true;
        for(int i = 0; i < 64; i++)
        {
            if((board[i % 8][i / 8] & 7) && !(board[i % 8][i / 8] & color) && CheckMove(i % 8, i / 8, position.x, position.y, false)) {capable = false; break;}
        }
        if(capable) return false;*/
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

/* TestCheck is called to see if a move will end in a board state in which the moving color is in check. This is both to check if the player
is positioning their own king in check and to force the player to escape an already existing check. The function should never be called while
the player is checkmated so there is no need to account for it. */
bool Board::TestCheck(int startX, int startY, int endX, int endY)
{
    char saveSquare = board[endX][endY];
    sf::Vector2i kingPreserve(startX, startY);

    bool isKing = (board[startX][startY] & 7) == KING;
    bool isWhitePiece = board[startX][startY] & WHITEPIECE;

    if(isKing) 
    {
        if(isWhitePiece) 
        {
            whiteKing = {endX, endY};
        }
        else
        {
            blackKing = {endX, endY};
        }
    }

    board[endX][endY] &= 192; board[endX][endY] |= (board[startX][startY] & 63);
    board[startX][startY] &= 192;

    bool ret = CheckAllChecks(board[endX][endY] & 48).x != -1;
    board[startX][startY] |= (board[endX][endY] & 63);
    board[endX][endY] = saveSquare;

    if(isKing)
    {
        if(isWhitePiece)
        {
            whiteKing = kingPreserve;
        }
        else
        {
            blackKing = kingPreserve;
        }
    }
    return ret;
}

/* CheckCheck function checks to see if a piece at a certain position and of a certain color is responsible for the opposing king being in check.
This function should be called after almost every move to determine if a checkmate should be checked for and should also be called during the checking of
a viable king move. */
int Board::CheckCheck(int x, int y, char color)
{
    return CheckMove(x, y, color == BLACKPIECE ? blackKing.x : whiteKing.x, color == BLACKPIECE ? blackKing.y : whiteKing.y, false);
}

//CheckMateCheck function checks to see if a specific color is in checkmate and will be called after every completed move
bool Board::CheckMateCheck(int x, int y)
{
    char color = (board[x][y] & 48) == BLACKPIECE ? WHITEPIECE : BLACKPIECE;
    sf::Vector2i checker = CheckAllChecks(color);

    //This function shouldn't be called in this case, but just to be sure
    if(checker.x == -1 || checker.y == -1) return false;

    //Before anything else, check to see if the king can move
    if(!MateCheck(color)) return false;

    //If king is mated and 2 pieces are placing it in check, king is checkmated
    if(checker.x == -2) return true;

    //If there is one piece responsible for checking, check to see if that piece can be captured
    for(int i = 0; i < 64; i++) if((board[i % 8][i / 8] & color) && CheckMove(i % 8, i / 8, checker.x, checker.y, false)) return false;

    //At this point, if a knight or pawn is responsible for the check, checkmate will be the same as mate according to my function checking
    if((board[checker.x][checker.y] & 7) == KNIGHT || (board[checker.x][checker.y] & 7) == PAWN) return true;

    /* At this point, only pieces that can be responsible for the check are bishop, queen, or rook. The path between this piece and the opposing king
    will be checked for potential check blocking. */
    int xStep = (color == WHITEPIECE ? whiteKing.x : blackKing.x) - checker.x;
    int yStep = (color == WHITEPIECE ? whiteKing.y : blackKing.y) - checker.y;
    xStep = xStep != 0 ? xStep / abs(xStep) : 0;
    yStep = yStep != 0 ? yStep / abs(yStep) : 0;

    for(int x = checker.x + xStep, y = checker.y + yStep; x != (color == WHITEPIECE ? whiteKing.x : blackKing.x) && y != (color == BLACKPIECE ? blackKing.y : blackKing.y);
        x+=xStep, y+=yStep) 
    {
        for(int i = 0; i < 64; i++) if((board[i % 8][i / 8] & color) && CheckMove(i % 8, i / 8, x, y, false)) return false;
    }

    //Checker can not be blocked or captured
    return true;
}

/* StaleMateCheck will be called after each move similarly to CheckMateCheck. The function will assume that it is called after CheckMateCheck returns false.
For the time being, stalemate will be the only method of draw that is checked for. */
bool Board::StaleMateCheck(int x, int y)
{
    char color = (board[x][y] & 48) == BLACKPIECE ? WHITEPIECE : BLACKPIECE;
    
    /* For now, I'm only going to check to see if the King is the only piece left. There are many niche cases to check for in which
    there are no legal moves and there exists more than a King. */
    for(int i = 0; i < 64; i++){
        if((board[i / 8][i % 8] & 7) != KING && (board[i / 8][i % 8] & color))
        {
            return false;
        }
    }
    return MateCheck(color);
}