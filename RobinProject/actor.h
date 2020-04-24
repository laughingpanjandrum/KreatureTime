#pragma once


#include "imageloader.h"



namespace head_layer
{
	const unsigned LAYER_EYES = 0;
	const unsigned LAYER_MOUTH = 1;
	const unsigned LAYER_NOSE = 2;
	const unsigned LAYER_EYEBROWS = 3;
	const unsigned LAYER_HAIR = 4;
	const unsigned LAYER_BANGS = 5;

	const unsigned LAYER__MAX = 6;
}



class player_image
{
public:

	player_image();
	~player_image() {}


	//		Setting textures

	void setBodyTexture(const texturePtr tx) { _body->setTexture(*tx); }
	void setLayerTexture(const unsigned layerId, const texturePtr tx);
	void tintLayer(const unsigned layerId, sf::Color tcol);


	//		Position (moves ALL the sprites that make up our body)

	void setPosition(const int x, const int y);


	//		Returns a new texture w/ all our textures rendered together.

	sf::RenderTexture* flatten();


	//		Public data

	spritePtr _body;
	vector<spritePtr> _layers;
};