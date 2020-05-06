#include "pch.h"
#include "fileloader.h"



//	Output the dialogue data to the console to make sure it loaded properly
void dialogueLoader::test_output_dialogue(dialoguePtr d)
{
	cout << "ID: " << d->id << endl;
	for (auto dop : d->options)
	{
		cout << "  Option [" + dop->id << "]";
		if (dop->startUnlocked)
			cout << " (starts unlocked)";
		cout << endl;

		for (auto ul : dop->unlocks)
			cout << "   unlocks '" << ul << "'" << endl;

		for (unsigned i = 0; i < dop->lines.size(); i++)
		{
			if (dop->speaker[i])
				cout << "    YOU: " << dop->lines[i] << endl;
			else
				cout << "    THEM: " << dop->lines[i] << endl;
		}
		cout << endl;
	}
}



//	Removes preceding and trailing whitespace, AND all internal whitespace that is NOT contained in quotation marks
string fileLoader::strip_whitespace(const string s)
{
	string nstr = "";
	bool inquotes = false;
	for (unsigned i = 0; i < s.size(); i++)
	{
		if (s[i] == '"')
		{
			inquotes = !inquotes;
			nstr += s[i];
		}
		else if (inquotes || (s[i] != '\t' && s[i] != ' '))
			nstr += s[i];
	}
	return nstr;
}

//	Gets the actual file path, given just the bare filename
string fileLoader::generate_file_path(const string subfolder, const string filename)
{
	return "data/" + subfolder + "/" + filename;
}


//	Takes a line formatted as:
//		"id" "value"
//	Strips out the quote marks; pair.first contains the id, pair.second contains the value.
pair<string, string> fileLoader::break_line(const string line)
{
	pair<string, string> result;
	string chunk = "";
	bool first = true;
	bool inquotes = false;
	for (unsigned i = 0; i < line.size(); i++)
	{
		//	a quote mark means either:
		//		start of the id
		//		end of the id
		//		start of the second value
		//		end of the second value (return)
		if (line[i] == '"')
		{
			if (inquotes)
			{
				if (first)
				{
					result.first = chunk;
					chunk = "";
					first = false;
				}
				else
				{
					result.second = chunk;
					return result;
				}
			}
			inquotes = !inquotes;
		}

		//	any other symbol is ignored if we're in quotes, & appended otherwise
		else if (inquotes)
			chunk += line[i];
	}

	//	If we don't close all the strings properly, we'll end up with garbage
	//	so we return an empty set instead and throw up a message.
	cout << "WARNING: A dialogue line wasn't formatted correctly! I ended up with this: (\"" << result.first << "\"), (\"" << result.second << "\"" << endl;
	return pair<string, string>();
}



//	Loads the current dialogue option until we hit a closing brace.
doptionPtr dialogueLoader::loadDialogueOption(ifstream * f)
{
	//	default values for a blank option
	auto dop = doptionPtr(new dialogue_option());
	dop->statReq = ATTR__MAX;
	dop->startUnlocked = false;
	dop->lockWhenRead = false;
	dop->visited = false;
	dop->endsConversation = false;
	
	
	//	load in the option
	string line;
	while (getline(*f, line))
	{
		line = fileLoader::strip_whitespace(line);

		//	a preceding '}' marks the end of the dialogue option
		if (line[0] == '}')
			break;

		//	otherwise, try to parse the line
		else
		{
			auto b = fileLoader::break_line(line);
		
			//	ID of this options
			if (b.first == "option")
				dop->id = b.second;

			//	Sets flags
			else if (b.first == "flag")
			{
				if (b.second == "unlocked")
					dop->startUnlocked = true;
				else if (b.second == "lockWhenRead")
					dop->lockWhenRead = true;
				else if (b.second == "ends")
					dop->endsConversation = true;
				else
					cout << "ERROR: Unrecognized flag " << b.second << " in dialogue with id " << dop->id << endl;
			}

			//	Requirements
			else if (b.first == "reqAppeal")
			{
				dop->statReq = ATTR_APPEAL;
				dop->statReqAmount = stoi(b.second);
			}
			else if (b.first == "reqCunning")
			{
				dop->statReq = ATTR_CUNNING;
				dop->statReqAmount = stoi(b.second);
			}
			else if (b.first == "reqDerringDo")
			{
				dop->statReq = ATTR_DERRING_DO;
				dop->statReqAmount = stoi(b.second);
			}
			else if (b.first == "reqMysticism")
			{
				dop->statReq = ATTR_MYSTICISM;
				dop->statReqAmount = stoi(b.second);
			}
			else if (b.first == "reqPomp")
			{
				dop->statReq = ATTR_POMP;
				dop->statReqAmount = stoi(b.second);
			}
			else if (b.first == "reqViolence")
			{
				dop->statReq = ATTR_VIOLENCE;
				dop->statReqAmount = stoi(b.second);
			}

			//	Selecting this option unlocks other options with the given ids
			else if (b.first == "unlock")
				dop->unlocks.push_back(b.second);

			//	Title of the dialogue option (displayed when selecting what to say)
			else if (b.first == "title")
				dop->title = b.second;

			//	Something you say
			else if (b.first == "you")
			{
				dop->lines.push_back(b.second);
				dop->speaker.push_back(true);
			}

			//	Something they say
			else if (b.first == "them")
			{
				dop->lines.push_back(b.second);
				dop->speaker.push_back(false);
			}

			//	Error
			else
				cout << "ERROR! Dialogue loader doesn't recognize the option id: '" << b.first << "'." << endl;
		}
	}
	dop->locked = !dop->startUnlocked;
	return dop;
}


