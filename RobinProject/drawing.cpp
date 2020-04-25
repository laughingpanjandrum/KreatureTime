#include "pch.h"
#include "drawing.h"



sf::Vector2i getRelativePositionInSprite(sf::IntRect bounds, sf::Vector2i pos)
{
	int x = (float)(pos.x - bounds.left) / (float)bounds.width;
	int y = (float)(pos.y - bounds.top) / (float)bounds.height;
	return sf::Vector2i(x, y);
}


//	Generates a new button and adds it to the list.
void createButton(vector<buttonPtr>* blist, const string id, const int x, const int y, const int w, const int h, texturePtr highlightTx)
{
	auto btn = buttonPtr(new button(id, x, y, w, h, highlightTx));
	blist->push_back(btn);
}

//	Returns True if (x,y) is in the rectangle defined by (x1,y1) and (x2,y2).
bool inRectangle(const int x, const int y, const int x1, const int y1, const int x2, const int y2)
{
	return x >= x1 && x <= x2 && y >= y1 && y <= y2;
}


//	Returns True if the given point is engulfed by the given button.
bool isPointOverButton(const int x, const int y, buttonPtr btn)
{
	return btn->_btnRect.contains(x, y);
}


//	Returns the first button in the list whose rectangle engulfs the given coordinate.
buttonPtr getButtonUnderPoint(const int x, const int y, vector<buttonPtr>* blist)
{
	for (auto btn : *blist)
	{
		if (isPointOverButton(x, y, btn))
			return btn;
	}
	return nullptr;
}


//	If the mouse is over any of the buttons in the list, draw that button's highlight sprite.
//	Otherwise, do not draw it.
void drawButtonList(sf::RenderWindow* rwindow, vector<buttonPtr>* blist, sf::Vector2i mpos)
{
	for (auto btn : *blist)
	{
		if (isPointOverButton(mpos.x, mpos.y, btn))
			rwindow->draw(*btn->_highlight);
	}
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


//	id: identifier for the buttons
//	x,y,w,h: dimensions of the rectangle forming the button
//	highlightTx: image displayed when we mouse over the button
button::button(string id, const int x, const int y, const int w, const int h, texturePtr highlightTx)
{
	_id = id;
	_btnRect = sf::IntRect(x, y, w, h);
	_highlight = spritePtr(new sf::Sprite());
	_highlight->setTexture(*highlightTx);
	_highlight->setPosition(x, y);
}
