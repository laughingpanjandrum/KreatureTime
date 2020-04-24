#include "pch.h"
#include "charviewer.h"



void charViewerLoop(gamedataPtr gdata)
{
	//	setup the screen
	bool done = false;
	gdata->playerSprite->setPosition(50, 50);
	sf::Event event;


	//	title text
	sf::Text title;
	title.setFont(gdata->usefont);
	title.setColor(sf::Color(0, 0, 0));
	title.setString("You have created " + gdata->playerName + ".");
	title.setPosition(10, 20);



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
		gdata->rwindow->display();
	}
}
