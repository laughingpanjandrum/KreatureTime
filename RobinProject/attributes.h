#pragma once

#include <string>
#include <vector>

using namespace std;



/*

Violence
Appeal
Pomp
Presence
Derring-Do

*/

const unsigned ATTR_APPEAL = 0;
const unsigned ATTR_CUNNING = 1;
const unsigned ATTR_DERRING_DO = 2;
const unsigned ATTR_MYSTICISM = 3;
const unsigned ATTR_POMP = 4;
const unsigned ATTR_VIOLENCE = 5;

const unsigned ATTR__MAX = 6;


string getAttributeName(const unsigned attr);