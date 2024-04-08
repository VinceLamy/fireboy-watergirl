#include "button.h"
#include <iostream>

//using namespace	std;

Button::Button(int x, int y)
{
	SetPosition(x, y);
	SetType(BUTTON);
}

void Button::Show()
{
	std::cout << YELLOW << 'B' << RESET;
}
