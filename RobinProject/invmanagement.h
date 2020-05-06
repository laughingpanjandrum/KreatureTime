#pragma once

#include "gamedata.h"
#include "equipment.h"
#include "drawing.h"


void renderPlayerEquipment(gamedataPtr gdata);

vector<equipmentPtr> getItemsForSlot(gamedataPtr gdata, const unsigned slot);
vector<spritePtr> createSpritesForEquipmentItems(vector<equipmentPtr>* items);

void updateItemListCategory(gamedataPtr gdata, vector<buttonPtr>* buttons, vector<spritePtr>* sprites, vector<equipmentPtr>* items, texturePtr check_tx);
void updateInventoryButtons(gamedataPtr gdata, vector<buttonPtr>* buttons, vector<sf::Text*>* txtbuttons, const unsigned currentSlot, texturePtr check_tx);

void inventoryManagementLoop(gamedataPtr gdata);