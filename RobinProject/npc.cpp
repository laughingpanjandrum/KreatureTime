#include "pch.h"
#include "npc.h"



//	Magically transforms an ID to an NPC sprite.
spritePtr createNPCSprite(const string id, npcDataManager* nman)
{
	for (auto dat : nman->all)
	{
		if (dat->id == id)
		{
			auto spr = spritePtr(new sf::Sprite());
			auto tx = new sf::Texture();
			if (tx->loadFromFile("data/img/npc/" + dat->file, sf::IntRect(dat->x, dat->y, dat->w, dat->h)))
			{
				spr->setTexture(*tx);
				return spr;
			}
			else
			{
				cout << "ERROR loading texture from file " << dat->file << "!" << endl;
				return nullptr;
			}
		}
	}
	cout << "ERROR! Couldn't find any NPC with ID " << id << endl;
	return nullptr;
}
