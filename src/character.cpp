#include "character.h"
#include <iostream>

//using namespace std;

Character::Character(Element e, int x, int y, bool state)
{
	_element = e;
	SetPosition(x, y);
	SetType(CHARACTER);
	setState(state);
}

Character::Character(Character& character)
{
	_element = character._element;
	_state = character._state;
}

void Character::Activate()
{
	_state = true;
}

void Character::Deactivate()
{
	_state = false;
}

bool Character::getState()
{
	return _state;
}

Element Character::getElement()
{
	return _element;
}

void Character::setState(bool state)
{
	_state = state;
}

void Character::Show()
{
	if (_element == WATER)
	{
		
		std::cout << CYAN << 'E' << RESET;
	}
	else if (_element == FIRE)
	{
		std::cout << RED << 'F' << RESET;
	}
}
