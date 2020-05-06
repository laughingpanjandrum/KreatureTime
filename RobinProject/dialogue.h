#pragma once

#include "imageloader.h"
#include "fileloader.h"
#include "gamedata.h"
#include "drawing.h"
#include "npc.h"


enum dialogueOptionState
{
	DOPTION_LOCKED,
	DOPTION_DONT_MEET_REQ,
	DOPTION_AVAILABLE,
};


string wrap_text(const string line, int linelen);
doptionPtr getDialogueOptionById(dialoguePtr d, const string id);
dialoguePtr getDialogueById(dialogueManager* dman, const string id);

dialogueOptionState getDialogueOptionState(gamedataPtr gdata, doptionPtr dop);

void updateDialogueDisplay(gamedataPtr gdata, doptionPtr dop, int l, sf::Text* youtxt, sf::Text* themtxt);
void dialogueLoop(gamedataPtr gdata, spritePtr npc);