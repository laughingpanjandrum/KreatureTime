#pragma once

#include "actor.h"
#include "attributes.h"



struct gamedata
{
	//	Render details
	sf::RenderWindow* rwindow;
	sf::Font usefont;
	

	//	Player data
	player_image* pimage;
	spritePtr playerSprite;
	texturePtr playerTexture;

	sf::String playerName;
	playerdataPtr playerData;


	//	Randomizer
};
typedef shared_ptr<gamedata> gamedataPtr;