#pragma once

#include <string>
#include <vector>
#include <memory>

#include "imageloader.h"

using namespace std;


#define EQUIPMENT_IMAGE_FILE "data/img/items/equipment.png"


/*
	Hat
	Shirt
	Pants
	Shoes
	Glove
	Face
	Holding Item
*/


const unsigned SLOT_HEAD = 0;
const unsigned SLOT_BODY = 1;
const unsigned SLOT_LEGS = 2;
const unsigned SLOT_FEET = 3;
const unsigned SLOT_FACE = 4;
const unsigned SLOT_HANDS = 5;
const unsigned SLOT_HELD = 6;

const unsigned SLOT__MAX = 7;


string getItemSlotName(const unsigned slot);



struct equipmentPiece
{
	//	identification
	string id;
	string name;

	//	visual data
	texturePtr tx;
	int filex, filey;	//	where the sprite is located in the image file

	//	equipment slot
	unsigned slot;

	//	flags
	bool hasItem;	//	it's in our inventory
};
typedef shared_ptr<equipmentPiece> equipmentPtr;


struct inventoryManager
{
	vector<equipmentPtr> equipment;
};



void loadAllItemTextures(inventoryManager* eman);
equipmentPtr getEquipmentItemById(inventoryManager* eman, string id);