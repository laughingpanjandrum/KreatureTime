#pragma once

#include "pch.h"
#include <iostream>
#include <random>
#include <vector>


#include "SFML/Graphics.hpp"

using namespace std;


#define DEFAULT_TX_WIDTH	200
#define DEFAULT_TX_HEIGHT	300



typedef shared_ptr<sf::Texture> texturePtr;
typedef shared_ptr<sf::Sprite> spritePtr;


struct texture_library
{
	vector<string> labels;
	vector<texturePtr> textures;
};


//	getting textures from sheets

texturePtr getTextureByLabel(texture_library* tlib, const string label);


//	generating texture libraries

void addTextureToLibrary(texture_library* tlib, const string label, texturePtr tx);

void createTextureAndAdd(texture_library* tlib, sf::Image* img, const string label, const int x, const int y, 
	const int w = DEFAULT_TX_WIDTH, const int h = DEFAULT_TX_HEIGHT);

texturePtr getTextureFromImage(sf::Image* img, const int x, const int y, const int w = DEFAULT_TX_WIDTH, const int h = DEFAULT_TX_HEIGHT);



//	generating texture libraries
texture_library* getCreatureLibrary();
texture_library* getButtonLibrary();