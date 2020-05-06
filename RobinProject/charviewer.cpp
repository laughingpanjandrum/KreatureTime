#include "pch.h"
#include "charviewer.h"



//	Put points into a stat (or take them out)
void tryAdjustStat(gamedataPtr gdata, const unsigned attr, int adj, int * statPoints)
{
	if (adj < 0 && gdata->playerData->getAttributeValue(attr) > 1)
	{
		gdata->playerData->adjustAttribute(attr, -1);
		(*statPoints)++;
	}
	else if (adj > 0 && *statPoints > 0)
	{
		gdata->playerData->adjustAttribute(attr, 1);
		(*statPoints)--;
	}
}


void charViewerLoop(gamedataPtr gdata)
{
	//	setup the screen
	bool done = false;
	gdata->playerSprite->setPosition(310, 160);
	sf::Event event;


	//	textures
	auto tlib = getStatSelectionLibrary();
	

	//	background
	sf::Sprite bg;
	bg.setTexture(*getTextureByLabel(tlib, "bg"));
	bg.setPosition(0, 0);


	//	title text
	auto title = createTextElement(&gdata->usefont, gdata->playerName, 570, 30);


	//	bottom text lol
	sf::Text btm;
	btm.setFont(gdata->usefont);
	btm.setColor(sf::Color(0, 0, 0));
	switch (randint(1, 6))
	{
	case(1): btm.setString("Does your creation please you?"); break;
	case(2): btm.setString("Do you feel satisfied?"); break;
	case(3): btm.setString("What do you think of them?"); break;
	case(4): btm.setString("Would you make another?"); break;
	case(5): btm.setString("Do you feel any regret?"); break;
	case(6): btm.setString("Will you treasure them?"); break;
	}
	btm.setPosition(20, 560);


	//	stat values
	vector<sf::Text*> stat_txt;
	stat_txt.push_back(createTextElement(&gdata->usefont, "1", 150, 160));
	stat_txt.push_back(createTextElement(&gdata->usefont, "1", 155, 275));
	stat_txt.push_back(createTextElement(&gdata->usefont, "1", 160, 465));
	stat_txt.push_back(createTextElement(&gdata->usefont, "1", 640, 170));
	stat_txt.push_back(createTextElement(&gdata->usefont, "1", 640, 300));
	stat_txt.push_back(createTextElement(&gdata->usefont, "1", 640, 460));


	//	stat buttons
	vector<buttonPtr> buttons;
	createButton(&buttons, "appeal_d",		80,  140, 50, 50,	getTextureByLabel(tlib, "btn_circle1"));
	createButton(&buttons, "appeal_u",		185, 140, 50, 50,	getTextureByLabel(tlib, "btn_circle1"));
	createButton(&buttons, "cunning_d",		80,  265, 50, 50,	getTextureByLabel(tlib, "btn_circle1"));
	createButton(&buttons, "cunning_u",		185, 265, 50, 50,	getTextureByLabel(tlib, "btn_circle1"));
	createButton(&buttons, "derring_d",		80,  450, 50, 50,	getTextureByLabel(tlib, "btn_circle1"));
	createButton(&buttons, "derring_u",		185, 450, 50, 50,	getTextureByLabel(tlib, "btn_circle1"));
	createButton(&buttons, "mystic_d",		560, 150, 50, 50,	getTextureByLabel(tlib, "btn_circle1"));
	createButton(&buttons, "mystic_u",		680, 150, 50, 50,	getTextureByLabel(tlib, "btn_circle1"));
	createButton(&buttons, "pomp_d",		560, 290, 50, 50,	getTextureByLabel(tlib, "btn_circle1"));
	createButton(&buttons, "pomp_u",		680, 290, 50, 50,	getTextureByLabel(tlib, "btn_circle1"));
	createButton(&buttons, "violence_d",	570, 440, 50, 50,	getTextureByLabel(tlib, "btn_circle1"));
	createButton(&buttons, "violence_u",	680, 440, 50, 50,	getTextureByLabel(tlib, "btn_circle1"));
	createButton(&buttons, "accept",		690, 490, 110, 105, getTextureByLabel(tlib, "btn_circle2"));


	//	stat points left
	int statPoints = 10;
	auto stat_pts_txt = createTextElement(&gdata->usefont, to_string(statPoints), 440, 475, sf::Color(0, 100, 0));


	//	viewing loop
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
			}

			//	Button handling
			else if (event.type == sf::Event::EventType::MouseButtonReleased)
			{
				//	figure out what part of the button pane we clicked on, if any
				auto mpos = sf::Mouse::getPosition(*gdata->rwindow);
				auto btn = getButtonUnderPoint(mpos.x, mpos.y, &buttons);

				if (btn != nullptr)
				{
					//	finish creating
					if (btn->_id == "accept")
						done = true;

					//	stat adjustments
					else if (btn->_id == "appeal_d")
						tryAdjustStat(gdata, ATTR_APPEAL, -1, &statPoints);
					else if (btn->_id == "appeal_u")
						tryAdjustStat(gdata, ATTR_APPEAL, 1, &statPoints);
					
					else if (btn->_id == "cunning_d")
						tryAdjustStat(gdata, ATTR_CUNNING, -1, &statPoints);
					else if (btn->_id == "cunning_u")
						tryAdjustStat(gdata, ATTR_CUNNING, 1, &statPoints);
					
					else if (btn->_id == "derring_d")
						tryAdjustStat(gdata, ATTR_DERRING_DO, -1, &statPoints);
					else if (btn->_id == "derring_u")
						tryAdjustStat(gdata, ATTR_DERRING_DO, 1, &statPoints);
					
					else if (btn->_id == "mystic_d")
						tryAdjustStat(gdata, ATTR_MYSTICISM, -1, &statPoints);
					else if (btn->_id == "mystic_u")
						tryAdjustStat(gdata, ATTR_MYSTICISM, 1, &statPoints);
					
					else if (btn->_id == "pomp_d")
						tryAdjustStat(gdata, ATTR_POMP, -1, &statPoints);
					else if (btn->_id == "pomp_u")
						tryAdjustStat(gdata, ATTR_POMP, 1, &statPoints);
					
					else if (btn->_id == "violence_d")
						tryAdjustStat(gdata, ATTR_VIOLENCE, -1, &statPoints);
					else if (btn->_id == "violence_u")
						tryAdjustStat(gdata, ATTR_VIOLENCE, 1, &statPoints);

					//	once we've clicked, update all the text fields (just in case)
					for (unsigned i = 0; i < stat_txt.size(); i++)
						stat_txt[i]->setString(to_string(gdata->playerData->getAttributeValue(i)));
					stat_pts_txt->setString(to_string(statPoints));
				}
			}
		}

		//	render the display
		gdata->rwindow->clear();
		
		gdata->rwindow->draw(bg);

		gdata->rwindow->draw(*gdata->playerSprite);
		gdata->rwindow->draw(*title);
		gdata->rwindow->draw(*stat_pts_txt);
		gdata->rwindow->draw(btm);
		for (auto st : stat_txt)
			gdata->rwindow->draw(*st);
		drawButtonList(gdata->rwindow, &buttons, sf::Mouse::getPosition(*gdata->rwindow));

		gdata->rwindow->display();
	}
}
