#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "TextureManager.h"
#include "Random.h"
#include "Board.h"
#include "Tile.h"

int main()
{
    int columns, rows, mines;
    ifstream inFile("boards/config.cfg");
    if (inFile.is_open())
    {
        inFile >> columns >> rows >> mines;
    }
    inFile.close();
    Board board = Board(columns, rows, mines);

    //create window
    sf::RenderWindow window(sf::VideoMode(board.columns * 32, board.rows * 32 + 88), "Minesweeper");

    //run program if window is open
    while (window.isOpen())
    {
        window.clear();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            //if mouse is pressed
            if (event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2i position = sf::Mouse::getPosition(window);
                int xIndex = position.x / 32;
                int yIndex = position.y / 32;
                //right button
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    //if game not over keep checking mouse clicks
                    if (board.inSession)
                    {
                        for (int i = 0; i < board.rows; i++)
                        {
                            for (int j = 0; j < board.columns; j++)
                            {
                                if ((xIndex == j && yIndex == i) && board.tiles[j][i].hidden)
                                {
                                    if (board.tiles[j][i].flagged)
                                    {
                                        board.tiles[j][i].flagged = false;
                                        board.minesLeft++;
                                    }
                                    else
                                    {
                                        board.tiles[j][i].flagged = true;
                                        board.minesLeft--;
                                    }
                                }
                            }
                        }
                    }
                }
                //left button
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    //if game not over keep checking mouse clicks
                    if (board.inSession)
                    {
                        for (int i = 0; i < board.rows; i++)
                        {
                            for (int j = 0; j < board.columns; j++)
                            {
                                if ((xIndex == j && yIndex == i) && (!board.tiles[j][i].flagged))
                                {
                                    if (board.tiles[xIndex][yIndex].mine) //check if spot is a mine
                                    {
                                        //print all mines if you lose
                                        for (int i = 0; i < board.rows; i++)
                                        {
                                            for (int j = 0; j < board.columns; j++)
                                            {
                                                if (board.tiles[j][i].mine)
                                                    board.tiles[j][i].hidden = false;
                                            }
                                        }
                                        board.mode = "lose";
                                        board.reset.setTexture(TextureManager::GetTexture("face_lose"));
                                        board.inSession = false;
                                    }
                                    board.checkEmptyAdj(&board.tiles[j][i]);
                                    board.tiles[j][i].hidden = false;
                                }
                            }
                        }
                        //debug button
                        if (board.debug.getGlobalBounds().contains(position.x, position.y))
                        {
                            if (!board.debugMode) //if board is not in debug mode
                            {
                                board.debugMode = true;
                                for (int i = 0; i < board.rows; i++)
                                {
                                    for (int j = 0; j < board.columns; j++)
                                    {
                                        if (board.tiles[j][i].mine && !board.tiles[j][i].flagged)
                                            board.tiles[j][i].hidden = false;
                                    }
                                }
                            }
                            else //board was previously in debug mode
                            {
                                board.debugMode = false;
                                for (int i = 0; i < board.rows; i++)
                                {
                                    for (int j = 0; j < board.columns; j++)
                                    {
                                        if (board.tiles[j][i].mine)
                                            board.tiles[j][i].hidden = true;
                                    }
                                }
                            }
                        }
                    }

                    //checking what button is clicked
                    //reset button
                    bool tempBool = board.debugMode;
                    if (board.test_1.getGlobalBounds().contains(position.x, position.y))
                    {
                        board = board.testCase("boards/testBoard1.brd");
                        board = board.testCase("boards/testBoard1.brd");
                    }
                    if (board.test_2.getGlobalBounds().contains(position.x, position.y))
                    {
                        board = board.testCase("boards/testBoard2.brd");
                        board = board.testCase("boards/testBoard2.brd");

                    }
                    if (board.test_3.getGlobalBounds().contains(position.x, position.y))
                    {
                        board = board.testCase("boards/testBoard3.brd");
                        board = board.testCase("boards/testBoard3.brd");
                    }

                    if (board.reset.getGlobalBounds().contains(position.x, position.y))
                    {
                        board = board.shuffleBoard();
                        board.mode = "happy";
                        board.reset.setTexture(TextureManager::GetTexture("face_happy"));
                        board.inSession = true;
                    }
                    if (tempBool) //if board was already in debug mode keep it that way
                    {
                        board.debugMode = true;
                        for (int i = 0; i < board.rows; i++)
                        {
                            for (int j = 0; j < board.columns; j++)
                            {
                                if (board.tiles[j][i].mine && !board.tiles[j][i].flagged)
                                    board.tiles[j][i].hidden = false;
                            }
                        }
                    }
                }
            }

        }
        int tilesRevealed = 0;
        for (int i = 0; i < board.rows; i++)
        {
            for (int j = 0; j < board.columns; j++)
            {
                if (!board.tiles[j][i].hidden)
                    tilesRevealed++;
            }
        }
        if (tilesRevealed == (board.numTiles - board.minesOrig) && board.mode != "lose" && !board.debugMode)
        {
            board.mode = "win";
            board.reset.setTexture(TextureManager::GetTexture("face_win"));
            board.inSession = false;
            for (int i = 0; i < board.rows; i++)
            {
                for (int j = 0; j < board.columns; j++)
                {
                    if (board.tiles[j][i].hidden)
                        board.tiles[j][i].flagged = true;
                }
            }
            board.minesLeft = 0;
        }

        board.printBoard(window);
        
        window.display();
    }

    TextureManager::Clear();

    return 0;
}