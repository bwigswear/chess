#include <iostream>
#include "board.h"
#include <SFML/Graphics.hpp>

int main(){

    sf::RenderWindow window(sf::VideoMode(1200, 1200), "Chessboard");

    sf::Texture example;
    example.loadFromFile("images/pieces.png");

    sf::Sprite spritePieces[32];

    Board blah(example);

    bool moving = false;
    int clickedPiece = -1;
    int startX, startY;
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
                            fflush(stdout);
                            break;
                        }
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonReleased) 
            {
                if (event.mouseButton.button == sf::Mouse::Left) 
                {
                    piecePosition = spritePieces[clickedPiece].getPosition();
                    switch(blah.CheckMove(startX, startY, (piecePosition.x + movingX) / squareSize, (piecePosition.y + movingY) / squareSize)){
                        case 2:
                            for(int i = 0; i < 32; i++)
                            {
                                if(i == clickedPiece) continue;
                                if(spritePieces[i].getGlobalBounds().contains(piecePosition.x + movingX, piecePosition.y + movingY))
                                {
                                    blah.MakeMove(startX, startY, (piecePosition.x + movingX) / squareSize, (piecePosition.y + movingY) / squareSize, squareSize, spritePieces[clickedPiece], spritePieces[i]);
                                    break;
                                }
                            }
                            break;
                        case 1:
                            blah.MakeMove(startX, startY, (piecePosition.x + movingX) / squareSize, (piecePosition.y + movingY) / squareSize, squareSize, spritePieces[clickedPiece]);
                            break;
                        case 0:
                            spritePieces[clickedPiece].setPosition(squareSize * startX, squareSize * startY);
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