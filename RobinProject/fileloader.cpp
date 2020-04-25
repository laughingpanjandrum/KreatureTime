#include "pch.h"
#include "fileloader.h"



//	Output the dialogue data to the console to make sure it loaded properly
void dialogueLoader::test_output_dialogue(dialoguePtr d)
{
	cout << "ID: " << d->id << endl;
	for (auto dop : d->options)
	{
		cout << "  Option [" + dop->id << "]";
		if (dop->first)
			cout << " (first)";
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
string dialogueLoader::strip_whitespace(const string s)
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
string dialogueLoader::generate_file_path(const string filename)
{
	return "data/dialogue/" + filename;
}


//	Takes a line formatted as:
//		"id" "value"
//	Strips out the quote marks; pair.first contains the id, pair.second contains the value.
pair<string, string> dialogueLoader::break_line(const string line)
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
	auto dop = doptionPtr(new dialogue_option());
	dop->startUnlocked = false;
	string line;
	while (getline(*f, line))
	{
		line = strip_whitespace(line);

		//	a preceding '}' marks the end of the dialogue option
		if (line[0] == '}')
			break;

		//	otherwise, try to parse the line
		else
		{
			auto b = break_line(line);
		
			//	ID of this options
			if (b.first == "option")
			{
				dop->id = b.second;
				if (b.second == DIALOGUE_FIRST_ID)
					dop->first = true;
			}

			//	Sets option to start unlocked
			else if (b.first == "unlocked")
			{
				if (b.second == "true")
					dop->startUnlocked = true;
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
		line = strip_whitespace(line);

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
			auto b = break_line(line);
			if (b.first == "id")
				d->id = b.second;
		}
	}

	//	Add the new dialogue to the manager
	dman->all.push_back(d);
}



//	Load every dialogue in the given file into the manager
void dialogueLoader::loadFile(const string filename, dialogueManager * dman)
{
	ifstream fobj;
	fobj.open(generate_file_path(filename));
	string line;
	while (getline(fobj, line))
	{
		if (line == DIALOGUE_START_MARK)
			loadDialogue(&fobj, dman);
	}
	fobj.close();

	test_output_dialogue(dman->all[0]);
}
