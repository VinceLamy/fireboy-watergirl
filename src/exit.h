#ifndef EXIT_H
#define EXIT_H

#define RESET	"\033[0m"
#define MAGENTA	"\033[35m"

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