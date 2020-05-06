#include "pch.h"
#include "invmanagement.h"



//	Updates the player's base texture to contain all our worn equipment.
void renderPlayerEquipment(gamedataPtr gdata)
{
	//	get the base character texture
	auto rtx = gdata->pimage->flatten();

	//	print each equipment layer on top of it
	for (auto tx : gdata->playerData->getWornEquipmentTextures())
	{
		auto spr = sf::Sprite();
		spr.setTexture(*tx);
		spr.setPosition(0, 0);
		rtx->draw(spr);
	}
	rtx->display();

	//	now update the player texture
	gdata->playerTexture = texturePtr(new sf::Texture(rtx->getTexture()));
	gdata->playerSprite->setTexture(*gdata->playerTexture);
}



//	Returns a list of items we're carrying that fit in this slot.
vector<equipmentPtr> getItemsForSlot(gamedataPtr gdata, const unsigned slot)
{
	vector<equipmentPtr> elist;
	for (auto it : gdata->eman.equipment)
	{
		if (it->slot == slot)
			elist.push_back(it);
	}
	return elist;
}


//	Generates a sprite for each equipment item & positions each one in a tiled pattern on the screen
vector<spritePtr> createSpritesForEquipmentItems(vector<equipmentPtr>* items)
{
	vector<spritePtr> spritelist;
	int atx = 250, aty = 50;
	for (auto it : *items)
	{
		auto spr = spritePtr(new sf::Sprite());
		spr->setTexture(*it->tx);
		spr->scale(sf::Vector2f(0.5, 0.5));
		spr->setPosition(atx, aty);
		atx += 100;
		if (atx >= 800)
		{
			atx = 250;
			aty += 150;
		}
		spritelist.push_back(spr);
	}
	return spritelist;
}



//	Creates buttons for each active inventory item
void updateItemListCategory(gamedataPtr gdata, vector<buttonPtr>* buttons, vector<spritePtr>* sprites, vector<equipmentPtr>* items, texturePtr check_tx)
{
	for (unsigned i = 0; i < sprites->size(); i++)
	{
		auto pos = sprites->at(i)->getPosition();
		createButton(buttons, items->at(i)->id, pos.x, pos.y, 100, 150, check_tx);
	}
}



//	Creates the buttons for selecting the inventory category
void updateInventoryButtons(gamedataPtr gdata, vector<buttonPtr>* buttons, vector<sf::Text*>* txtbuttons, const unsigned currentSlot, texturePtr check_tx)
{
	for (unsigned slot = 0; slot < SLOT__MAX; slot++)
	{
		//	text label (coloured if it's the current slot)
		auto col = sf::Color(0, 0, 0);
		if (slot == currentSlot)
			col = sf::Color(0, 150, 0);
		txtbuttons->push_back(createTextElement(&gdata->usefont, getItemSlotName(slot), 50 + slot * 75, 10, col, 15));
		createButton(buttons, "slot_" + to_string(slot), 40 + slot * 75, 10, 85, 20, check_tx);
	}
}




//	The main inventory management control loop
void inventoryManagementLoop(gamedataPtr gdata)
{
	//	Position player sprite
	gdata->playerSprite->setPosition(10, 150);
	auto check_tx = getTextureFromFile("dialogue/dot.png");


	//	Selected item category
	auto currentSlot = SLOT_BODY;
	auto elist = getItemsForSlot(gdata, currentSlot);
	auto item_sprites = createSpritesForEquipmentItems(&elist);


	//	Buttons to select categories
	vector<buttonPtr> buttons;
	vector<sf::Text*> txtbuttons;
	updateInventoryButtons(gdata, &buttons, &txtbuttons, currentSlot, check_tx);
	updateItemListCategory(gdata, &buttons, &item_sprites, &elist, check_tx);


	//	The principal loop.
	bool done = false;
	sf::Event event;
	while (!done)
	{
		//	get input
		while (gdata->rwindow->pollEvent(event))
		{
			//	closed window
			if (event.type == sf::Event::EventType::Closed)
				done = true;

			//	Quit w/ ESCAPE
			if (event.type == sf::Event::EventType::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
					done = true;
			}

			//	Mouse clicks
			if (event.type == sf::Event::EventType::MouseButtonReleased)
			{
				auto mpos = sf::Mouse::getPosition(*gdata->rwindow);
				auto btn = getButtonUnderPoint(mpos.x, mpos.y, &buttons);
				if (btn != nullptr)
				{
					//	Update current slot?
					if (btn->_id.substr(0, 4) == "slot")
					{
						int n = btn->_id.back() - 48;
						if (n >= 0 && n < SLOT__MAX)
						{
							currentSlot = n;
							elist = getItemsForSlot(gdata, currentSlot);
							item_sprites = createSpritesForEquipmentItems(&elist);
							buttons.clear();
							txtbuttons.clear();
							updateInventoryButtons(gdata, &buttons, &txtbuttons, currentSlot, check_tx);
							updateItemListCategory(gdata, &buttons, &item_sprites, &elist, check_tx);
						}
					}

					//	Selected an item?
					else
					{
						auto it = getEquipmentItemById(&gdata->eman, btn->_id);
						if (it != nullptr)
						{
							if (gdata->playerData->isEquipped(it))
								gdata->playerData->clearItemSlot(it->slot);
							else
								gdata->playerData->equipItem(it);
							renderPlayerEquipment(gdata);
						}
					}
				}
			}
		}

		//	Render display
		gdata->rwindow->clear(sf::Color(255, 255, 255));

		//	Category selectors
		for (auto txobj : txtbuttons)
			gdata->rwindow->draw(*txobj);
		drawButtonList(gdata->rwindow, &buttons, sf::Mouse::getPosition(*gdata->rwindow));

		//	Player sprite
		gdata->rwindow->draw(*gdata->playerSprite);

		//	Items in this category
		for (auto spr : item_sprites)
			gdata->rwindow->draw(*spr);

		gdata->rwindow->display();
	}
}
