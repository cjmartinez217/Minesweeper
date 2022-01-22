#include "Tile.h"
#include "Board.h"

Tile::Tile()
{
	tile.setTexture(TextureManager::GetTexture("tile_revealed"));
	hidden = true;
	flagged = false;
	mine = false;
}

void Tile::changeType(string newType)
{
	tile.setTexture(TextureManager::GetTexture(newType));
}

void Tile::drawTile(sf::RenderWindow& window)
{
	window.draw(tile);
}

void Tile::setPosition(int x, int y)
{
	tile.setPosition(x, y);
}

int Tile::setNeighborNumber()
{
	numNeighborMines = 0;
	for (int i = 0; i < neighbors.size(); i++)
	{
		if (neighbors[i]->mine)
			numNeighborMines++;
	}
	string newType = "number_" + to_string(numNeighborMines);
	if(numNeighborMines > 0 && !mine)
		changeType(newType);
	return numNeighborMines;
}



