#pragma once

#include "actor.h"


struct button
{
	button() {}
	button(string id, const int x, const int y, const int w, const int h, texturePtr highlightTx);

	string			_id;
	sf::IntRect		_btnRect;
	spritePtr		_highlight;
};
typedef shared_ptr<button> buttonPtr;



sf::Vector2i getRelativePositionInSprite(sf::IntRect bounds, sf::Vector2i pos);


//	Button management
void createButton(vector<buttonPtr>* blist, const string id, const int x, const int y, const int w, const int h, texturePtr highlightTx);
bool inRectangle(const int x, const int y, const int x1, const int y1, const int x2, const int y2);
bool isPointOverButton(const int x, const int y, buttonPtr btn);
buttonPtr getButtonUnderPoint(const int x, const int y, vector<buttonPtr>* blist);

void drawButtonList(sf::RenderWindow* rwindow, vector<buttonPtr>* blist, sf::Vector2i mpos);



//	Text
sf::Text* createTextElement(sf::Font* font, string txt, int x, int y, sf::Color color = sf::Color(0, 0, 0), int sz = 30);


//	Character drawing
void renderActor(sf::RenderWindow* rwindow, player_image* pimage);