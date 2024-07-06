#include <iostream>
#include "board.h"
#include <SFML/Graphics.hpp>

int main(){

    sf::RenderWindow window(sf::VideoMode(800, 800), "Chessboard");

    sf::Texture example;
    example.loadFromFile("images/pieces.png");

    sf::Sprite spritePieces[32];

    Board blah(example);

    bool moving = false;
    int clickedPiece = -1;
    int startX, startY, endX, endY;
    float movingX, movingY;
    sf::Vector2f mousePosition, piecePosition;
    float squareSize = static_cast<float>(window.getSize().x) / 8;

    window.setFramerateLimit(60);
    window.clear();
    blah.ResetBoard();
    blah.RenderPieces(window, spritePieces, squareSize);

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) 
            {
                if (event.mouseButton.button == sf::Mouse::Left) 
                {
                    
                    mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                    for(int i = 0; i < 32; i++)
                    {
                        if(spritePieces[i].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
                        {
                            moving = true;
                            clickedPiece = i;
                            piecePosition = spritePieces[i].getPosition();
                            startX = piecePosition.x / squareSize;
                            startY = piecePosition.y / squareSize;
                            movingX = mousePosition.x - piecePosition.x;
                            movingY = mousePosition.y - piecePosition.y;
                            break;
                        }
                    }
                    continue;
                }
            }

            if (event.type == sf::Event::MouseButtonReleased) 
            {
                if (event.mouseButton.button == sf::Mouse::Left) 
                {
                    if(clickedPiece == -1) continue;
                    piecePosition = spritePieces[clickedPiece].getPosition();
                    endX = (piecePosition.x + movingX) / squareSize;
                    endY = (piecePosition.y + movingY) / squareSize;
                    switch(blah.CheckMove(startX, startY, endX, endY, true)){
                        case 4:
                            //En Passant
                            for(int i = 0; i < 32; i++)
                            {
                                if(spritePieces[i].getGlobalBounds().contains(piecePosition.x + movingX, startY * squareSize))
                                {
                                    blah.MakeMove(startX, startY, endX, startY, squareSize, spritePieces[clickedPiece], spritePieces[i]);
                                    break;
                                }
                            }
                            blah.MakeMove(endX, startY, endX, endY, squareSize, spritePieces[clickedPiece]);
                            break;
                        case 3:
                            //Castle
                            if(startY == 0)
                            {
                                blah.MakeMove(4, 0, 6, 0, squareSize, spritePieces[4]);
                                blah.MakeMove(7, 0, 5, 0, squareSize, spritePieces[7]);
                            }
                            else
                            {
                                blah.MakeMove(4, 7, 6, 7, squareSize, spritePieces[28]);
                                blah.MakeMove(7, 7, 5, 7, squareSize, spritePieces[31]);
                            }
                            break;
                        case 2:
                            //Capture a Piece
                            for(int i = 0; i < 32; i++)
                            {
                                if(i == clickedPiece) continue;
                                if(spritePieces[i].getGlobalBounds().contains(piecePosition.x + movingX, piecePosition.y + movingY))
                                {
                                    blah.MakeMove(startX, startY, endX, endY, squareSize, spritePieces[clickedPiece], spritePieces[i]);
                                    break;
                                }
                            }
                            break;
                        case 1:
                            //Move Without Capturing
                            blah.MakeMove(startX, startY, endX, endY, squareSize, spritePieces[clickedPiece]);
                            break;
                        case 0:
                            //Illegal Move
                            spritePieces[clickedPiece].setPosition(squareSize * startX, squareSize * startY);
                    }
                    if(blah.CheckMateCheck(endX, endY))
                    {
                        blah.ResetBoard();
                        blah.RenderPieces(window, spritePieces, squareSize);
                    }
                    moving = false;
                    clickedPiece = -1;
                }
            }
            if (event.type == sf::Event::MouseMoved)
            {
                if (moving && clickedPiece != -1)
                {
                    mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    spritePieces[clickedPiece].setPosition(mousePosition.x - movingX, mousePosition.y - movingY);
                }
            }
        }
        window.clear();
        blah.RenderBoard(window, squareSize);
        for(int i = 0; i < 32; i++) window.draw(spritePieces[i]);
        if(clickedPiece > -1) window.draw(spritePieces[clickedPiece]);
        window.display();
    }

}