//	Loads dialogue options until we hit the END mark.
void dialogueLoader::loadDialogue(ifstream * f, dialogueManager * dman)
{
	auto d = dialoguePtr(new dialogue());
	string line;

	//	Read in data for the dialogue line
	while (getline(*f, line))
	{
		line = fileLoader::strip_whitespace(line);

		//	end of dialogue reach
		if (line == DIALOGUE_END_MARK)
			break;

		//	a preceding '{' indicates the start of a new dialogue option
		if (line[0] == '{')
		{
			auto dop = loadDialogueOption(f);
			d->options.push_back(dop);
		}

		//	a preceding '}' means we have some kind of error
		else if (line[0] == '}')
		{
			cout << "Whoops, there's an extra brace here!" << endl;
		}

		//	otherwise, try to parse the line
		else
		{
			auto b = fileLoader::break_line(line);
			if (b.first == "id")
				d->id = b.second;
			else if (b.first == "npc")
				d->withNpc = b.second;
		}
	}

	//	Add the new dialogue to the manager
	dman->all.push_back(d);
}



//	Load every dialogue in the given file into the manager
void dialogueLoader::loadFile(const string filename, dialogueManager * dman)
{
	ifstream fobj;
	fobj.open(fileLoader::generate_file_path(SUBFOLDER_DIALOGUE, filename));
	string line;
	while (getline(fobj, line))
	{
		if (line == DIALOGUE_START_MARK)
			loadDialogue(&fobj, dman);
	}
	fobj.close();

//	test_output_dialogue(dman->all[0]);
}


//	Frames contain data about the sprite coordinates at which to centre & the player's position in the frame.
lframePtr locationLoader::loadFrame(ifstream * f)
{
	auto lf = lframePtr(new location_frame());
	string line;
	while (getline(*f, line))
	{
		line = fileLoader::strip_whitespace(line);
		if (line == "[END]")
			return lf;

		else if (!line.empty())
		{
			auto dat = fileLoader::break_line(line);
			
			if (dat.first == "x")
				lf->framex = stoi(dat.second);
			else if (dat.first == "y")
				lf->framey = stoi(dat.second);

			else if (dat.first == "player_x")
				lf->playerx = stoi(dat.second);
			else if (dat.first == "player_y")
				lf->playery = stoi(dat.second);

			else
				cout << "Unrecognized frame data: " << dat.first << "," << dat.second << endl;
		}
	}

	cout << "ERROR: File terminated before end of frame!" << endl;
	return lf;
}


//	An NPC appearing in a location.
lnpcPtr locationLoader::loadNpc(ifstream * f)
{
	auto npc = lnpcPtr(new location_npc());
	string line;
	while (getline(*f, line))
	{
		line = fileLoader::strip_whitespace(line);
		if (line == "[END]")
			return npc;

		else if (!line.empty())
		{
			auto dat = fileLoader::break_line(line);

			if (dat.first == "x")
				npc->x = stoi(dat.second);
			else if (dat.first == "y")
				npc->y = stoi(dat.second);

			else if (dat.first == "id")
				npc->npcId = dat.second;
			else if (dat.first == "dialogue")
				npc->dialogueId = dat.second;

			else
				cout << "Unrecognized location NPC data: " << dat.first << "," << dat.second << endl;
		}
	}

	cout << "ERROR: File terminated before end of NPC data!" << endl;
	return npc;
}



//	Load an entire location from a given file.
void locationLoader::loadLocation(ifstream * f, locationManager * lman)
{
	auto loc = locationPtr(new location());
	string line;
	while (getline(*f, line))
	{
		line = fileLoader::strip_whitespace(line);
		if (line == "[FRAME]")
			loc->frames.push_back(loadFrame(f));
		else if (line == "[NPC]")
			loc->npcs.push_back(loadNpc(f));

		else if (!line.empty())
		{
			auto dat = fileLoader::break_line(line);

			if (dat.first == "id")
				loc->id = dat.second;
			else if (dat.first == "back")
				loc->bg_texture = dat.second;
			else if (dat.first == "fore")
				loc->fg_texture = dat.second;
			else if (dat.first == "name")
				loc->name = dat.second;
		}
	}
	lman->all.push_back(loc);
	cout << "Generated location with id '" << loc->id << "' named '" << loc->name << "'." << endl;
	//cout << "  Start: " << loc->startx << "," << loc->starty;
	//cout << "; Frame: " << loc->framex << "," << loc->framey << endl;
}


