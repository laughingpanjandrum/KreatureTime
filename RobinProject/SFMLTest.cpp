

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
#include "charviewer.h"

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
	initrand();

	//	base game data
	auto gdata = gamedataPtr(new gamedata());
	gdata->pimage = new player_image();
	gdata->rwindow = new sf::RenderWindow(sf::VideoMode(800, 600), "Kreature Time");


	//	font to be used
	gdata->usefont = sf::Font();
	gdata->usefont.loadFromFile("data/Old Typewriter2.0.ttf");


	//	character creation
	charCreationLoop(gdata);
	charViewerLoop(gdata);
}