#pragma once

#include "actor.h"


sf::Vector2i getRelativePositionInSprite(sf::IntRect bounds, sf::Vector2i pos);


void renderActor(sf::RenderWindow* rwindow, player_image* pimage);