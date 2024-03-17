#ifndef POOL_H
#define POOL_H

#include "tile.h"

class Pool : public Tile
{
public:
	Pool(int x, int y, Element e);
	~Pool();

	void SetElememt(Element e);
	Element GetElement();

	void Show();

private:
	Element _element;
};

#endif POOL_H