//	Load location data from a given file, adding it to the manager.
void locationLoader::loadFile(const string filename, locationManager * lman)
{
	ifstream fobj;
	fobj.open(fileLoader::generate_file_path(SUBFOLDER_LOCATIONS, filename));
	if (fobj.is_open())
	{
		loadLocation(&fobj, lman);
		fobj.close();
	}
	else
		cout << "ERROR! Couldn't open location file " << filename << endl;
}


npcDataPtr npcLoader::loadNPC(ifstream * f)
{
	string line;
	auto npc = npcDataPtr(new npcData());

	//	default size values
	npc->w = 200;
	npc->h = 300;

	//	load data
	while (getline(*f, line))
	{
		line = fileLoader::strip_whitespace(line);
		if (line == "[END]")
			return npc;

		else
		{
			auto dat = fileLoader::break_line(line);
			
			if (dat.first == "id")
				npc->id = dat.second;
			else if (dat.first == "file")
				npc->file = dat.second;
			
			else if (dat.first == "x")
				npc->x = stoi(dat.second);
			else if (dat.first == "y")
				npc->y = stoi(dat.second);
			
			else if (dat.first == "w")
				npc->w = stoi(dat.second);
			else if (dat.first == "h")
				npc->h = stoi(dat.second);
		}
	}

	cout << "ERROR: NPC with id '" << npc->id << "' didn't terminate correctly" << endl;
	return npc;
}


void npcLoader::loadFile(const string filename, npcDataManager * nman)
{
	ifstream fobj;
	fobj.open(fileLoader::generate_file_path(SUBFOLDER_NPCS, filename));
	if (fobj.is_open())
	{
		string line;
		while (getline(fobj, line))
		{
			if (fileLoader::strip_whitespace(line) == "[NPC]")
				nman->all.push_back(loadNPC(&fobj));
		}
		fobj.close();
	}
	else
		cout << "ERROR! Couldn't open NPC file " << filename << endl;

	//	debugger
	cout << "Loaded NPCS:" << endl;
	for (auto npc : nman->all)
		cout << " id '" << npc->id << "', file '" << npc->file << "', rectangle " << npc->x << ',' << npc->y << ',' << npc->x << ',' << npc->y << endl;
}





//=-=-=======================================================================================
//		EQUIPMENT




equipmentPtr itemLoader::loadEquipmentPiece(ifstream * f)
{
	auto it = equipmentPtr(new equipmentPiece());
	it->hasItem = false;
	string line;

	while (getline(*f, line))
	{
		//	finish if we get a closing brace
		line = fileLoader::strip_whitespace(line);
		if (line == "}")
			return it;

		//	otherwise parse the line
		else
		{
			auto dat = fileLoader::break_line(line);
			
			//	string data
			if (dat.first == "id")
				it->id = dat.second;
			else if (dat.first == "name")
				it->name = dat.second;

			//	texture position
			else if (dat.first == "x")
				it->filex = stoi(dat.second);
			else if (dat.first == "y")
				it->filey = stoi(dat.second);

			//	equip slot
			else if (dat.first == "slot")
			{
				if (dat.second == "head")
					it->slot = SLOT_HEAD;
				else if (dat.second == "face")
					it->slot = SLOT_FACE;
				else if (dat.second == "body")
					it->slot = SLOT_BODY;
				else if (dat.second == "legs")
					it->slot = SLOT_LEGS;
				else if (dat.second == "feet")
					it->slot = SLOT_FEET;
				else if (dat.second == "hands")
					it->slot = SLOT_HANDS;
				else if (dat.second == "held")
					it->slot = SLOT_HELD;
				else
					cout << "ERROR: Unknown item slot '" << dat.second << "' (item id " << it->id << ")" << endl;
			}

			//	error
			else
				cout << "ERROR: Bad item tag " << dat.first << " for item with id " << it->id << endl;
		}
	}

	//	Whoops!
	cout << "ERROR: File terminated before item block!" << endl;
	return it;
}



void itemLoader::loadFile(const string filename, inventoryManager * eman)
{
	ifstream fobj;
	fobj.open(fileLoader::generate_file_path(SUBFOLDER_ITEMS, filename));
	if (fobj.is_open())
	{
		string line;
		while (getline(fobj, line))
		{
			if (fileLoader::strip_whitespace(line) == "{")
				eman->equipment.push_back(loadEquipmentPiece(&fobj));
		}
		fobj.close();
	}
	else
		cout << "ERROR! Couldn't open item file " << filename << endl;
}
