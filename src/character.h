#ifndef CARACTER_H
#define CARACTER_H

#include "tile.h"

class Character : public Tile
{
public:
	Character(Element, int = 0, int = 0, bool = false);
	Character(Character&);

	void Activate();
	void Deactivate();

	bool getState();
	Element getElement();

	void setState(bool state);

	void Show();

private:
	Element _element;
	bool _state;
};

#endif CARACTER_H
