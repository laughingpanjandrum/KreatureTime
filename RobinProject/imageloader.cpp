#include "pch.h"
#include "imageloader.h"

texturePtr getTextureByLabel(texture_library * tlib, const string label)
{
	auto f = find(tlib->labels.begin(), tlib->labels.end(), label);
	if (f != tlib->labels.end())
		return tlib->textures[f - tlib->labels.begin()];
	return texturePtr();
}


//	Adds a texture with the given label to the library.
void addTextureToLibrary(texture_library * tlib, const string label, texturePtr tx)
{
	tlib->labels.push_back(label);
	tlib->textures.push_back(tx);
}


//	Generates a texture using the given data and adds it to the given library.
void createTextureAndAdd(texture_library * tlib, sf::Image * img, const string label, const int x, const int y, const int w, const int h)
{
	addTextureToLibrary(tlib, label, getTextureFromImage(img, x, y, w, h));
}


//	Creates a new texture from the given chunk of an image.
texturePtr getTextureFromImage(sf::Image * img, const int x, const int y, const int w, const int h)
{
	sf::Texture* tx = new sf::Texture();
	tx->loadFromImage(*img, sf::IntRect(x, y, w, h));
	return texturePtr(tx);
}


//	Creates and returns the texture sheet for character creation
texture_library * getCreatureLibrary()
{
	auto tlib = new texture_library();
	auto img = new sf::Image();

	img->loadFromFile("img/cc_face_parts.png");
	//img->createMaskFromColor(sf::Color(255, 105, 180));

	createTextureAndAdd(tlib, img, "base",		0, 0);
	for (int i = 0; i < 14; i++)
		createTextureAndAdd(tlib, img, "eyes_" + to_string(i), 200 + i * 200, 0);
	for (int i = 0; i < 10; i++)
		createTextureAndAdd(tlib, img, "nose_" + to_string(i), i * 200, 300);
	for (int i = 0; i < 12; i++)
		createTextureAndAdd(tlib, img, "mouth_" + to_string(i), i * 200, 600);
	for (int i = 0; i < 8; i++)
		createTextureAndAdd(tlib, img, "brow_" + to_string(i), i * 200, 900);
	for (int i = 0; i < 9; i++)
		createTextureAndAdd(tlib, img, "hair_" + to_string(i), i * 200, 1200);
	for (int i = 0; i < 6; i++)
		createTextureAndAdd(tlib, img, "bangs_" + to_string(i), i * 200, 1500);

	createTextureAndAdd(tlib, img, "empty", 0, 1800);

	return tlib;
}

texture_library * getCharCreationLibrary()
{
	auto tlib = new texture_library();
	auto img = new sf::Image();

	img->loadFromFile("img/cc_screen.png");
	createTextureAndAdd(tlib, img, "screen", 0, 0, 800, 600);

	img->loadFromFile("img/cc_buttons.png");
	createTextureAndAdd(tlib, img, "btn_reset", 40, 65, 140, 110);
	createTextureAndAdd(tlib, img, "btn_randomize", 33, 180, 150, 150);
	createTextureAndAdd(tlib, img, "btn_arrow", 496, 44, 60, 60);
	createTextureAndAdd(tlib, img, "btn_accept", 595, 520, 205, 70);
	createTextureAndAdd(tlib, img, "question", 755, 465, 35, 45);

	return tlib;
}

texture_library * getStatSelectionLibrary()
{
	auto tlib = new texture_library();
	auto img = new sf::Image();

	img->loadFromFile("img/cc_mirror.png");
	createTextureAndAdd(tlib, img, "bg", 0, 0, 800, 600);

	img->loadFromFile("img/cc_mirror_buttons.png");
	createTextureAndAdd(tlib, img, "btn_circle1", 575, 450, 80, 70);
	createTextureAndAdd(tlib, img, "btn_circle2", 690, 490, 110, 105);

	return tlib;
}
