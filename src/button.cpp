#include "button.h"
#include <iostream>

using namespace	std;

Button::Button(int x, int y)
{
	SetPosition(x, y);
	SetType(BUTTON);
	//SetState(OPEN); //Pour CheckGateTest
}

Button::Button()
{
}

void Button::Show()
{
	cout << 'B';
}
