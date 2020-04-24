

#include "pch.h"
#include <iostream>
#include <random>
#include <vector>

#include "SFML/Graphics.hpp"
#include "imageloader.h"
#include "actor.h"
#include "drawing.h"
#include "gamedata.h"
#include "charcreation.h"

using namespace std;




void looplayer(player_image* pimage, const unsigned layer_idx, vector<texturePtr>* txlist, int* idx)
{
	(*idx)++;
	if (*idx >= txlist->size())
		*idx = 0;
	pimage->setLayerTexture(layer_idx, txlist->at(*idx));
}



int main()
{
	//sf::RenderWindow renderWindow(sf::VideoMode(640, 480), "Hello SFML");

	//	initrand
	std::uniform_int_distribution<int> randomColorRange(0, 255);
	std::random_device rd;
	std::mt19937 randomNumbers(rd());


	//	base game data
	auto gdata = gamedataPtr(new gamedata());
	gdata->pimage = new player_image();
	gdata->rwindow = new sf::RenderWindow(sf::VideoMode(800, 600), "Kreature Time");

	charCreationLoop(gdata);
}