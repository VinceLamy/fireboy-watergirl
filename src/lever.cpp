#include "lever.h"
#include <iostream>

using namespace	std;

Lever::Lever(int x, int y)
{
	SetPosition(x, y);
	SetType(LEVER);
	//SetState(OPEN); //Pour CheckGateTest
}

Lever::Lever()
{
}

void Lever::Show()
{
	if (GetState() == CLOSED)
		cout << YELLOW << 'L' << RESET;
	if (GetState() == OPEN)
		cout << YELLOW << '<' << RESET;
}