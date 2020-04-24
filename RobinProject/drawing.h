#pragma once

#include "actor.h"


sf::Vector2i getRelativePositionInSprite(sf::IntRect bounds, sf::Vector2i pos);

bool inRectangle(const int x, const int y, const int x1, const int y1, const int x2, const int y2);

void renderActor(sf::RenderWindow* rwindow, player_image* pimage);