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
	rtx->clear(sf::Color(0, 0, 0, 0));

	//	we need to temporarily move all our sprites so they're at zero, the origin of the new RTX
	auto oldpos = _body->getPosition();
	setPosition(0, 0);

	//	render onto the texture
	rtx->draw(*_body);
	for (auto layer : _layers)
		rtx->draw(*layer);
	rtx->display();

	//	return to our previous position
	setPosition(oldpos.x, oldpos.y);
	return rtx;
}



playerdata::playerdata()
{
	//	blank attribute values
	for (unsigned i = 0; i < ATTR__MAX; i++)
		_attributes.push_back(1);

	//	initialize item slots
	for (unsigned i = 0; i < SLOT__MAX; i++)
		_worn.push_back(nullptr);
}

void playerdata::setAttributeValue(const unsigned attr, int v)
{
	_attributes[attr] = v;
}

const int playerdata::getAttributeValue(const unsigned attr)
{
	return _attributes[attr];
}

void playerdata::adjustAttribute(const unsigned attr, int adj)
{
	_attributes[attr] += adj;
}


//	Returns a list of worn equipment textures in the order they should be equipped.
vector<texturePtr> playerdata::getWornEquipmentTextures() const
{
	vector<texturePtr> txlist;
	for (auto it : _worn)
	{
		if (it != nullptr)
			txlist.push_back(it->tx);
	}
	return txlist;
}

//	Sets the given item as equipped in the given slot.
void playerdata::equipItem(equipmentPtr it)
{
	_worn[it->slot] = it;
}
