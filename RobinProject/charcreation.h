#pragma once

#include "gamedata.h"
#include "drawing.h"
#include "random.h"



void loopCharacterFeature(gamedataPtr gdata, const unsigned layer_index, vector<texturePtr>* txlist, int* idx, int vec);
void finishCharacter(gamedataPtr gdata);
void charCreationLoop(gamedataPtr gdata);