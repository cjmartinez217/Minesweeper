#include "Board.h"
#include "Random.h"
#include <iostream>
#include <fstream>
using namespace std;

Board::Board(int columns, int rows, int mines)
{
	this->columns = columns;
	this->rows = rows;
	this->minesOrig = mines;
	this->numTiles = columns * rows;
	minesLeft = minesOrig;
	int x, y;
	for (int i = 0; i < columns; i++)
	{
		vector<Tile> tempColumn;
		for (int j = 0; j < rows; j++)
		{
			Tile newTile = Tile();
			tempColumn.push_back(newTile);
		}
		tiles.push_back(tempColumn);
	}
	for (int i = 0; i < mines; i++)
	{
		x = Random::Int(0, columns - 1);
		y = Random::Int(0, rows - 1);
		tiles[x][y].changeType("mine");
		tiles[x][y].mine = true;
	}
	//set neighbors for each tile
	setNeighbors();

	//change tiles to numbers if they are by mines
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			tiles[j][i].setNeighborNumber();
		}
	}

	//set other modes
	debugMode = false;
	inSession = true;
	mode = "happy";

	//put sprite buttons in right place
	string face = "face_" + mode;
	reset.setTexture(TextureManager::GetTexture(face));
	debug.setTexture(TextureManager::GetTexture("debug"));
	test_1.setTexture(TextureManager::GetTexture("test_1"));
	test_2.setTexture(TextureManager::GetTexture("test_2"));
	test_3.setTexture(TextureManager::GetTexture("test_3"));
}

void Board::setNeighbors()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			if (i != 0)
			{
				tiles[j][i].neighbors.push_back(&tiles[j][i - 1]);
				if (j != columns - 1)
					tiles[j][i].neighbors.push_back(&tiles[j + 1][i - 1]);
			}
			if (j != columns - 1)
			{
				tiles[j][i].neighbors.push_back(&tiles[j + 1][i]);
				if (i != rows - 1)
					tiles[j][i].neighbors.push_back(&tiles[j + 1][i + 1]);
			}
			if (i != rows - 1)
			{
				tiles[j][i].neighbors.push_back(&tiles[j][i + 1]);
				if (j != 0)
					tiles[j][i].neighbors.push_back(&tiles[j - 1][i + 1]);
			}
			if (j != 0)
			{
				tiles[j][i].neighbors.push_back(&tiles[j - 1][i]);
				if (i != 0)
					tiles[j][i].neighbors.push_back(&tiles[j - 1][i - 1]);
			}
		}
	}
}

Board Board::shuffleBoard()
{
	int tempColumns, tempRows, tempMines;
	ifstream inFile("boards/config.cfg");
	if (inFile.is_open())
	{
		inFile >> tempColumns >> tempRows >> tempMines;
	}
	Board newBoard = Board(tempColumns, tempRows, tempMines);
	inFile.close();
	return newBoard;
}

