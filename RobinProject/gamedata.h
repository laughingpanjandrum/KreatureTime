#pragma once

#include "actor.h"
#include "attributes.h"
#include "fileloader.h"



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


	//	Dialogue
	dialogueManager dman;
	dialoguePtr currentDialogue;

};
typedef shared_ptr<gamedata> gamedataPtr;