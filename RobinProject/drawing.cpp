#include "pch.h"
#include "drawing.h"



sf::Vector2i getRelativePositionInSprite(sf::IntRect bounds, sf::Vector2i pos)
{
	int x = (float)(pos.x - bounds.left) / (float)bounds.width;
	int y = (float)(pos.y - bounds.top) / (float)bounds.height;
	return sf::Vector2i(x, y);
}


void renderActor(sf::RenderWindow * rwindow, player_image * pimage)
{
	//	the actor's BODY is rendered first, then each of their layers
	rwindow->draw(*pimage->_body);
	for (auto l : pimage->_layers)
	{
		if (l->getTexture() != nullptr)
			rwindow->draw(*l);
	}
}
