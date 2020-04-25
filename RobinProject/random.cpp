#include "pch.h"
#include "random.h"

void initrand()
{
	srand(time(0));
}

int randint(int a, int b)
{
	//If the range contains only one value, just return that value
	if (a == b)
		return a;

	//	error checking: b can't be less than a
	else if (b < a)
	{
		cout << "ERROR: Called randint with range " << a << ',' << b << endl;
		return 0;
	}

	// Otherwise return random
	return std::rand() % (b - a + 1) + a;
}

int randrange(int v)
{
	return randint(0, v - 1);
}
