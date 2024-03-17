#ifndef WALL_H
#define WALL_H

#include "tile.h"

class Wall : public Tile
{
public:
	Wall(int x, int y);
	~Wall();
	void Show();
};

#endif WALL_H