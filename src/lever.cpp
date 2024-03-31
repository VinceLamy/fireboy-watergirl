#include "lever.h"
#include <iostream>

using namespace	std;

Lever::Lever(int x, int y)
{
	SetPosition(x, y);
	SetType(LEVER);
}

void Lever::Show()
{
	if (GetState() == CLOSED)
		cout << YELLOW << 'L' << RESET;
	if (GetState() == OPEN)
		cout << YELLOW << '<' << RESET;
}