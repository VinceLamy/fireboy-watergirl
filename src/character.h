#ifndef CARACTER_H
#define CARACTER_H

#include "tile.h"

class Character : public Tile
{
public:
	Character(Element, int = 0, int = 0, bool = false);
	Character(Character&);
	~Character();

	void Activate();
	void Deactivate();

	bool getState();
	Element getElement();
	Coordinate getSpeed();

	void setState(bool state);
	void setElement(Element element);
	void setSpeed(int speedX, int speedY);

	void Show();

private:
	Element _element;
	//float _gravite;
	bool _state;
	Coordinate _speed;
};

#endif CARACTER_H
