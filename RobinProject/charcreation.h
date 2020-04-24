#pragma once

#include "gamedata.h"
#include "drawing.h"


int randrange(const int v, mt19937* rng);

void loopCharacterFeature(gamedataPtr gdata, const unsigned layer_index, vector<texturePtr>* txlist, int* idx, int vec);
void charCreationLoop(gamedataPtr gdata);