#include "pch.h"
#include "charviewer.h"



void charViewerLoop(gamedataPtr gdata)
{
	//	setup the screen
	bool done = false;
	gdata->playerSprite->setPosition(100, 100);
	sf::Event event;


	//	title text
	sf::Text title;
	title.setFont(gdata->usefont);
	title.setColor(sf::Color(0, 0, 0));
	title.setString("You have created " + gdata->playerName + ".");
	title.setPosition(20, 40);


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


	//	additional fields for our stats
	vector<sf::Text> stat_fields;
	for (unsigned i = 0; i < ATTR__MAX; i++)
	{
		sf::Text txf;
		txf.setFont(gdata->usefont);
		txf.setColor(sf::Color(0, 0, 0));
		txf.setString(getAttributeName(i) + ": " + to_string(gdata->playerData->getAttributeValue(i)));
		txf.setPosition(350, 120 + 50 * i);
		txf.setCharacterSize(15);
		stat_fields.push_back(txf);
	}



	//	viewing loop
	while (!done)
	{
		//	get input
		while (gdata->rwindow->pollEvent(event))
		{
			if (event.type == sf::Event::EventType::Closed)
				done = true;


			if (event.type == sf::Event::EventType::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
					done = true;
			}
		}

		//	render the display
		gdata->rwindow->clear(sf::Color(255, 255, 255));
		gdata->rwindow->draw(*gdata->playerSprite);
		gdata->rwindow->draw(title);
		gdata->rwindow->draw(btm);
		for (auto txf : stat_fields)
			gdata->rwindow->draw(txf);
		gdata->rwindow->display();
	}
}
