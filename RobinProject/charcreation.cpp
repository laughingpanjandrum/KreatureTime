#include "pch.h"
#include "charcreation.h"
#include "drawing.h"


void loopCharacterFeature(gamedataPtr gdata, const unsigned layer_index, vector<texturePtr>* txlist, int * idx, int vec)
{
	(*idx) += vec;
	if ((*idx) >= txlist->size())
		*idx = 0;
	else if ((*idx) < 0)
		*idx = txlist->size() - 1;
	gdata->pimage->setLayerTexture(layer_index, txlist->at(*idx));
}




void charCreationLoop(gamedataPtr gdata)
{
	bool done = false;
	sf::Event event;

	//	texture library for the player character
	auto tlib = getCreatureLibrary();
	gdata->pimage->setBodyTexture(getTextureByLabel(tlib, "base"));
	gdata->pimage->setPosition(100, 50);

	//	test: final rendered dude
	sf::Sprite finaldude;
	finaldude.setTexture(*getTextureByLabel(tlib, "base"));
	finaldude.setPosition(600, 300);

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


	//	a little title img
	auto tlib_btns = getButtonLibrary();
	auto title_sprite = sf::Sprite(*getTextureByLabel(tlib_btns, "kreature_title"));
	title_sprite.setPosition(100, 400);


	//	buttons for changing stuff
	auto btn_pane = sf::Sprite(*getTextureByLabel(tlib_btns, "btn_pane"));
	btn_pane.setPosition(500, -50);
	btn_pane.setOrigin(0, 0);


	//	for colour swapping
	auto col_pane = sf::Sprite(*getTextureByLabel(tlib_btns, "colors"));
	col_pane.setPosition(450, 390);
	col_pane.setOrigin(0, 0);


	//	for catching clicks in the button pane
	auto bpane_rect = btn_pane.getGlobalBounds();
	auto cpane_rect = col_pane.getGlobalBounds();


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
				if (event.key.code == sf::Keyboard::Escape)
					done = true;
				else if (event.key.code == sf::Keyboard::Return)
				{
					auto rtx = gdata->pimage->flatten();
					finaldude.setTexture(rtx->getTexture());
				}
			}


			else if (event.type == sf::Event::EventType::MouseButtonReleased)
			{
				//	figure out what part of the button pane we clicked on, if any
				auto mpos = sf::Mouse::getPosition(*gdata->rwindow);
				int xoffs = mpos.x - bpane_rect.left;
				int yoffs = mpos.y - bpane_rect.top;

				//	EYES BUTTONS
				if (yoffs >= 95 && yoffs <= 145)
				{
					if (xoffs >= 0 && xoffs <= 75)
						loopCharacterFeature(gdata, head_layer::LAYER_EYES, &tx_eyes, &idx_eyes, -1);
					else if (xoffs >= 140 && xoffs <= 180)
						loopCharacterFeature(gdata, head_layer::LAYER_EYES, &tx_eyes, &idx_eyes, 1);
				}

				//	NOSE BUTTONS
				else if (yoffs >= 150 && yoffs <= 195)
				{
					if (xoffs >= 0 && xoffs <= 75)
						loopCharacterFeature(gdata, head_layer::LAYER_NOSE, &tx_noses, &idx_noses, -1);
					else if (xoffs >= 140 && xoffs <= 180)
						loopCharacterFeature(gdata, head_layer::LAYER_NOSE, &tx_noses, &idx_noses, 1);
				}

				//	MOUTH BUTTONS
				else if (yoffs >= 220 && yoffs <= 260)
				{
					if (xoffs >= 0 && xoffs <= 75)
						loopCharacterFeature(gdata, head_layer::LAYER_MOUTH, &tx_mouths, &idx_mouths, -1);
					else if (xoffs >= 140 && xoffs <= 180)
						loopCharacterFeature(gdata, head_layer::LAYER_MOUTH, &tx_mouths, &idx_mouths, 1);
				}

				//	EYEBROW BUTTONS
				else if (yoffs >= 265 && yoffs <= 305)
				{
					if (xoffs >= 0 && xoffs <= 20)
						loopCharacterFeature(gdata, head_layer::LAYER_EYEBROWS, &tx_brows, &idx_brows, -1);
					else if (xoffs >= 175 && xoffs <= 200)
						loopCharacterFeature(gdata, head_layer::LAYER_EYEBROWS, &tx_brows, &idx_brows, 1);
				}

				//	HAIR BUTTONS
				else if (yoffs >= 330 && yoffs <= 370)
				{
					if (xoffs >= 0 && xoffs <= 40)
						loopCharacterFeature(gdata, head_layer::LAYER_HAIR, &tx_hair, &idx_hair, -1);
					else if (xoffs >= 145 && xoffs <= 180)
						loopCharacterFeature(gdata, head_layer::LAYER_HAIR, &tx_hair, &idx_hair, 1);
				}

				//	BANGS BUTTONS
				else if (yoffs >= 380 && yoffs <= 420)
				{
					if (xoffs >= 0 && xoffs <= 40)
						loopCharacterFeature(gdata, head_layer::LAYER_BANGS, &tx_bangs, &idx_bangs, -1);
					else if (xoffs >= 155 && xoffs <= 195)
						loopCharacterFeature(gdata, head_layer::LAYER_BANGS, &tx_bangs, &idx_bangs, 1);
				}


				//	clicking in the colour pane sets the colour of hair+bangs
				xoffs = mpos.x - cpane_rect.left;
				yoffs = mpos.y - cpane_rect.top;
				if (xoffs >= 0 && xoffs <= 300 && yoffs >= 0 && yoffs <= 200)
				{
					auto img = col_pane.getTexture()->copyToImage();
					auto px = img.getPixel(xoffs, yoffs);
					gdata->pimage->tintLayer(head_layer::LAYER_HAIR, px);
					gdata->pimage->tintLayer(head_layer::LAYER_BANGS, px);
				}
			}
		}

		//	render the window
		gdata->rwindow->clear(sf::Color(255, 255, 255));
		renderActor(gdata->rwindow, gdata->pimage);
		gdata->rwindow->draw(btn_pane);
		gdata->rwindow->draw(col_pane);
		gdata->rwindow->draw(title_sprite);
		gdata->rwindow->display();
	}
}
