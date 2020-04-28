#include "pch.h"
#include "exploration.h"


//	Returns the location instance with the given ID, if it exists.
locationPtr getLocationById(locationManager * lman, const string id)
{
	for (auto loc : lman->all)
	{
		if (loc->id == id)
			return loc;
	}
	cout << "ERROR: Can't find location with ID " << id << endl;
	return nullptr;
}



//	Sets the position of the foreground/background sprites.
void positionBackground(spritePtr bg, spritePtr fg, const int x, const int y)
{
	bg->setPosition(x, y);
	fg->setPosition(x, y);
}


//	Sets the given sprite's position as an offset from the top-left of the given background sprite.
void positionSpriteInFrame(spritePtr spr, spritePtr frame, const int x, const int y)
{
	auto frame_pos = frame->getPosition();
	int nx = x + frame_pos.x;
	int ny = y + frame_pos.y;
	spr->setPosition(nx, ny);
}


//	Set the current frame data.
void setCurrentFrame(gamedataPtr gdata, lframePtr frame)
{
	gdata->currentFrame = frame;
	positionBackground(gdata->bg_sprite, gdata->fg_sprite, -frame->framex, -frame->framey);
	positionSpriteInFrame(gdata->playerSprite, gdata->bg_sprite, frame->playerx, frame->playery);
}



//	Shift the current view until it's aligned with the current frame.
void stepFrameTransition(gamedataPtr gdata)
{
	sf::Vector2i vec;
	vec.x = 0; vec.y = 0;
	

	//	get direction to correct frame position
	auto cpos = gdata->bg_sprite->getPosition();


	//	difference between our desired position & the current frame position
	int xdiff = cpos.x + gdata->currentFrame->framex;
	int ydiff = cpos.y + gdata->currentFrame->framey;

	if		(xdiff < 0)	vec.x = FRAME_TRANSITION_RATE;
	else if (xdiff > 0)	vec.x = -FRAME_TRANSITION_RATE;
	if		(ydiff < 0)	vec.y = FRAME_TRANSITION_RATE;
	else if (ydiff > 0)	vec.y = -FRAME_TRANSITION_RATE;


	//	shift the frame
	cpos.x += vec.x;
	cpos.y += vec.y;
	positionBackground(gdata->bg_sprite, gdata->fg_sprite, cpos.x, cpos.y);


	//	shift the player sprite and all NPC sprites so they maintains their positions
	gdata->playerSprite->move(vec.x, vec.y);
	for (auto npc : gdata->npcSprites)
		npc->move(vec.x, vec.y);


	//	if we've reached the desired position, stop transitioning & set the player's final position.
	//	we consider ourselves at the destination point if the absolute distance along each axis is less than the transition distance.
	if (abs(xdiff) <= FRAME_TRANSITION_RATE && abs(ydiff) <= FRAME_TRANSITION_RATE)
	{
		gdata->inFrameTransition = false;
		positionSpriteInFrame(gdata->playerSprite, gdata->bg_sprite, gdata->currentFrame->playerx, gdata->currentFrame->playery);
	}
}



//	Cycles to the 'next' or 'last' frame.
void cycleFrame(gamedataPtr gdata, int* frameIdx, int vec)
{
	//	change frame index, making sure it stays within legal bounds
	*frameIdx += vec;
	if (*frameIdx < 0)
		*frameIdx = 0;
	else if (*frameIdx >= gdata->currentLocation->frames.size())
		*frameIdx = gdata->currentLocation->frames.size() - 1;

	//	start a frame transition animation
	gdata->currentFrame = gdata->currentLocation->frames[*frameIdx];
	gdata->inFrameTransition = true;
	//setCurrentFrame(gdata, gdata->currentLocation->frames[*frameIdx]);
}



