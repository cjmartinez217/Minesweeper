#include "TextureManager.h"

// redeclare static variable
unordered_map<string, sf::Texture> TextureManager::textures;

void TextureManager::LoadTexture(string fileName)
{
	string path = "images/";
	path += fileName + ".png";

	textures[fileName].loadFromFile(path);
}

sf::Texture& TextureManager::GetTexture(string textureName)
{
	//if texture dont exist, load first then return
	if (textures.find(textureName) == textures.end())
		LoadTexture(textureName);

	return textures[textureName];
}

void TextureManager::Clear()
{
	textures.clear(); //get rid of stored objects
}
