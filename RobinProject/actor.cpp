#include "pch.h"
#include "actor.h"



player_image::player_image()
{
	//	create untextured sprites for each layer of our head
	for (unsigned i = 0; i < head_layer::LAYER__MAX; i++)
		_layers.push_back(spritePtr(new sf::Sprite()));
	_body = spritePtr(new sf::Sprite());
}



void player_image::setLayerTexture(const unsigned layerId, const texturePtr tx)
{
	if (layerId >= 0 && layerId < head_layer::LAYER__MAX)
		_layers[layerId]->setTexture(*tx);
	else
		cout << "ERROR: Bad layer for actor " << layerId << endl;
}

void player_image::tintLayer(const unsigned layerId, sf::Color tcol)
{
	_layers[layerId]->setColor(tcol);
}

void player_image::setPosition(const int x, const int y)
{
	_body->setPosition(x, y);
	for (auto layer : _layers)
		layer->setPosition(x, y);
}


//	Render all our layers into a single RenderTexture and return it.
sf::RenderTexture* player_image::flatten()
{
	auto rtx = new sf::RenderTexture();
	rtx->create(200, 300);
	rtx->draw(*_body);
	for (auto layer : _layers)
		rtx->draw(*layer);
	return rtx;
}
