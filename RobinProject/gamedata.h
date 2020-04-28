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

	//	Locations
	locationManager lman;
	locationPtr currentLocation;

	//	Current position in a location
	lframePtr currentFrame;
	spritePtr bg_sprite;
	spritePtr fg_sprite;

	bool inFrameTransition = false;
	sf::Vector2i frame_trans_vec;

};
typedef shared_ptr<gamedata> gamedataPtr;