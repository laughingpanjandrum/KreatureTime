

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
#include "fileloader.h"
#include "dialogue.h"
#include "exploration.h"

using namespace std;




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


	//	loading game data
	dialogueLoader::loadFile("test1.d", &gdata->dman);
	locationLoader::loadFile("airship.loc", &gdata->lman);
	npcLoader::loadFile("npc.dat", &gdata->nman);


	loadingscreen(gdata);


	//	character creation
	charCreationLoop(gdata);
	//charViewerLoop(gdata);

	loadingscreen(gdata);

	gdata->currentLocation = getLocationById(&gdata->lman, "airship");
	explorationLoop(gdata);


	/*auto d = getDialogueById(&gdata->dman, "test1");
	gdata->currentDialogue = d;
	dialogueLoop(gdata);*/

	return 1;
}