#include "pch.h"
#include "attributes.h"

string getAttributeName(const unsigned attr)
{
	switch (attr)
	{
	case(ATTR_APPEAL):		return "Appeal";
	case(ATTR_CUNNING):		return "Cunning";
	case(ATTR_DERRING_DO):	return "Derring-Do";
	case(ATTR_MYSTICISM):	return "Mysticism";
	case(ATTR_POMP):		return "Pomp";
	case(ATTR_VIOLENCE):	return "Violence";
	}
	return "{unknown attribute " + to_string(attr) + "}";
}
