#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;


#define DIALOGUE_START_MARK	"[DIALOGUE]"
#define DIALOGUE_END_MARK	"[END]"

//	a dialogue option with this ID is automatically the starting option
#define DIALOGUE_FIRST_ID	"_FIRST"




struct dialogue_option
{
	string			id;
	string			title;
	vector<string>	lines;
	vector<bool>	speaker;

	vector<string>	unlocks;

	bool			first;
	bool			startUnlocked;
	bool			locked;
};
typedef shared_ptr<dialogue_option> doptionPtr;


struct dialogue
{
	string				id;
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

	string strip_whitespace(const string s);
	string generate_file_path(const string filename);

	pair<string, string> break_line(const string line);

	doptionPtr loadDialogueOption(ifstream* f);
	void loadDialogue(ifstream* f, dialogueManager* dman);
	void loadFile(const string filename, dialogueManager* dman);

}