//	Main loop for the EXPLORATION game
void explorationLoop(gamedataPtr gdata)
{
	//	Current location.
	auto loc = gdata->currentLocation;

	//	Get the foreground/background textures for this location.
	cout << "Loading texture file " << loc->bg_texture << endl;
	texturePtr bg = getTextureFromFile("loc/" + loc->bg_texture);
	cout << "Loading texture file " << loc->fg_texture << endl;
	texturePtr fg = getTextureFromFile("loc/" + loc->fg_texture);


	//	Highlighting marks
	sf::Sprite bang;
	auto bang_tx = getTextureFromFile("bang.png");
	bang.setTexture(*bang_tx);
	bang.setPosition(-50, -50);


	//	Create sprites to contain these textures.
	gdata->bg_sprite = spritePtr(new sf::Sprite());
	gdata->fg_sprite = spritePtr(new sf::Sprite());
	gdata->bg_sprite->setTexture(*bg);
	gdata->fg_sprite->setTexture(*fg);
	gdata->bg_sprite->setOrigin(0, 0);
	gdata->fg_sprite->setOrigin(0, 0);


	//	Load NPC sprites
	gdata->npcSprites.clear();
	for (auto npcdat : loc->npcs)
	{
		auto npc_spr = createNPCSprite(npcdat->npcId, &gdata->nman);
		npc_spr->setOrigin(0, 0);
		gdata->npcSprites.push_back(npc_spr);
	}


	//	Initial frame
	int frameIdx = 0;
	setCurrentFrame(gdata, loc->frames[frameIdx]);
	for (unsigned i = 0; i < loc->npcs.size(); i++)
		positionSpriteInFrame(gdata->npcSprites[i], gdata->bg_sprite, loc->npcs[i]->x, loc->npcs[i]->y);


	//	The principal loop.
	bool done = false;
	sf::Event event;
	while (!done)
	{
		//	get input
		while (gdata->rwindow->pollEvent(event))
		{
			if (event.type == sf::Event::EventType::Closed)
				done = true;


			//	Quit w/ ESCAPE
			if (event.type == sf::Event::EventType::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
					done = true;


				//	test frame transition
				else if (event.key.code == sf::Keyboard::Right)
					cycleFrame(gdata, &frameIdx, 1);
				else if (event.key.code == sf::Keyboard::Left)
					cycleFrame(gdata, &frameIdx, -1);
			}


			//	Handle clicks
			else if (event.type == sf::Event::EventType::MouseButtonReleased && !gdata->inFrameTransition)
			{
				auto mpos = sf::Mouse::getPosition(*gdata->rwindow);
				
				//	Did we click on a creature?
				for (unsigned i = 0; i < loc->npcs.size(); i++)
				{
					if (gdata->npcSprites[i]->getGlobalBounds().contains(mpos.x, mpos.y))
					{
						//	begin a dialogue, if one is defined
						auto dialogueId = loc->npcs[i]->dialogueId;
						if (!dialogueId.empty())
						{
							//	get the dialogue
							gdata->currentDialogue = getDialogueById(&gdata->dman, dialogueId);
							if (gdata->currentDialogue != nullptr)
							{
								//	save the player's current position
								auto ppos = gdata->playerSprite->getPosition();

								//	execute the dialogue
								loadingscreen(gdata);
								dialogueLoop(gdata);

								//	restore the player's previous position upon exiting dialogue
								gdata->playerSprite->setPosition(ppos);
							}
						}
					}
				}
			}
		}




		//	if the frame is currently transitioning, we shift the view-window
		if (gdata->inFrameTransition)
			stepFrameTransition(gdata);



		//	render the display
		gdata->rwindow->clear();

		//	background on the BOTTOM
		gdata->rwindow->draw(*gdata->bg_sprite);

		//	draw NPC's: check if we've moused over any of them
		auto mpos = sf::Mouse::getPosition(*gdata->rwindow);
		for (auto npc : gdata->npcSprites)
		{
			gdata->rwindow->draw(*npc);
			if (npc->getGlobalBounds().contains(mpos.x, mpos.y))
			{
				bang.setPosition(npc->getPosition());
				gdata->rwindow->draw(bang);
			}
		}

		//	player sprite
		gdata->rwindow->draw(*gdata->playerSprite);

		//	foreground on the TOP
		gdata->rwindow->draw(*gdata->fg_sprite);

		gdata->rwindow->display();
	}
}
