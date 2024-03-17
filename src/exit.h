#ifndef EXIT_H
#define EXIT_H

#include "tile.h"

class Exit : public Tile
{
public:
	Exit(int x, int y);
	~Exit();

	void SetState(State s);
	State GetState();

	void Show();

private:
	State _state = CLOSED;
};

#endif EXIT_H