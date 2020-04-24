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

texture_library * getButtonLibrary()
{
	auto tlib = new texture_library();
	auto img = new sf::Image();

	img->loadFromFile("img/cc_btn_pane.png");
	createTextureAndAdd(tlib, img, "btn_pane", 0, 0, 200, 516);
	
	img->loadFromFile("img/kreature_title.png");
	createTextureAndAdd(tlib, img, "kreature_title", 0, 0, 300, 200);

	img->loadFromFile("img/colors.png");
	createTextureAndAdd(tlib, img, "colors", 0, 0, 300, 200);

	return tlib;
}
