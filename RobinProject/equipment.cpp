#include "pch.h"
#include "equipment.h"



string getItemSlotName(const unsigned slot)
{
	switch (slot)
	{
	case(SLOT_BODY):		return "Body";
	case(SLOT_FACE):		return "Face";
	case(SLOT_FEET):		return "Feet";
	case(SLOT_HANDS):		return "Hands";
	case(SLOT_HEAD):		return "Head";
	case(SLOT_HELD):		return "Held";
	case(SLOT_LEGS):		return "Legs";
	}
	return "{bad item slot " + to_string(slot) + "}";
}



//	Loads all textures for images into their respective item structs.
void loadAllItemTextures(inventoryManager * eman)
{
	//	ALL equipment textures should be in this file, please!
	sf::Image img;
	img.loadFromFile(EQUIPMENT_IMAGE_FILE);


	//	Generate them based on item structs
	for (auto it : eman->equipment)
	{
		auto tx = texturePtr(new sf::Texture());
		tx->loadFromImage(img, sf::IntRect(it->filex, it->filey, DEFAULT_TX_WIDTH, DEFAULT_TX_HEIGHT));
		it->tx = tx;
	}
}


//	Returns the item with the given ID, if it exists
equipmentPtr getEquipmentItemById(inventoryManager * eman, string id)
{
	for (auto it : eman->equipment)
	{
		if (it->id == id)
			return it;
	}
	cout << "ERROR: No item with ID " << id << endl;
	return nullptr;
}
