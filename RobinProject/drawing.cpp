#include "pch.h"
#include "drawing.h"



sf::Vector2i getRelativePositionInSprite(sf::IntRect bounds, sf::Vector2i pos)
{
	int x = (float)(pos.x - bounds.left) / (float)bounds.width;
	int y = (float)(pos.y - bounds.top) / (float)bounds.height;
	return sf::Vector2i(x, y);
}


//	Returns True if (x,y) is in the rectangle defined by (x1,y1) and (x2,y2).
bool inRectangle(const int x, const int y, const int x1, const int y1, const int x2, const int y2)
{
	return x >= x1 && x <= x2 && y >= y1 && y <= y2;
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
