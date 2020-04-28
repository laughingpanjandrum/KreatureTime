#pragma once

#include "gamedata.h"
#include "imageloader.h"
#include "fileloader.h"


#define FRAME_TRANSITION_RATE	3


locationPtr getLocationById(locationManager* lman, const string id);

void positionBackground(spritePtr bg, spritePtr fg, const int x, const int y);
void positionSpriteInFrame(spritePtr spr, spritePtr frame, const int x, const int y);
void setCurrentFrame(gamedataPtr gdata, lframePtr frame);

void stepFrameTransition(gamedataPtr gdata);
void cycleFrame(gamedataPtr gdata, int* frameIdx, int vec);

void explorationLoop(gamedataPtr gdata);