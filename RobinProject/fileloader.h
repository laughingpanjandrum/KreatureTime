#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "attributes.h"
#include "equipment.h"

using namespace std;


#define SUBFOLDER_DIALOGUE	"dialogue"
#define SUBFOLDER_LOCATIONS	"locations"
#define SUBFOLDER_NPCS		"npc"
#define SUBFOLDER_ITEMS		"items"

#define DIALOGUE_START_MARK	"[DIALOGUE]"
#define DIALOGUE_END_MARK	"[END]"



// ..........................................................................
//		Utility functions for file loading


namespace fileLoader
{
	string strip_whitespace(const string s);
	string generate_file_path(const string subfolder, const string filename);

	pair<string, string> break_line(const string line);
}



// ..........................................................................
//		Data structures for dialogues



struct dialogue_option
{
	//		Identification.
	string			id;

	//		Text displayed.
	string			title;
	vector<string>	lines;
	vector<bool>	speaker;

	//		Requirements.
	unsigned		statReq;
	int				statReqAmount;

	//		Rewards.
	vector<string>	unlocks;

	//		Flags.
	bool			lockWhenRead;
	bool			startUnlocked;
	bool			endsConversation;
	bool			locked;
	bool			visited;
};
typedef shared_ptr<dialogue_option> doptionPtr;


struct dialogue
{
	string				id;
	string				withNpc;
	vector<doptionPtr>	options;
};
typedef shared_ptr<dialogue> dialoguePtr;



struct dialogueManager
{
	vector<dialoguePtr> all;
};




namespace dialogueLoader
{

	void test_output_dialogue(dialoguePtr d);

	doptionPtr loadDialogueOption(ifstream* f);
	void loadDialogue(ifstream* f, dialogueManager* dman);
	void loadFile(const string filename, dialogueManager* dman);

}





// ..........................................................................
//		Data structures for locations



struct location_frame
{
	int framex, framey;
	int playerx, playery;
};
typedef shared_ptr<location_frame> lframePtr;


struct location_npc
{
	string npcId;
	int x, y;
	string dialogueId;
};
typedef shared_ptr<location_npc> lnpcPtr;


struct location
{
	string id;		//	unique identifier for this location
	string name;	//	name (displayed to the player)


	//	foreground/background textures
	string bg_texture;
	string fg_texture;


	//	npc data
	vector<lnpcPtr> npcs;


	//	frames available in the location
	vector<lframePtr> frames;
};
typedef shared_ptr<location> locationPtr;


struct locationManager
{
	vector<locationPtr> all;
};



namespace locationLoader
{

	lframePtr loadFrame(ifstream* f);
	lnpcPtr loadNpc(ifstream* f);
	void loadLocation(ifstream* f, locationManager* lman);
	void loadFile(const string filename, locationManager* lman);

}




// ..........................................................................
//		Data structures for NPCs


struct npcData
{
	string id;
	string file;
	int x, y, w, h;
};
typedef shared_ptr<npcData> npcDataPtr;


struct npcDataManager
{
	vector<npcDataPtr> all;
};


namespace npcLoader
{

	npcDataPtr loadNPC(ifstream* f);
	void loadFile(const string filename, npcDataManager* nman);

}




// ..........................................................................
//		Item data


namespace itemLoader
{

	equipmentPtr loadEquipmentPiece(ifstream* f);
	void loadFile(const string filename, inventoryManager* eman);

}