#pragma once
#include <array>
#include "Tile.h"
using namespace std;

struct Board : private Tile
{
	sf::Sprite reset, debug, test_1, test_2, test_3;
	int columns, rows, minesLeft, numTiles, minesOrig;
	bool debugMode, inSession;
	string mode;
	vector<vector<Tile>> tiles;
	Board(int columns, int rows, int mines);
	Board();
	void setNeighbors();
	Board shuffleBoard();
	void printBoard(sf::RenderWindow& window);
	void checkEmptyAdj(Tile* tile);
	Board testCase(string fileName);
	void setMineLocations(int tempArray[25][16]);
};

