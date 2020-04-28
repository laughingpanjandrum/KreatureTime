#include "pch.h"
#include "charcreation.h"



//	Cycles through a character feature list.
void loopCharacterFeature(gamedataPtr gdata, const unsigned layer_index, vector<texturePtr>* txlist, int * idx, int vec)
{
	(*idx) += vec;
	if ((*idx) >= txlist->size())
		*idx = 0;
	else if ((*idx) < 0)
		*idx = txlist->size() - 1;
	gdata->pimage->setLayerTexture(layer_index, txlist->at(*idx));
}



//	Renders the character's layers into a single texture, which is then set to the 'playerSprite' object in gamedata.
void finishCharacter(gamedataPtr gdata)
{
	//	player object
	gdata->playerData = playerdataPtr(new playerdata());

	//	create the player sprite
	gdata->playerSprite = spritePtr(new sf::Sprite());

	//	render the texture
	auto rtx = gdata->pimage->flatten();
	gdata->playerTexture = texturePtr(new sf::Texture(rtx->getTexture()));
	gdata->playerSprite->setTexture(*gdata->playerTexture);
}



void charCreationLoop(gamedataPtr gdata)
{
	bool done = false;
	sf::Event event;


	//	Set up a textbox for naming
	sf::Text name_text_box;
	name_text_box.setFont(gdata->usefont);
	name_text_box.setCharacterSize(30);
	name_text_box.setColor(sf::Color(120, 120, 120));
	name_text_box.setPosition(85, 540);
	name_text_box.setString("Enter Name");
	bool startedName = false;


	//	texture library for the player character
	auto tlib = getCreatureLibrary();
	gdata->pimage->setBodyTexture(getTextureByLabel(tlib, "base"));
	gdata->pimage->setPosition(215, 60);


	//	tables of image options
	vector<texturePtr> tx_eyes = { getTextureByLabel(tlib, "empty") };
	vector<texturePtr> tx_noses = { getTextureByLabel(tlib, "empty") };
	vector<texturePtr> tx_mouths = { getTextureByLabel(tlib, "empty") };
	vector<texturePtr> tx_brows = { getTextureByLabel(tlib, "empty") };
	vector<texturePtr> tx_hair = { getTextureByLabel(tlib, "empty") };
	vector<texturePtr> tx_bangs = { getTextureByLabel(tlib, "empty") };
	
	for (int i = 0; i < 14; i++)
		tx_eyes.push_back(getTextureByLabel(tlib, "eyes_" + to_string(i)));
	for (int i = 0; i < 10; i++)
		tx_noses.push_back(getTextureByLabel(tlib, "nose_" + to_string(i)));
	for (int i = 0; i < 12; i++)
		tx_mouths.push_back(getTextureByLabel(tlib, "mouth_" + to_string(i)));
	for (int i = 0; i < 8; i++)
		tx_brows.push_back(getTextureByLabel(tlib, "brow_" + to_string(i)));
	for (int i = 0; i < 9; i++)
		tx_hair.push_back(getTextureByLabel(tlib, "hair_" + to_string(i)));
	for (int i = 0; i < 6; i++)
		tx_bangs.push_back(getTextureByLabel(tlib, "bangs_" + to_string(i)));

	int idx_eyes = 0;
	int idx_noses = 0;
	int idx_mouths = 0;
	int idx_brows = 0;
	int idx_hair = 0;
	int idx_bangs = 0;


	//	bg
	auto btn_tlib = getCharCreationLibrary();
	auto bg = sf::Sprite(*getTextureByLabel(btn_tlib, "screen"));
	auto bg_img = bg.getTexture()->copyToImage();


	//	default hair colours
	gdata->pimage->tintLayer(head_layer::LAYER_BANGS, sf::Color(0, 0, 0));
	gdata->pimage->tintLayer(head_layer::LAYER_HAIR, sf::Color(0, 0, 0));


	//	this is just a fun little feature for Robin
	sf::Sprite qmark;
	qmark.setTexture(*getTextureByLabel(btn_tlib, "question"));
	qmark.setPosition(755, 530);


	//	list of buttons appearing
	vector<buttonPtr> buttons;
	
	createButton(&buttons, "btn_reset",			40,  65,  140, 110, getTextureByLabel(btn_tlib, "btn_reset"));
	createButton(&buttons, "btn_randomize",		33,  180, 150, 150, getTextureByLabel(btn_tlib, "btn_randomize"));
	createButton(&buttons, "btn_begin",			595, 520, 205, 70,  getTextureByLabel(btn_tlib, "btn_accept"));
	
	createButton(&buttons, "btn_sight_l",		640, 45,  50, 50, getTextureByLabel(btn_tlib, "btn_arrow"));
	createButton(&buttons, "btn_sight_r",		500, 45,  50, 50, getTextureByLabel(btn_tlib, "btn_arrow"));
	createButton(&buttons, "btn_smell_l",		505, 95,  50, 50, getTextureByLabel(btn_tlib, "btn_arrow"));
	createButton(&buttons, "btn_smell_r",		640, 95,  50, 50, getTextureByLabel(btn_tlib, "btn_arrow"));
	createButton(&buttons, "btn_taste_l",		505, 160, 50, 50, getTextureByLabel(btn_tlib, "btn_arrow"));
	createButton(&buttons, "btn_taste_r",		640, 160, 50, 50, getTextureByLabel(btn_tlib, "btn_arrow"));
	createButton(&buttons, "btn_expression_l",	485, 210, 50, 50, getTextureByLabel(btn_tlib, "btn_arrow"));
	createButton(&buttons, "btn_expression_r",	670, 210, 50, 50, getTextureByLabel(btn_tlib, "btn_arrow"));
	createButton(&buttons, "btn_style_l",		505, 270, 50, 50, getTextureByLabel(btn_tlib, "btn_arrow"));
	createButton(&buttons, "btn_style_r",		645, 270, 50, 50, getTextureByLabel(btn_tlib, "btn_arrow"));
	createButton(&buttons, "btn_fringe_l",		510, 325, 50, 50, getTextureByLabel(btn_tlib, "btn_arrow"));
	createButton(&buttons, "btn_fringe_r",		660, 325, 50, 50, getTextureByLabel(btn_tlib, "btn_arrow"));


	//	the character creation loop
	while (!done)
	{
		//	await input
		while (gdata->rwindow->pollEvent(event))
		{
			if (event.type == sf::Event::EventType::Closed)
				done = true;


			if (event.type == sf::Event::EventType::KeyPressed)
			{
				//	flatten all layers into a single texture and conclude
				if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Escape)
				{
					finishCharacter(gdata);
					done = true;
				}

				//	backspace
				else if (event.key.code == sf::Keyboard::BackSpace)
				{
					if (!gdata->playerName.isEmpty() && startedName)
					{
						gdata->playerName = gdata->playerName.substring(0, gdata->playerName.getSize() - 1);
						name_text_box.setString(gdata->playerName);
					}
				}
			}


			else if (event.type == sf::Event::EventType::TextEntered)
			{
				if ((event.text.unicode >= 'A' && event.text.unicode <= 'z') || event.text.unicode == ' ')
				{
					//	if we haven't started typing a name yet, clear the existing name
					if (!startedName)
					{
						startedName = true;
						gdata->playerName = "";
						name_text_box.setColor(sf::Color(0, 0, 0));
					}

					//	limit length of name
					if (gdata->playerName.getSize() < 12)
					{
						gdata->playerName += event.text.unicode;
						name_text_box.setString(gdata->playerName);
					}
				}
			}


			else if (event.type == sf::Event::EventType::MouseButtonReleased)
			{
				//	figure out what part of the button pane we clicked on, if any
				auto mpos = sf::Mouse::getPosition(*gdata->rwindow);
				auto btn = getButtonUnderPoint(mpos.x, mpos.y, &buttons);

				if (btn != nullptr)
				{
					//	RESET THE CREATURE
					if (btn->_id == "btn_reset")
					{
						idx_eyes = 0;
						idx_noses = 0;
						idx_mouths = 0;
						idx_brows = 0;
						idx_hair = 0;
						idx_bangs = 0;

						gdata->pimage->setLayerTexture(head_layer::LAYER_EYES, tx_eyes[idx_eyes]);
						gdata->pimage->setLayerTexture(head_layer::LAYER_NOSE, tx_noses[idx_noses]);
						gdata->pimage->setLayerTexture(head_layer::LAYER_MOUTH, tx_mouths[idx_mouths]);
						gdata->pimage->setLayerTexture(head_layer::LAYER_EYEBROWS, tx_brows[idx_brows]);
						gdata->pimage->setLayerTexture(head_layer::LAYER_HAIR, tx_hair[idx_hair]);
						gdata->pimage->setLayerTexture(head_layer::LAYER_BANGS, tx_bangs[idx_bangs]);

						gdata->pimage->tintLayer(head_layer::LAYER_HAIR, sf::Color(0, 0, 0));
						gdata->pimage->tintLayer(head_layer::LAYER_BANGS, sf::Color(0, 0, 0));
					}


					//	RANDOMIZE THE CREATURE
					else if (btn->_id == "btn_randomize")
					{
						idx_eyes = randrange(tx_eyes.size());
						idx_noses = randrange(tx_noses.size());
						idx_mouths = randrange(tx_mouths.size());
						idx_brows = randrange(tx_brows.size());
						idx_hair = randrange(tx_hair.size());
						idx_bangs = randrange(tx_bangs.size());
						gdata->pimage->setLayerTexture(head_layer::LAYER_EYES, tx_eyes[idx_eyes]);
						gdata->pimage->setLayerTexture(head_layer::LAYER_NOSE, tx_noses[idx_noses]);
						gdata->pimage->setLayerTexture(head_layer::LAYER_MOUTH, tx_mouths[idx_mouths]);
						gdata->pimage->setLayerTexture(head_layer::LAYER_EYEBROWS, tx_brows[idx_brows]);
						gdata->pimage->setLayerTexture(head_layer::LAYER_HAIR, tx_hair[idx_hair]);
						gdata->pimage->setLayerTexture(head_layer::LAYER_BANGS, tx_bangs[idx_bangs]);

						sf::Color col(randrange(256), randrange(256), randrange(256));
						gdata->pimage->tintLayer(head_layer::LAYER_HAIR, col);
						gdata->pimage->tintLayer(head_layer::LAYER_BANGS, col);
					}

					//	EYES BUTTONS
					else if (btn->_id == "btn_sight_l")
						loopCharacterFeature(gdata, head_layer::LAYER_EYES, &tx_eyes, &idx_eyes, -1);
					else if (btn->_id == "btn_sight_r")
						loopCharacterFeature(gdata, head_layer::LAYER_EYES, &tx_eyes, &idx_eyes, 1);

					//	NOSE BUTTONS
					else if (btn->_id == "btn_smell_l")
						loopCharacterFeature(gdata, head_layer::LAYER_NOSE, &tx_noses, &idx_noses, -1);
					else if (btn->_id == "btn_smell_r")
						loopCharacterFeature(gdata, head_layer::LAYER_NOSE, &tx_noses, &idx_noses, 1);

					//	MOUTH BUTTONS
					else if (btn->_id == "btn_taste_l")
						loopCharacterFeature(gdata, head_layer::LAYER_MOUTH, &tx_mouths, &idx_mouths, -1);
					else if (btn->_id == "btn_taste_r")
						loopCharacterFeature(gdata, head_layer::LAYER_MOUTH, &tx_mouths, &idx_mouths, 1);

					//	EYEBROW BUTTONS
					else if (btn->_id == "btn_expression_l")
						loopCharacterFeature(gdata, head_layer::LAYER_EYEBROWS, &tx_brows, &idx_brows, -1);
					else if (btn->_id == "btn_expression_r")
						loopCharacterFeature(gdata, head_layer::LAYER_EYEBROWS, &tx_brows, &idx_brows, 1);


					//	HAIR BUTTONS
					else if (btn->_id == "btn_style_l")
						loopCharacterFeature(gdata, head_layer::LAYER_HAIR, &tx_hair, &idx_hair, -1);
					else if (btn->_id == "btn_style_r")
						loopCharacterFeature(gdata, head_layer::LAYER_HAIR, &tx_hair, &idx_hair, 1);

					//	BANGS BUTTONS
					else if (btn->_id == "btn_fringe_l")
						loopCharacterFeature(gdata, head_layer::LAYER_BANGS, &tx_bangs, &idx_bangs, -1);
					else if (btn->_id == "btn_fringe_r")
						loopCharacterFeature(gdata, head_layer::LAYER_BANGS, &tx_bangs, &idx_bangs, 1);


					//	Finish creature
					else if (btn->_id == "btn_begin")
					{
						finishCharacter(gdata);
						done = true;
					}
				}


				//	clicking in the colour pane sets the colour of hair+bangs
				else if (inRectangle(mpos.x, mpos.y, 0, 400, 665, 535))
				{
					auto px = bg_img.getPixel(mpos.x, mpos.y);
					gdata->pimage->tintLayer(head_layer::LAYER_HAIR, px);
					gdata->pimage->tintLayer(head_layer::LAYER_BANGS, px);
				}
			}
		}

		//	render the window
		gdata->rwindow->clear();
		auto mpos = sf::Mouse::getPosition(*gdata->rwindow);
		
		gdata->rwindow->draw(bg);
		renderActor(gdata->rwindow, gdata->pimage);
		gdata->rwindow->draw(name_text_box);
		drawButtonList(gdata->rwindow, &buttons, mpos);

		//	only draw the question mark if we haven't highlighted the 'begin' button
		auto mbtn = getButtonUnderPoint(mpos.x, mpos.y, &buttons);
		if (mbtn == nullptr || mbtn->_id != "btn_begin")
			gdata->rwindow->draw(qmark);
		
		gdata->rwindow->display();
	}
}
