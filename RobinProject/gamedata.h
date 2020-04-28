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


	//	NPC data
	npcDataManager nman;


	//	Dialogue
	dialogueManager dman;
	dialoguePtr currentDialogue;
	spritePtr dialogue_frame_1;
	spritePtr dialogue_frame_2;


	//	Locations
	locationManager lman;
	locationPtr currentLocation;


	//	Current position in a location
	vector<spritePtr> npcSprites;
	lframePtr currentFrame;
	spritePtr bg_sprite;
	spritePtr fg_sprite;

	bool inFrameTransition = false;
	sf::Vector2i frame_trans_vec;

};
typedef shared_ptr<gamedata> gamedataPtr;



inline void loadingscreen(gamedataPtr gdata)
{
	auto tx = sf::Text();
	tx.setFont(gdata->usefont);
	tx.setString("Loading...");
	tx.setPosition(350, 250);
	tx.setColor(sf::Color(0, 0, 0));
	
	gdata->rwindow->clear(sf::Color(255, 255, 255));
	gdata->rwindow->draw(tx);
	gdata->rwindow->display();
}