void Board::printBoard(sf::RenderWindow& window)
{
	//draw tiles
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			sf::Sprite tileSprite;
			//set texture to sprite
			//tile hidden
			if (tiles[j][i].hidden)
			{
				tileSprite.setTexture(TextureManager::GetTexture("tile_hidden"));
				tileSprite.setPosition(j * 32, i * 32);
				window.draw(tileSprite);
				//if tile is flagged
				if (tiles[j][i].flagged)
				{
					tileSprite.setTexture(TextureManager::GetTexture("flag"));
					tileSprite.setPosition(j * 32, i * 32);
					window.draw(tileSprite);
				}
			}
			//tile not hidden
			else
			{
				tileSprite.setTexture(TextureManager::GetTexture("tile_revealed"));
				tileSprite.setPosition(j * 32, i * 32);
				window.draw(tileSprite);
				tiles[j][i].setPosition(j * 32, i * 32);
				tiles[j][i].drawTile(window);
			}

		}
	}

	//draw mine count
	sf::Sprite sprite;
	int tempNum = abs(minesLeft);
	for (int i = 0; i < 3; i++)
	{
		int digit = tempNum % 10;
		sprite.setTextureRect(sf::IntRect(digit * 21, 0, 21, 32));
		sprite.setTexture(TextureManager::GetTexture("digits"));
		sprite.setPosition(63 - 21 * i, rows * 32);
		window.draw(sprite);
		tempNum /= 10;
	}
	if (minesLeft < 0)
	{
		sprite.setTextureRect(sf::IntRect(210, 0, 21, 32));
		sprite.setTexture(TextureManager::GetTexture("digits"));
		sprite.setPosition(21, rows * 32);
		window.draw(sprite);
	}

	//draw buttons
	sf::Vector2u size = window.getSize();
	reset.setPosition(size.x / 2 - 32, rows * 32);
	window.draw(reset);
	//debug
	debug.setPosition(size.x / 2 + 96, rows * 32);
	window.draw(debug);
	//tests
	test_1.setPosition(size.x / 2 + 160, rows * 32);
	window.draw(test_1);
	test_2.setPosition(size.x / 2 + 224, rows * 32);
	window.draw(test_2);
	test_3.setPosition(size.x / 2 + 288, rows * 32);
	window.draw(test_3);
}

void Board::checkEmptyAdj(Tile* tile)
{
	if (tile->hidden && !tile->flagged)
	{
		if (tile->numNeighborMines == 0)
		{
			tile->hidden = false;
			for (int i = 0; i < tile->neighbors.size(); i++)
			{
				checkEmptyAdj(tile->neighbors[i]);
			}
		}
		else
		{
			string tileTemp = "number_";
			tileTemp += to_string(tile->numNeighborMines);
			tile->changeType(tileTemp);
			tile->hidden = false;
		}
	}
}

Board Board::testCase(string fileName)
{
	Board tempBoard = Board();
	int mines = 0;
	ifstream inFile(fileName);
	int tempArray[25][16];
	if (inFile.is_open())
	{
		for (int i = 0; i < 16; i++)
		{
			for (int j = 0; j < 25; j++)
			{
				char tempNum = inFile.get();
				tempArray[j][i] = tempNum - '0';
			}
			inFile.get();
		}
	}
	inFile.close();
	for (int i = 0; i < 25; i++)
	{
		vector<Tile> tempColumn;
		for (int j = 0; j < 16; j++)
		{
			Tile tempTile;
			if (tempArray[i][j] == 1)
			{
				tempTile.mine = true;
				tempTile.changeType("mine");
				mines++;
			}
			tempColumn.push_back(tempTile);
		}
		tempBoard.tiles.push_back(tempColumn);
	}

	tempBoard.columns = 25;
	tempBoard.rows = 16;
	tempBoard.minesOrig = mines;
	tempBoard.numTiles = columns * rows;
	tempBoard.minesLeft = minesOrig;
	//set neighbors for each tile
	tempBoard.setNeighbors();

	//change tiles to numbers if they are by mines
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			tempBoard.tiles[j][i].setNeighborNumber();
		}
	}

	//set other modes
	tempBoard.debugMode = false;
	tempBoard.inSession = true;
	tempBoard.mode = "happy";

	//put sprite buttons in right place
	string face = "face_" + mode;
	tempBoard.reset.setTexture(TextureManager::GetTexture(face));
	tempBoard.debug.setTexture(TextureManager::GetTexture("debug"));
	tempBoard.test_1.setTexture(TextureManager::GetTexture("test_1"));
	tempBoard.test_2.setTexture(TextureManager::GetTexture("test_2"));
	tempBoard.test_3.setTexture(TextureManager::GetTexture("test_3"));

	return tempBoard;
}

void Board::setMineLocations(int tempArray[25][16])
{
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 25; j++)
		{
			if (tempArray[j][i] == 1)
			{
				tiles[j][i].mine = true;
				tiles[j][i].changeType("mine");
			}
			else
			{
				tiles[j][i].mine = false;
				tiles[j][i].changeType("tile_revealed");
			}
		}
	}
}

Board::Board()
{
}
