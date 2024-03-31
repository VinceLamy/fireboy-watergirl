#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "tile.h"

class Controller : public Tile
{
public:
	void SetState(State s);
	State GetState();

	virtual void Show() = 0;

private:
	State _state = CLOSED;
};

#endif CONTROLLER_H
