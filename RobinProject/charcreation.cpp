#include "pch.h"
#include "charcreation.h"


int randrange(const int v, mt19937 * rng)
{
	uniform_int_distribution<int> rng_range(0, v - 1);
	return rng_range(*rng);
}

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
	name_text_box.setColor(sf::Color(0, 0, 0));
	name_text_box.setPosition(65, 550);


	//	Randomizer
	uniform_int_distribution<int> randomColorRange(0, 255);
	random_device rd;
	mt19937 randomNumbers(rd());



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
	auto btn_tlib = getButtonLibrary();
	auto bg = sf::Sprite(*getTextureByLabel(btn_tlib, "screen"));
	auto bg_img = bg.getTexture()->copyToImage();


	//	default hair colours
	gdata->pimage->tintLayer(head_layer::LAYER_BANGS, sf::Color(0, 0, 0));
	gdata->pimage->tintLayer(head_layer::LAYER_HAIR, sf::Color(0, 0, 0));


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
					if (!gdata->playerName.isEmpty())
					{
						gdata->playerName = gdata->playerName.substring(0, gdata->playerName.getSize() - 1);
						name_text_box.setString(gdata->playerName);
					}
				}
			}


			else if (event.type == sf::Event::EventType::TextEntered)
			{
				if (event.text.unicode >= 'A' && event.text.unicode <= 'z')
				{
					gdata->playerName += event.text.unicode;
					name_text_box.setString(gdata->playerName);
				}
			}


			else if (event.type == sf::Event::EventType::MouseButtonReleased)
			{
				//	figure out what part of the button pane we clicked on, if any
				auto mpos = sf::Mouse::getPosition(*gdata->rwindow);
				int x = mpos.x;
				int y = mpos.y;

				//	RESET THE CREATURE
				if (inRectangle(x, y, 30, 165, 60, 165))
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
				else if (inRectangle(x, y, 20, 195, 170, 350))
				{
					idx_eyes = randrange(tx_eyes.size(), &randomNumbers);
					idx_noses = randrange(tx_noses.size(), &randomNumbers);
					idx_mouths = randrange(tx_mouths.size(), &randomNumbers);
					idx_brows = randrange(tx_brows.size(), &randomNumbers);
					idx_hair = randrange(tx_hair.size(), &randomNumbers);
					idx_bangs = randrange(tx_bangs.size(), &randomNumbers);
					gdata->pimage->setLayerTexture(head_layer::LAYER_EYES, tx_eyes[idx_eyes]);
					gdata->pimage->setLayerTexture(head_layer::LAYER_NOSE, tx_noses[idx_noses]);
					gdata->pimage->setLayerTexture(head_layer::LAYER_MOUTH, tx_mouths[idx_mouths]);
					gdata->pimage->setLayerTexture(head_layer::LAYER_EYEBROWS, tx_brows[idx_brows]);
					gdata->pimage->setLayerTexture(head_layer::LAYER_HAIR, tx_hair[idx_hair]);
					gdata->pimage->setLayerTexture(head_layer::LAYER_BANGS, tx_bangs[idx_bangs]);

					sf::Color col(randrange(256, &randomNumbers), randrange(256, &randomNumbers), randrange(256, &randomNumbers));
					gdata->pimage->tintLayer(head_layer::LAYER_HAIR, col);
					gdata->pimage->tintLayer(head_layer::LAYER_BANGS, col);
				}

				//	EYES BUTTONS
				else if (y >= 55 && y <= 100)
				{
					if (x >= 510 && x <= 550)
						loopCharacterFeature(gdata, head_layer::LAYER_EYES, &tx_eyes, &idx_eyes, -1);
					else if (x >= 650 && x <= 700)
						loopCharacterFeature(gdata, head_layer::LAYER_EYES, &tx_eyes, &idx_eyes, 1);
				}

				//	NOSE BUTTONS
				else if (y >= 105 && y <= 145)
				{
					if (x >= 510 && x <= 550)
						loopCharacterFeature(gdata, head_layer::LAYER_NOSE, &tx_noses, &idx_noses, -1);
					else if (x >= 650 && x <= 700)
						loopCharacterFeature(gdata, head_layer::LAYER_NOSE, &tx_noses, &idx_noses, 1);
				}

				//	MOUTH BUTTONS
				else if (y >= 170 && y <= 210)
				{
					if (x >= 510 && x <= 550)
						loopCharacterFeature(gdata, head_layer::LAYER_MOUTH, &tx_mouths, &idx_mouths, -1);
					else if (x >= 650 && x <= 700)
						loopCharacterFeature(gdata, head_layer::LAYER_MOUTH, &tx_mouths, &idx_mouths, 1);
				}

				//	EYEBROW BUTTONS
				else if (y >= 220 && y <= 260)
				{
					if (x >= 510 && x <= 550)
						loopCharacterFeature(gdata, head_layer::LAYER_EYEBROWS, &tx_brows, &idx_brows, -1);
					else if (x >= 650 && x <= 700)
						loopCharacterFeature(gdata, head_layer::LAYER_EYEBROWS, &tx_brows, &idx_brows, 1);
				}

				//	HAIR BUTTONS
				else if (y >= 280 && y <= 320)
				{
					if (x >= 520 && x <= 550)
						loopCharacterFeature(gdata, head_layer::LAYER_HAIR, &tx_hair, &idx_hair, -1);
					else if (x >= 660 && x <= 695)
						loopCharacterFeature(gdata, head_layer::LAYER_HAIR, &tx_hair, &idx_hair, 1);
				}

				//	BANGS BUTTONS
				else if (y >= 335 && y <= 375)
				{
					if (x >= 520 && x <= 560)
						loopCharacterFeature(gdata, head_layer::LAYER_BANGS, &tx_bangs, &idx_bangs, -1);
					else if (x >= 670 && x <= 705)
						loopCharacterFeature(gdata, head_layer::LAYER_BANGS, &tx_bangs, &idx_bangs, 1);
				}


				//	clicking in the colour pane sets the colour of hair+bangs
				else if (x >= 0 && x <= 665 && y >= 400 && y <= 535)
				{
					auto px = bg_img.getPixel(x, y);
					gdata->pimage->tintLayer(head_layer::LAYER_HAIR, px);
					gdata->pimage->tintLayer(head_layer::LAYER_BANGS, px);
				}
			}
		}

		//	render the window
		gdata->rwindow->clear();
		gdata->rwindow->draw(bg);
		renderActor(gdata->rwindow, gdata->pimage);
		gdata->rwindow->draw(name_text_box);
		gdata->rwindow->display();
	}
}
