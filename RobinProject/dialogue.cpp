#include "pch.h"
#include "dialogue.h"


//	returns text with newline characters inserted whenever a word would push the current chunk past the maximum line length.
string wrap_text(const string line, int linelen)
{
	string finaltxt, word, chunk;
	for (unsigned i = 0; i < line.size(); i++)
	{
		//	end of word
		if (line[i] == ' ')
		{
			if ((chunk + word).size() > linelen)
			{
				finaltxt += chunk + "\n";
				chunk = word + ' ';
			}
			else
				chunk += word + ' ';
			word = "";
		}
		else
			word += line[i];
	}
	if (!word.empty())
		chunk += word;
	if (!chunk.empty())
		finaltxt += chunk;
	return finaltxt;
}


//	Returns pointer to the option with the given id in the given dialog, if one exists.
doptionPtr getDialogueOptionById(dialoguePtr d, const string id)
{
	for (auto dop : d->options)
	{
		if (dop->id == id)
			return dop;
	}
	cout << "ERROR! Coudln't find dialogue option with id " << id << endl;
	return nullptr;
}


dialoguePtr getDialogueById(dialogueManager * dman, const string id)
{
	for (auto d : dman->all)
	{
		if (d->id == id)
			return d;
	}
	cout << "ERROR! Couldn't find dialogue with id " << id << endl;
	return nullptr;
}


//	Updates dialogue box to display the given line.
void updateDialogueDisplay(gamedataPtr gdata, doptionPtr dop, int l, sf::Text* youtxt, sf::Text* themtxt)
{
	youtxt->setString("");
	themtxt->setString("");
	if (dop->speaker[l])
		youtxt->setString(wrap_text(dop->lines[l], 40));
	else
		themtxt->setString(wrap_text(dop->lines[l], 40));
}



void dialogueLoop(gamedataPtr gdata)
{
	bool done = false;
	sf::Event event;


	//	dialog-specific textures
	auto tlib = getCharCreationLibrary();


	//	(test using npc textures)
	auto npclib = getNPCLibrary();
	sf::Sprite npc;
	npc.setTexture(*getTextureByLabel(npclib, "reggie_p"));
	npc.setPosition(585, 285);


	//	position character portraits
	gdata->playerSprite->setPosition(15, 15);


	//	dialogue state
	auto cdialog = gdata->currentDialogue;
	int lineIdx = 0;
	doptionPtr currentOption = cdialog->options[0];


	//	text boxes for dialogs
	auto you_txtbox = createTextElement(&gdata->usefont, "", 230, 100, sf::Color(0, 0, 0), 20);
	auto them_txtbox = createTextElement(&gdata->usefont, "", 50, 400, sf::Color(0, 0, 0), 20);


	//	buttons (for selecting dialog options)
	vector<buttonPtr> buttons;


	//	initialize to first line
	updateDialogueDisplay(gdata, currentOption, 0, you_txtbox, them_txtbox);


	//	viewing loop
	while (!done)
	{
		//	get input
		while (gdata->rwindow->pollEvent(event))
		{
			if (event.type == sf::Event::EventType::Closed)
				done = true;


			//	Quit w/ ESCAPE
			if (event.type == sf::Event::EventType::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
					done = true;

				//	next line of dialog
				else if (event.key.code == sf::Keyboard::Space)
				{
					//	next line, if any remain
					if (lineIdx < currentOption->lines.size() - 1)
					{
						lineIdx++;
						updateDialogueDisplay(gdata, currentOption, lineIdx, you_txtbox, them_txtbox);
					}

					//	if we've reached the end, show dialogue options
					else
					{
						buttons.clear();
						auto tpos = you_txtbox->getPosition();
						string lines = "";
						int aty = tpos.y - 10;
						for (unsigned i = 0; i < cdialog->options.size(); i++)
						{
							auto dop = cdialog->options[i];
							if (!dop->locked)
							{
								lines += dop->title + "\n\n";
								createButton(&buttons, dop->id, tpos.x - 20, aty, 100, 30, getTextureByLabel(tlib, "btn_arrow"));
								aty += 40;
							}
						}
						you_txtbox->setString(lines);
						them_txtbox->setString("");
					}
				}
			}

			//	Button handling
			else if (event.type == sf::Event::EventType::MouseButtonReleased)
			{
				//	figure out what part of the button pane we clicked on, if any
				auto mpos = sf::Mouse::getPosition(*gdata->rwindow);
				auto btn = getButtonUnderPoint(mpos.x, mpos.y, &buttons);

				//	get dialogue option with that id
				if (btn != nullptr)
				{
					//	load the dialog option
					currentOption = getDialogueOptionById(cdialog, btn->_id);
					if (currentOption != nullptr)
					{
						//	update visible text
						updateDialogueDisplay(gdata, currentOption, 0, you_txtbox, them_txtbox);

						//	clear select, restart from the beginning of this option
						lineIdx = 0;
						buttons.clear();

						//	check for options unlocked by this option
						for (auto unlock_id : currentOption->unlocks)
						{
							auto dop = getDialogueOptionById(cdialog, unlock_id);
							if (dop != nullptr)
								dop->locked = false;
						}
					}
				}
			}
		}

		//	render the display
		gdata->rwindow->clear(sf::Color(255, 255, 255));

		//	character portraits
		gdata->rwindow->draw(*gdata->playerSprite);
		gdata->rwindow->draw(npc);

		//	dialogue lines
		gdata->rwindow->draw(*you_txtbox);
		gdata->rwindow->draw(*them_txtbox);

		//	dialog options, if any
		drawButtonList(gdata->rwindow, &buttons, sf::Mouse::getPosition(*gdata->rwindow));

		gdata->rwindow->display();
	}
}
