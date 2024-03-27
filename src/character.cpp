#include "character.h"
#include <iostream>

using namespace std;

Character::Character(Element e, int x, int y, bool state)
{
	_element = e;
	SetPosition(x, y);
	SetType(CHARACTER);
	setState(state);
}

Character::Character(Character& character)
{
	_speed = character._speed;
	_element = character._element;
	_state = character._state;
}

Character::~Character()
{

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
Coordinate Character::getSpeed()
{
	return _speed;
}

void Character::setState(bool state)
{
	_state = state;
}

void Character::setSpeed(int speedX, int speedY)
{
	_speed.x = speedX;
	_speed.y = speedY;
}

void Character::Show()
{
	if (_element == WATER)
	{
		
		cout << CYAN << 'E' << RESET;
	}
	else if (_element == FIRE)
	{
		cout << RED << 'F' << RESET;
	}
}
