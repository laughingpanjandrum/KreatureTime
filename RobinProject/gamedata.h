#pragma once

#include "actor.h"



struct gamedata
{
	sf::RenderWindow* rwindow;
	player_image* pimage;
};
typedef shared_ptr<gamedata> gamedataPtr;