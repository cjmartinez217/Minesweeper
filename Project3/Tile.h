#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
using namespace std;

struct Tile
{
	sf::Sprite tile;
	bool hidden, flagged, mine;
	int numNeighborMines;
	vector<Tile*> neighbors;
	Tile();
	void changeType(string newType);
	void drawTile(sf::RenderWindow& window);
	void setPosition(int x, int y);
	int setNeighborNumber();
};

