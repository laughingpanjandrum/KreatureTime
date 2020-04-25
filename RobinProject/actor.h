#pragma once

#include "attributes.h"
#include "imageloader.h"
#include "random.h"


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


//	For character creation: contains all the layers of the Kreature, and can render them down into a single RenderTexture for convenience
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




//	Player statistics
class playerdata
{
public:

	playerdata();
	~playerdata() {}

	void setAttributeValue(const unsigned attr, int v);
	const int getAttributeValue(const unsigned attr);

	void adjustAttribute(const unsigned attr, int adj);

private:

	vector<int> _attributes;

};
typedef shared_ptr<playerdata> playerdataPtr;