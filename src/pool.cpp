#include "pool.h"
#include <iostream>

using namespace std;

Pool::Pool(int x, int y, Element e)
{
	_element = e;
	SetPosition(x, y);
	SetType(POOL);
}

Pool::~Pool()
{
}

void Pool::SetElememt(Element e)
{
	_element = e;
}

Element Pool::GetElement()
{
	return _element;
}

void Pool::Show()
{
	if (_element == WATER)
	{
		cout << '-';
	}
	else if (_element == FIRE)
	{
		cout << '+';
	}
	else if (_element == GOO)
	{
		cout << '~';
	}
}
