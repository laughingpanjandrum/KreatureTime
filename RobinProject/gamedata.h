#pragma once

#include "actor.h"



struct gamedata
{
	//	Render details
	sf::RenderWindow* rwindow;
	sf::Font usefont;
	
	//	Player data
	player_image* pimage;
	sf::String playerName;
	spritePtr playerSprite;
	texturePtr playerTexture;
};
typedef shared_ptr<gamedata> gamedataPtr;