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


//	Returns the state of the given dialogue option, ie whether it's currently available, & if not, WHY not
dialogueOptionState getDialogueOptionState(gamedataPtr gdata, doptionPtr dop)
{
	//	Can't access because it's locked.
	if (dop->locked)
		return DOPTION_LOCKED;

	//	Visible, but can't access because we don't have the required stats.
	else if (dop->statReq != ATTR__MAX && gdata->playerData->getAttributeValue(dop->statReq) < dop->statReqAmount)
		return DOPTION_DONT_MEET_REQ;

	//	Otherwise, it's available.
	return DOPTION_AVAILABLE;
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



void dialogueLoop(gamedataPtr gdata, spritePtr npc)
{
	bool done = false;
	sf::Event event;

	//	dialogue selection indicator
	auto check_tx = getTextureFromFile("dialogue/dot.png");

	//	position NPC image
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

	//	list of text-boxes, each containing a single dialogue option
	vector<sf::Text*> doptions;


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
			//	closing the window quits instantly and horribly
			if (event.type == sf::Event::EventType::Closed)
				done = true;


			//	Button handling
			if (event.type == sf::Event::EventType::MouseButtonReleased)
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
						//	if the current option has the 'ends' flag set, the conversation ends now
						if (currentOption->endsConversation)
							done = true;

						//	otherwise, show dialogue options
						else
						{
							//	create a button for each dialogue option
							buttons.clear();
							doptions.clear();
							auto tpos = you_txtbox->getPosition();
							int aty = tpos.y - 10;

							//	add every option to the textbox
							for (unsigned i = 0; i < cdialog->options.size(); i++)
							{
								auto dop = cdialog->options[i];
								auto dop_state = getDialogueOptionState(gdata, dop);

								//	Option is available, display it normally
								if (dop_state == DOPTION_AVAILABLE)
								{
									//	indicate whether a stat is being used to select this
									string txt = dop->title;
									if (dop->statReq != ATTR__MAX)
										txt += " (" + getAttributeName(dop->statReq) + ' ' + to_string(dop->statReqAmount) + ")";

									//	options we have already visited are greyed out
									sf::Color opcol(0, 0, 0);
									if (dop->visited)
										opcol = sf::Color(100, 100, 100);
									doptions.push_back(createTextElement(&gdata->usefont, txt, tpos.x, aty, opcol, 15));
									
									//	clickable region
									createButton(&buttons, dop->id, tpos.x - 25, aty, 400, 30, check_tx);
									aty += 35;
								}

								//	Option is NOT available, but is still visible
								else if (dop_state == DOPTION_DONT_MEET_REQ)
								{
									int val = gdata->playerData->getAttributeValue(dop->statReq);
									string txt = dop->title + " (" + getAttributeName(dop->statReq) + ' ' + to_string(val) + '/' + to_string(dop->statReqAmount) + ")";
									doptions.push_back(createTextElement(&gdata->usefont, txt, tpos.x, aty, sf::Color(127, 0, 0), 15));
									aty += 35;
								}
							}

							//	update the textbox, clear the NPC's dialogue
							you_txtbox->setString("");
							them_txtbox->setString("");
							viewingOptions = true;
						}
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
							doptions.clear();

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
		gdata->rwindow->clear();

		//	background of whatever the previous location was
		gdata->rwindow->draw(*gdata->bg_sprite);
		gdata->rwindow->draw(*gdata->fg_sprite);

		//	interface elements
		gdata->rwindow->draw(*gdata->dialogue_frame_1);
		gdata->rwindow->draw(*gdata->dialogue_frame_2);

		//	character portraits
		gdata->rwindow->draw(*gdata->playerSprite);
		gdata->rwindow->draw(*npc);

		//	dialogue lines
		gdata->rwindow->draw(*you_txtbox);
		gdata->rwindow->draw(*them_txtbox);

		//	dialog options, if any
		for (auto dline : doptions)
			gdata->rwindow->draw(*dline);
		drawButtonList(gdata->rwindow, &buttons, sf::Mouse::getPosition(*gdata->rwindow));

		gdata->rwindow->display();
	}
}
