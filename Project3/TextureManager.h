#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
using std::unordered_map;
using std::string;

class TextureManager
{
	static unordered_map<string, sf::Texture> textures;
	static void LoadTexture(string textureName); //LoadTexture("space") no need for rest of address
public:
	static sf::Texture& GetTexture(string textureName);
	static void Clear(); // Call once at end of main, not creating instance so no destructor called
};

