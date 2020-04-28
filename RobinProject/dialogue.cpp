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
		youtxt->setString(wrap_text(dop->lines[l], 45));
	else
		themtxt->setString(wrap_text(dop->lines[l], 50));
}



void dialogueLoop(gamedataPtr gdata)
{
	bool done = false;
	sf::Event event;


	//	dialogue interface
	auto bg_tx_1 = getTextureFromFile("dialogue/you_frame.png");
	auto bg_tx_2 = getTextureFromFile("dialogue/npc_frame.png");
	sf::Sprite youframe, npcframe;
	youframe.setTexture(*bg_tx_1);
	npcframe.setTexture(*bg_tx_2);


	//	dialogue selection indicator
	auto check_tx = getTextureFromFile("dialogue/dot.png");


	//	(test using npc textures)
	auto npc = createNPCSprite(gdata->currentDialogue->withNpc, &gdata->nman);
	npc->setPosition(570, 290);


	//	position character portrait
	gdata->playerSprite->setPosition(65, 40);


	//	dialogue state
	auto cdialog = gdata->currentDialogue;
	int lineIdx = 0;
	doptionPtr currentOption = cdialog->options[0];


	//	text boxes for dialogs
	auto you_txtbox = createTextElement(&gdata->usefont, "", 260, 100, sf::Color(0, 0, 0), 15);
	auto them_txtbox = createTextElement(&gdata->usefont, "", 50, 400, sf::Color(0, 0, 0), 15);


	//	buttons (for selecting dialog options)
	vector<buttonPtr> buttons;


	//	initialize to first line
	bool viewingOptions = false;
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
			}

			//	Button handling
			else if (event.type == sf::Event::EventType::MouseButtonReleased)
			{
				//	we are cycling through lines
				if (!viewingOptions)
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
								createButton(&buttons, dop->id, tpos.x - 25, aty, 400, 30, check_tx);
								aty += 35;
							}
						}
						you_txtbox->setString(lines);
						them_txtbox->setString("");
						viewingOptions = true;
					}
				}

				//	otherwise, see if we clicked on a dialogue option
				else
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
							viewingOptions = false;
							updateDialogueDisplay(gdata, currentOption, 0, you_txtbox, them_txtbox);

							//	clear select, restart from the beginning of this option
							lineIdx = 0;
							buttons.clear();

							//	check whether the option should lock once it's read
							if (currentOption->lockWhenRead)
								currentOption->locked = true;
							currentOption->visited = true;

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
		}

		//	render the display
		gdata->rwindow->clear(sf::Color(255, 255, 255));

		//	interface elements
		gdata->rwindow->draw(youframe);
		gdata->rwindow->draw(npcframe);

		//	character portraits
		gdata->rwindow->draw(*gdata->playerSprite);
		gdata->rwindow->draw(*npc);

		//	dialogue lines
		gdata->rwindow->draw(*you_txtbox);
		gdata->rwindow->draw(*them_txtbox);

		//	dialog options, if any
		drawButtonList(gdata->rwindow, &buttons, sf::Mouse::getPosition(*gdata->rwindow));

		gdata->rwindow->display();
	}
}
